<script setup lang="ts">
import { onMounted, ref } from 'vue'
import {
  CheckCircle2,
  XCircle,
  RotateCcw,
  ShieldCheck,
  ChevronLeft,
  ChevronRight,
  Eye,
  Check,
  AlertTriangle,
  Info,
  Sparkles,
} from 'lucide-vue-next'
import type { CertificationAnswer } from '@/core/domain/entities/CertificationQuestion.entity'
import { useCertificationQuiz } from '@/composables/useCertificationQuiz'
import { Badge } from '@/components/ui/badge'
import { Button } from '@/components/ui/button'

const {
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
} = useCertificationQuiz()

defineProps<{ bare?: boolean }>()

const showResetConfirm = ref(false)
const hasResumed = ref(false)

onMounted(() => {
  void loadQuiz()
})

function handleAnswer(answer: CertificationAnswer): void {
  answerQuestion(answer)
}

function handleSelectAnswerInReview(questionId: number, answer: CertificationAnswer): void {
  setQuestionAnswer(questionId, answer)
}

function handleResumeQuiz(): void {
  resumeQuiz()
  hasResumed.value = true
}

function handleDismissDraft(): void {
  resetQuiz()
  hasResumed.value = true
}

function confirmReset(): void {
  resetQuiz()
  showResetConfirm.value = false
  hasResumed.value = false
}
</script>

<template>
  <div class="w-full h-full flex flex-col px-4 md:px-8 py-3 space-y-3 overflow-hidden font-body text-white">
    <!-- Bandeau de reprise sur interruption (Exigence 3.4) -->
    <div
      v-if="!isLoading && !error && !isFinished && hasSavedDraft && !hasResumed"
      class="shrink-0 p-4 rounded-xl border border-[var(--theme-accent-primary,#1a6fff)]/40 bg-[var(--theme-accent-primary,#1a6fff)]/15 flex flex-col sm:flex-row items-start sm:items-center justify-between gap-3 shadow-lg"
    >
      <div class="flex items-center gap-3">
        <div class="w-9 h-9 rounded-lg bg-[var(--theme-accent-primary,#1a6fff)]/25 text-[var(--theme-accent-primary,#1a6fff)] flex items-center justify-center shrink-0">
          <RotateCcw class="w-5 h-5" />
        </div>
        <div>
          <p class="text-sm font-bold text-white font-heading">Passation en cours sauvegardée</p>
          <p class="text-xs text-white/70 font-body">
            Une session de questionnaire a été interrompue. Vous pouvez reprendre là où vous vous étiez arrêté.
          </p>
        </div>
      </div>

      <div class="flex items-center gap-2 self-end sm:self-center">
        <Button
          variant="outline"
          size="sm"
          class="text-xs border-white/20 text-white hover:bg-white/10 cursor-pointer"
          @click="handleDismissDraft"
        >
          Recommencer à zéro
        </Button>
        <Button
          size="sm"
          class="bg-[var(--theme-accent-primary,#1a6fff)] hover:brightness-110 text-white font-bold text-xs gap-1.5 cursor-pointer shadow"
          @click="handleResumeQuiz"
        >
          <RotateCcw class="w-3.5 h-3.5" />
          <span>Reprendre le questionnaire où vous en étiez</span>
        </Button>
      </div>
    </div>
    <!-- Écran de chargement -->
    <div
      v-if="isLoading"
      class="flex-1 flex flex-col items-center justify-center text-sm text-white/60 font-body gap-3"
    >
      <div class="w-8 h-8 border-2 border-[var(--theme-accent-primary,#1a6fff)] border-t-transparent rounded-full animate-spin" />
      <span>Chargement du questionnaire de certification...</span>
    </div>

    <!-- Écran d'erreur -->
    <div
      v-else-if="error"
      class="flex-1 flex flex-col items-center justify-center text-sm text-red-400 font-body gap-2"
    >
      <AlertTriangle class="w-8 h-8 text-red-400" />
      <span>{{ error }}</span>
      <Button variant="outline" size="sm" class="mt-2 text-xs cursor-pointer" @click="loadQuiz">
        Réessayer
      </Button>
    </div>

    <!-- ÉCRAN 1 : RÉSULTATS DÉFINITIFS & JUSTIFICATIONS PÉDAGOGIQUES -->
    <template v-else-if="isFinished">
      <div class="flex-1 min-h-0 flex flex-col overflow-y-auto space-y-6 pr-1 pb-6">
        <!-- Carte Synthèse Résultat -->
        <div
          class="flex flex-col items-center justify-center text-center px-6 py-8 rounded-2xl border bg-[#14141d] shadow-lg shrink-0"
          :class="isPassed ? 'border-emerald-500/40 bg-emerald-950/10' : 'border-red-500/40 bg-red-950/10'"
        >
          <div
            class="w-16 h-16 rounded-full flex items-center justify-center mb-3 shadow-inner"
            :class="isPassed ? 'bg-emerald-500/20 text-emerald-400 ring-2 ring-emerald-500/40' : 'bg-red-500/20 text-red-400 ring-2 ring-red-500/40'"
          >
            <ShieldCheck class="w-9 h-9" />
          </div>

          <Badge
            variant="pill"
            class="mb-2 text-xs font-semibold px-3 py-1"
            :class="isPassed ? 'bg-emerald-500/20 text-emerald-300 border border-emerald-500/30' : 'bg-red-500/20 text-red-300 border border-red-500/30'"
          >
            {{ isPassed ? 'Badge de certification JEB validé' : 'Score insuffisant (80% requis)' }}
          </Badge>

          <h2 class="text-2xl md:text-3xl font-bold text-white tracking-tight font-heading">
            {{ isPassed ? 'Félicitations, Certification Obtenue !' : 'Résultat insuffisant' }}
          </h2>

          <p class="text-base text-white/80 font-medium mt-1">
            Score final : <span class="font-bold text-white text-lg">{{ score }} / {{ totalQuestions }}</span>
            ({{ scorePercent }}%)
          </p>

          <p class="text-xs text-white/50 max-w-md mt-2">
            {{ isPassed
              ? 'Votre badge de certification JEB est attribué à votre profil. Il atteste de vos aptitudes transversales auprès des recruteurs.'
              : 'L\'obtention de la certification requiert 16 bonnes réponses sur 20 (80%). Vous pouvez reprendre l\'évaluation pour progresser.'
            }}
          </p>

          <div class="flex items-center gap-3 mt-5">
            <Button
              variant="outline"
              size="sm"
              class="gap-2 font-bold cursor-pointer border-white/20 hover:bg-white/10"
              @click="confirmReset"
            >
              <RotateCcw class="w-4 h-4" />
              <span>Recommencer le questionnaire</span>
            </Button>
          </div>
        </div>

        <!-- Section Revue Pédagogique détaillée (20 questions avec justification 5 lignes) -->
        <div class="space-y-4">
          <div class="flex items-center justify-between border-b border-white/10 pb-3">
            <div class="flex items-center gap-2">
              <Sparkles class="w-5 h-5 text-[var(--theme-accent-primary,#1a6fff)]" />
              <h3 class="text-lg font-bold font-heading text-white">
                Analyse pédagogique & Critères d'évaluation (20 questions)
              </h3>
            </div>
            <span class="text-xs text-white/50">
              {{ score }} correctes sur {{ totalQuestions }}
            </span>
          </div>

          <div class="grid grid-cols-1 gap-4">
            <div
              v-for="(q, idx) in questions"
              :key="q.id"
              class="rounded-xl border p-4 transition-all bg-[#14141d]"
              :class="answers[q.id] === q.reponse ? 'border-emerald-500/30' : 'border-red-500/30'"
            >
              <div class="flex items-start justify-between gap-3 mb-2">
                <div class="flex items-center gap-2 flex-wrap">
                  <span class="w-6 h-6 rounded-full bg-white/10 text-white font-bold text-xs flex items-center justify-center shrink-0">
                    {{ idx + 1 }}
                  </span>
                  <Badge variant="pill" class="text-[11px] bg-white/5 text-white/70 border border-white/10">
                    {{ q.categorie }}
                  </Badge>
                  <span class="text-[10px] text-white/40 uppercase tracking-wider">Format {{ q.type }}</span>
                </div>

                <Badge
                  variant="pill"
                  class="text-[11px] font-semibold px-2.5 py-0.5"
                  :class="answers[q.id] === q.reponse ? 'bg-emerald-500/20 text-emerald-300' : 'bg-red-500/20 text-red-300'"
                >
                  {{ answers[q.id] === q.reponse ? 'Correct' : 'Erreur' }}
                </Badge>
              </div>

              <!-- Question -->
              <p class="text-sm md:text-base font-semibold text-white mb-3">
                {{ q.question }}
              </p>

              <!-- Réponses comparées -->
              <div class="flex items-center gap-4 text-xs mb-3 bg-black/30 p-2.5 rounded-lg border border-white/5">
                <div>
                  <span class="text-white/50">Votre réponse : </span>
                  <span
                    class="font-bold ml-1 px-2 py-0.5 rounded"
                    :class="answers[q.id] === q.reponse ? 'text-emerald-300 bg-emerald-500/20' : 'text-red-300 bg-red-500/20'"
                  >
                    {{ answers[q.id] ?? 'Non répondu' }}
                  </span>
                </div>
                <div>
                  <span class="text-white/50">Réponse attendue : </span>
                  <span class="font-bold text-emerald-300 ml-1 px-2 py-0.5 rounded bg-emerald-500/20">
                    {{ q.reponse }}
                  </span>
                </div>
              </div>

              <!-- Justification pédagogique en 5 lignes -->
              <div class="rounded-lg bg-[#0e0e16] p-3 border border-white/10">
                <div class="text-[11px] font-bold text-[var(--theme-accent-primary,#1a6fff)] uppercase tracking-wider mb-1.5 flex items-center gap-1.5">
                  <Info class="w-3.5 h-3.5" />
                  <span>Justification pédagogique & grille de compétences</span>
                </div>
                <pre class="text-xs text-white/80 whitespace-pre-line font-sans leading-relaxed">{{ q.justification }}</pre>
              </div>
            </div>
          </div>
        </div>
      </div>
    </template>

    <!-- ÉCRAN 2 : REVUE DES RÉPONSES AVANT VALIDATION FINALE -->
    <template v-else-if="isReviewing">
      <div class="flex-1 min-h-0 flex flex-col overflow-y-auto space-y-4 pr-1 pb-6">
        <!-- Bandeau de synthèse revue -->
        <div class="rounded-2xl border border-white/10 bg-[#14141d] p-5 shadow-lg shrink-0">
          <div class="flex flex-col md:flex-row md:items-center justify-between gap-4">
            <div>
              <div class="flex items-center gap-2 text-xs font-semibold text-[var(--theme-accent-primary,#1a6fff)] uppercase tracking-wider mb-1">
                <Eye class="w-4 h-4" />
                <span>Contrôle & Synthèse</span>
              </div>
              <h2 class="text-xl md:text-2xl font-bold text-white font-heading">
                Revue des réponses avant validation finale
              </h2>
              <p class="text-xs md:text-sm text-white/60 mt-1">
                Vérifiez ou modifiez vos réponses pour chacune des 20 questions avant de soumettre votre certification.
              </p>
            </div>

            <div class="flex items-center gap-3">
              <Badge
                variant="pill"
                class="px-3 py-1.5 text-xs font-bold"
                :class="hasAllAnswered ? 'bg-emerald-500/20 text-emerald-300 border border-emerald-500/30' : 'bg-amber-500/20 text-amber-300 border border-amber-500/30'"
              >
                {{ answeredCount }} / {{ totalQuestions }} questions complétées
              </Badge>
            </div>
          </div>

          <!-- Actions de revue -->
          <div class="flex flex-wrap items-center gap-3 mt-4 pt-4 border-t border-white/10">
            <Button
              variant="outline"
              size="sm"
              class="gap-1.5 text-xs cursor-pointer border-white/20 hover:bg-white/10"
              @click="backToQuestions"
            >
              <ChevronLeft class="w-4 h-4" />
              <span>Retour au questionnaire</span>
            </Button>

            <Button
              variant="cyan"
              size="sm"
              class="gap-2 text-xs font-bold cursor-pointer ml-auto bg-[var(--theme-accent-primary,#1a6fff)] hover:brightness-110 text-white shadow-md"
              @click="submitFinalValidation"
            >
              <Check class="w-4 h-4" />
              <span>Valider définitivement ma certification</span>
            </Button>
          </div>
        </div>

        <!-- Liste des 20 questions à revoir -->
        <div class="space-y-3">
          <div
            v-for="(q, idx) in questions"
            :key="q.id"
            class="rounded-xl border border-white/10 bg-[#12121c] p-4 transition-all hover:border-white/25"
          >
            <div class="flex flex-col md:flex-row md:items-center justify-between gap-3">
              <div class="flex-1 min-w-0">
                <div class="flex items-center gap-2 flex-wrap mb-1">
                  <span class="w-6 h-6 rounded-full bg-white/10 text-white font-bold text-xs flex items-center justify-center shrink-0">
                    {{ idx + 1 }}
                  </span>
                  <Badge variant="pill" class="text-[10px] bg-white/5 text-white/70 border border-white/10">
                    {{ q.categorie }}
                  </Badge>
                  <span class="text-[10px] text-white/40">({{ q.type }})</span>
                </div>
                <p class="text-sm font-medium text-white line-clamp-2">
                  {{ q.question }}
                </p>
              </div>

              <!-- Choix d'option rapide ou statut -->
              <div class="flex items-center gap-2 shrink-0">
                <Button
                  v-for="opt in (q.type === 'Oui/Non' ? ['Oui', 'Non'] : ['Vrai', 'Faux']) as CertificationAnswer[]"
                  :key="opt"
                  size="sm"
                  variant="outline"
                  class="h-8 px-3 text-xs font-bold rounded-lg cursor-pointer transition-colors"
                  :class="answers[q.id] === opt ? 'bg-[var(--theme-accent-primary,#1a6fff)] text-white border-[var(--theme-accent-primary,#1a6fff)]' : 'bg-white/5 text-white/70 border-white/10 hover:bg-white/10'"
                  @click="handleSelectAnswerInReview(q.id, opt)"
                >
                  {{ opt }}
                </Button>

                <Button
                  size="sm"
                  variant="ghost"
                  class="h-8 px-2 text-xs text-white/50 hover:text-white cursor-pointer"
                  title="Aller à la question"
                  @click="goToQuestion(idx)"
                >
                  Modifier
                </Button>
              </div>
            </div>
          </div>
        </div>

        <!-- Bouton de soumission finale bas de page -->
        <div class="pt-2 flex justify-end">
          <Button
            variant="cyan"
            size="lg"
            class="w-full md:w-auto gap-2 font-bold cursor-pointer bg-[var(--theme-accent-primary,#1a6fff)] hover:brightness-110 text-white shadow-lg"
            @click="submitFinalValidation"
          >
            <Check class="w-5 h-5" />
            <span>Valider définitivement ma certification ({{ answeredCount }}/{{ totalQuestions }})</span>
          </Button>
        </div>
      </div>
    </template>

    <!-- ÉCRAN 3 : QUESTIONNAIRE EN COURS (QUESTION X SUR 20) -->
    <template v-else-if="currentQuestion && currentOptions">
      <div class="flex-1 min-h-0 flex flex-col justify-between overflow-hidden">
        <!-- Barre de progression supérieure & Métadonnées -->
        <div class="shrink-0 space-y-2 mb-2">
          <div class="flex items-center justify-between text-xs font-semibold text-white/70">
            <div class="flex items-center gap-2">
              <span class="font-bold text-white text-sm">Question {{ currentIndex + 1 }} / {{ totalQuestions }}</span>
              <span class="text-white/40 hidden sm:inline">•</span>
              <span class="text-white/50 text-[11px] hidden sm:inline">
                {{ answeredCount }} répondues
              </span>
            </div>

            <div class="flex items-center gap-2">
              <Badge
                variant="pill"
                class="border border-[var(--theme-accent-primary,#1a6fff)]/30 bg-[var(--theme-accent-primary,#1a6fff)]/10 text-[var(--theme-accent-primary,#1a6fff)] font-medium text-[11px] px-3 py-0.5 rounded-full"
              >
                {{ currentCategory }}
              </Badge>

              <button
                type="button"
                class="text-[11px] text-white/50 hover:text-white underline cursor-pointer ml-1"
                @click="goToReview"
              >
                Revoir ({{ answeredCount }}/{{ totalQuestions }})
              </button>
            </div>
          </div>

          <!-- Jauge de progression 1 à 20 -->
          <div class="h-2 w-full rounded-full bg-white/10 overflow-hidden relative">
            <div
              class="h-full bg-gradient-to-r from-[var(--theme-accent-primary,#1a6fff)] to-cyan-400 transition-all duration-300 rounded-full"
              :style="{ width: `${progress}%` }"
            />
          </div>

          <!-- Mini indicateur de questions 1 à 20 cliquables -->
          <div class="hidden sm:flex items-center justify-between gap-1 pt-1">
            <button
              v-for="(q, idx) in questions"
              :key="q.id"
              type="button"
              class="flex-1 h-1.5 rounded-full transition-all cursor-pointer"
              :class="{
                'bg-[var(--theme-accent-primary,#1a6fff)] ring-1 ring-white': idx === currentIndex,
                'bg-emerald-400/80': idx !== currentIndex && answers[q.id],
                'bg-white/15 hover:bg-white/30': idx !== currentIndex && !answers[q.id],
              }"
              :title="`Question ${idx + 1} (${answers[q.id] ? 'Répondue' : 'En attente'})`"
              @click="goToQuestion(idx)"
            />
          </div>
        </div>

        <!-- Carte Centrale de la Question -->
        <div class="flex-1 min-h-0 rounded-2xl border border-white/10 bg-[#14141d] shadow-xl flex flex-col justify-between overflow-hidden p-6 md:p-10 my-2">
          <!-- Haut de la carte : Numéro & Catégorie -->
          <div class="flex items-center justify-between text-xs text-white/50 border-b border-white/10 pb-3 shrink-0">
            <span class="font-mono text-[11px] uppercase tracking-wider text-white/60">
              Catégorie {{ Math.floor(currentIndex / 5) + 1 }} / 4 : {{ currentCategory }}
            </span>
            <span class="text-[11px] font-mono text-white/40">
              Format : {{ currentQuestion.type }}
            </span>
          </div>

          <!-- Texte de la Question -->
          <div class="flex-1 min-h-0 flex items-center justify-center my-4 overflow-y-auto px-2">
            <p class="text-lg md:text-2xl font-bold leading-relaxed text-white text-center font-heading max-w-2xl">
              {{ currentQuestion.question }}
            </p>
          </div>

          <!-- Boutons Options de Réponse (Vrai/Faux ou Oui/Non) -->
          <div class="flex items-stretch gap-4 w-full max-w-lg mx-auto shrink-0 pt-2">
            <Button
              variant="outline"
              size="lg"
              class="flex-1 h-14 rounded-xl gap-2 text-base font-bold border-white/15 bg-[#181824] text-white hover:bg-emerald-500/20 hover:border-emerald-400 cursor-pointer transition-all shadow-md"
              :class="{
                'ring-2 ring-emerald-400 bg-emerald-500/25 border-emerald-400': currentAnswer === currentOptions[0],
              }"
              @click="handleAnswer(currentOptions[0])"
            >
              <CheckCircle2 class="w-5 h-5 text-emerald-400 shrink-0" />
              <span>{{ currentOptions[0] }}</span>
            </Button>

            <Button
              variant="outline"
              size="lg"
              class="flex-1 h-14 rounded-xl gap-2 text-base font-bold border-white/15 bg-[#181824] text-white hover:bg-red-500/20 hover:border-red-400 cursor-pointer transition-all shadow-md"
              :class="{
                'ring-2 ring-red-400 bg-red-500/25 border-red-400': currentAnswer === currentOptions[1],
              }"
              @click="handleAnswer(currentOptions[1])"
            >
              <XCircle class="w-5 h-5 text-red-400 shrink-0" />
              <span>{{ currentOptions[1] }}</span>
            </Button>
          </div>
        </div>

        <!-- Barre de Navigation Inférieure & Sauvegarde -->
        <div class="shrink-0 flex items-center justify-between pt-2 text-xs text-white/60">
          <div class="flex items-center gap-2">
            <Button
              variant="ghost"
              size="sm"
              class="h-8 px-2.5 gap-1 text-white/70 hover:text-white cursor-pointer"
              :disabled="currentIndex === 0"
              @click="previousQuestion"
            >
              <ChevronLeft class="w-4 h-4" />
              <span class="hidden sm:inline">Précédente</span>
            </Button>

            <Button
              variant="ghost"
              size="sm"
              class="h-8 px-2.5 gap-1 text-white/70 hover:text-white cursor-pointer"
              :disabled="currentIndex >= totalQuestions - 1"
              @click="nextQuestion"
            >
              <span class="hidden sm:inline">Suivante</span>
              <ChevronRight class="w-4 h-4" />
            </Button>
          </div>

          <div class="flex items-center gap-2">
            <!-- Bouton Revoir les réponses avant validation finale -->
            <Button
              variant="outline"
              size="sm"
              class="h-8 px-3 gap-1.5 font-semibold text-xs border-white/20 bg-white/5 hover:bg-white/10 text-white cursor-pointer"
              @click="goToReview"
            >
              <Eye class="w-3.5 h-3.5 text-[var(--theme-accent-primary,#1a6fff)]" />
              <span>Revoir les réponses ({{ answeredCount }}/{{ totalQuestions }})</span>
            </Button>

            <!-- Réinitialiser avec dialogue -->
            <button
              v-if="!showResetConfirm"
              type="button"
              class="text-[11px] text-white/40 hover:text-red-400 cursor-pointer ml-1"
              title="Réinitialiser le questionnaire"
              @click="showResetConfirm = true"
            >
              Réinitialiser
            </button>
            <div v-else class="flex items-center gap-1 bg-red-950/80 px-2 py-1 rounded border border-red-500/40">
              <span class="text-[10px] text-red-300">Confirmer ?</span>
              <button
                type="button"
                class="text-[10px] font-bold text-red-200 underline cursor-pointer"
                @click="confirmReset"
              >
                Oui
              </button>
              <button
                type="button"
                class="text-[10px] text-white/50 cursor-pointer ml-1"
                @click="showResetConfirm = false"
              >
                Non
              </button>
            </div>
          </div>
        </div>
      </div>
    </template>
  </div>
</template>
