import { ref, computed, shallowRef } from 'vue'
import type { CertificationAnswer, CertificationQuestion } from '@/core/domain/entities/CertificationQuestion.entity'
import { getOptions, isCorrectAnswer } from '@/core/domain/entities/CertificationQuestion.entity'
import { CertificationMockRepository } from '@/core/infrastructure/repositories/CertificationMockRepository'
import { HttpClient } from '@/core/infrastructure/http/HttpClient'

const JEB_STATUS_KEY = 'candidate_jeb_certified'
const JEB_QUIZ_IN_PROGRESS_KEY = 'jeb_quiz_in_progress'
const PASS_THRESHOLD = 0.8 // 80% soit 16/20

const repository = new CertificationMockRepository()

// Module-level reactive state to guarantee synchronized state across all consumers
const questions = shallowRef<CertificationQuestion[]>([])
const currentIndex = ref<number>(0)
const answers = ref<Record<number, CertificationAnswer>>({})
const score = ref<number>(0)
const isLoading = ref<boolean>(false)
const isReviewing = ref<boolean>(false)
const isFinished = ref<boolean>(false)
const error = ref<string | null>(null)
const draftVersion = ref<number>(0)

export function useCertificationQuiz() {
  const currentQuestion = computed<CertificationQuestion | null>(() => {
    return questions.value[currentIndex.value] ?? null
  })

  const currentOptions = computed<[CertificationAnswer, CertificationAnswer] | null>(() => {
    return currentQuestion.value ? getOptions(currentQuestion.value) : null
  })

  const totalQuestions = computed<number>(() => questions.value.length)

  const currentCategory = computed<string>(() => {
    return currentQuestion.value?.categorie ?? ''
  })

  const answeredCount = computed<number>(() => {
    return Object.keys(answers.value).length
  })

  const currentAnswer = computed<CertificationAnswer | null>(() => {
    if (!currentQuestion.value) return null
    return answers.value[currentQuestion.value.id] ?? null
  })

  const progress = computed<number>(() => {
    if (totalQuestions.value === 0) return 0
    return Math.round((answeredCount.value / totalQuestions.value) * 100)
  })

  const scorePercent = computed<number>(() => {
    if (totalQuestions.value === 0) return 0
    return Math.round((score.value / totalQuestions.value) * 100)
  })

  const isPassed = computed<boolean>(() => {
    if (totalQuestions.value === 0) return false
    return score.value / totalQuestions.value >= PASS_THRESHOLD
  })

  const hasAllAnswered = computed<boolean>(() => {
    return totalQuestions.value > 0 && answeredCount.value >= totalQuestions.value
  })

  const hasSavedDraft = computed<boolean>(() => {
    void draftVersion.value
    if (typeof localStorage === 'undefined') return false
    try {
      const raw = localStorage.getItem(JEB_QUIZ_IN_PROGRESS_KEY)
      if (!raw) return false
      const parsed = JSON.parse(raw) as { currentIndex?: number; answers?: Record<number, string> }
      return Boolean(
        parsed &&
        typeof parsed === 'object' &&
        parsed.answers &&
        Object.keys(parsed.answers).length > 0
      )
    } catch {
      return false
    }
  })

  function saveDraft(): void {
    if (typeof localStorage === 'undefined' || isFinished.value) return
    try {
      const draft = {
        currentIndex: currentIndex.value,
        answers: answers.value,
      }
      localStorage.setItem(JEB_QUIZ_IN_PROGRESS_KEY, JSON.stringify(draft))
      draftVersion.value++
    } catch (e) {
      console.warn('Impossible de sauvegarder le brouillon du questionnaire JEB', e)
    }
  }

  function clearDraft(): void {
    if (typeof localStorage === 'undefined') return
    try {
      localStorage.removeItem(JEB_QUIZ_IN_PROGRESS_KEY)
      draftVersion.value++
    } catch (e) {
      console.warn('Impossible de nettoyer le brouillon du questionnaire JEB', e)
    }
  }

  function resumeQuiz(): void {
    if (typeof localStorage === 'undefined') return
    try {
      const raw = localStorage.getItem(JEB_QUIZ_IN_PROGRESS_KEY)
      if (!raw) return
      const parsed = JSON.parse(raw) as { currentIndex?: number; answers?: Record<number, CertificationAnswer> }
      if (parsed && typeof parsed === 'object' && parsed.answers) {
        answers.value = { ...parsed.answers }
        if (
          typeof parsed.currentIndex === 'number' &&
          parsed.currentIndex >= 0 &&
          parsed.currentIndex < (questions.value.length || 20)
        ) {
          currentIndex.value = parsed.currentIndex
        }
        isFinished.value = false
        isReviewing.value = false
        draftVersion.value++
      }
    } catch (e) {
      console.warn('Impossible de restaurer le questionnaire JEB sauvegardé', e)
    }
  }

  async function loadQuiz(): Promise<void> {
    if (questions.value.length > 0) {
      return
    }

    isLoading.value = true
    error.value = null
    try {
      const loadedQuestions = await repository.getAllQuestions()

      // Calibrage rigoureux sur exactement 20 questions (5 par catégorie)
      const categoriesOrder = ['Travail', 'Culture', 'Comportement', 'Gestion de projets']
      const calibrated: CertificationQuestion[] = []
      for (const cat of categoriesOrder) {
        const catQuestions = loadedQuestions.filter(
          (q) => q.categorie === cat || q.categorie.toLowerCase().startsWith(cat.toLowerCase())
        )
        calibrated.push(...catQuestions.slice(0, 5))
      }

      questions.value = calibrated.length === 20 ? calibrated : loadedQuestions.slice(0, 20)
      currentIndex.value = 0
      answers.value = {}
      score.value = 0
      isReviewing.value = false
      isFinished.value = false
    } catch (err) {
      error.value = err instanceof Error ? err.message : 'Erreur lors du chargement des questions'
    } finally {
      isLoading.value = false
    }
  }

  function answerQuestion(answer: CertificationAnswer): void {
    const question = currentQuestion.value
    if (!question || isFinished.value) return

    answers.value = {
      ...answers.value,
      [question.id]: answer,
    }

    // Si on est à la dernière question, aller directement à la revue
    if (currentIndex.value >= questions.value.length - 1) {
      isReviewing.value = true
    } else {
      currentIndex.value++
    }

    saveDraft()
  }

  function setQuestionAnswer(questionId: number, answer: CertificationAnswer): void {
    if (isFinished.value) return
    answers.value = {
      ...answers.value,
      [questionId]: answer,
    }
    saveDraft()
  }

  function goToQuestion(index: number): void {
    if (index >= 0 && index < questions.value.length) {
      currentIndex.value = index
      isReviewing.value = false
      saveDraft()
    }
  }

  function nextQuestion(): void {
    if (currentIndex.value < questions.value.length - 1) {
      currentIndex.value++
      isReviewing.value = false
    } else {
      isReviewing.value = true
    }
    saveDraft()
  }

  function previousQuestion(): void {
    if (currentIndex.value > 0) {
      currentIndex.value--
      isReviewing.value = false
    }
    saveDraft()
  }

  function goToReview(): void {
    isReviewing.value = true
  }

  function backToQuestions(): void {
    isReviewing.value = false
  }

  async function submitFinalValidation(): Promise<void> {
    if (questions.value.length === 0) return

    let calculatedScore = 0
    for (const q of questions.value) {
      const candidateAns = answers.value[q.id]
      if (candidateAns && isCorrectAnswer(q, candidateAns)) {
        calculatedScore++
      }
    }

    score.value = calculatedScore
    isReviewing.value = false
    isFinished.value = true

    // Nettoyer l'état sauvegardé en cours de passation
    clearDraft()

    const total = questions.value.length || 20
    const isSuccess = calculatedScore / total >= PASS_THRESHOLD

    if (isSuccess) {
      // 1. Stocker le statut JEB certifié dans localStorage
      try {
        localStorage.setItem(JEB_STATUS_KEY, 'true')
      } catch (e) {
        console.warn('Impossible de stocker le statut JEB certifié', e)
      }

      // 2. Mettre à jour le profil utilisateur en session
      try {
        const storedUser = localStorage.getItem('auth_user') || localStorage.getItem('user')
        if (storedUser) {
          const userObj = JSON.parse(storedUser)
          userObj.certificated = true
          localStorage.setItem('auth_user', JSON.stringify(userObj))
          localStorage.setItem('user', JSON.stringify(userObj))
        }
      } catch (e) {
        console.warn('Impossible de mettre à jour le profil utilisateur en session', e)
      }

      // 3. Persistance en BDD via appel API HTTP POST /job_seeker/certification
      try {
        const httpClient = new HttpClient()
        await httpClient.post('/job_seeker/certification', {
          score: calculatedScore,
          total,
        })
      } catch (err) {
        console.warn('Erreur lors de la persistance de la certification en BDD:', err)
      }
    }
  }

  function resetQuiz(): void {
    currentIndex.value = 0
    answers.value = {}
    score.value = 0
    isReviewing.value = false
    isFinished.value = false
    error.value = null
    clearDraft()
  }

  return {
    questions,
    currentQuestion,
    currentOptions,
    currentAnswer,
    currentIndex,
    currentCategory,
    totalQuestions,
    answeredCount,
    progress,
    score,
    scorePercent,
    isPassed,
    hasAllAnswered,
    hasSavedDraft,
    resumeQuiz,
    isLoading,
    isReviewing,
    isFinished,
    error,
    answers,
    loadQuiz,
    answerQuestion,
    setQuestionAnswer,
    goToQuestion,
    nextQuestion,
    previousQuestion,
    goToReview,
    backToQuestions,
    submitFinalValidation,
    resetQuiz,
    restartQuiz: resetQuiz,
  }
}
