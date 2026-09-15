<script setup lang="ts">
import { ref, shallowRef, computed, onMounted } from 'vue'
import {
  Plus,
  Edit2,
  Trash2,
  RotateCcw,
  Search,
  CheckCircle2,
  BookOpen,
  Award,
  Layers,
  Sliders,
} from 'lucide-vue-next'
import AdminHeader from '@/components/admin/AdminHeader.vue'
import { AdminCertificationService } from '@/core/services/AdminCertificationService'
import type {
  AdminCertificationQuestionEntity,
  QuestionDifficulty,
} from '@/core/domain/entities/AdminCertificationQuestion.entity'
import type { CertificationAnswer } from '@/core/domain/entities/CertificationQuestion.entity'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
} from '@/components/ui/dialog'

const certService = new AdminCertificationService()

const questions = shallowRef<AdminCertificationQuestionEntity[]>([])
const categories = ref<string[]>([])
const selectedCategory = ref<string>('Toutes')
const searchQuery = ref<string>('')
const isLoading = ref<boolean>(true)
const notificationMessage = ref<string | null>(null)

// Formulaire d'édition / création
const isFormModalOpen = ref<boolean>(false)
const isEditing = ref<boolean>(false)
const formQuestionId = ref<number | null>(null)
const formCategory = ref<string>('Travail')
const formType = ref<string>('Vrai/Faux')
const formQuestionText = ref<string>('')
const formAnswer = ref<CertificationAnswer>('Vrai')
const formWeight = ref<number>(1)
const formDifficulty = ref<QuestionDifficulty>('Moyen')
const formSuccessRate = ref<number>(85)

// Modale de confirmation de suppression
const isDeleteModalOpen = ref<boolean>(false)
const questionToDelete = ref<Pick<AdminCertificationQuestionEntity, 'id' | 'question'> | null>(null)

async function loadData(): Promise<void> {
  isLoading.value = true
  try {
    const [qList, cats] = await Promise.all([
      certService.getQuestions(),
      certService.getCategories(),
    ])
    questions.value = qList
    categories.value = ['Toutes', ...cats]
  } catch {
    // Erreur de chargement
  } finally {
    isLoading.value = false
  }
}

const filteredQuestions = computed(() => {
  return questions.value.filter((q: AdminCertificationQuestionEntity) => {
    const matchCategory = selectedCategory.value === 'Toutes' || q.categorie === selectedCategory.value
    const matchSearch =
      !searchQuery.value.trim() ||
      q.question.toLowerCase().includes(searchQuery.value.toLowerCase()) ||
      q.categorie.toLowerCase().includes(searchQuery.value.toLowerCase())
    return matchCategory && matchSearch
  })
})

const totalPoints = computed(() => {
  return questions.value.reduce((sum: number, q: AdminCertificationQuestionEntity) => sum + (q.isActive ? q.ponderation : 0), 0)
})

function showNotification(msg: string): void {
  notificationMessage.value = msg
  setTimeout(() => {
    notificationMessage.value = null
  }, 4000)
}

function openCreateModal(): void {
  isEditing.value = false
  formQuestionId.value = null
  formCategory.value = 'Travail'
  formType.value = 'Vrai/Faux'
  formQuestionText.value = ''
  formAnswer.value = 'Vrai'
  formWeight.value = 1
  formDifficulty.value = 'Moyen'
  formSuccessRate.value = 85
  isFormModalOpen.value = true
}

function openEditModal(q: Pick<AdminCertificationQuestionEntity, 'id' | 'categorie' | 'type' | 'question' | 'reponse' | 'ponderation' | 'difficulty' | 'successRate'>): void {
  isEditing.value = true
  formQuestionId.value = q.id
  formCategory.value = q.categorie
  formType.value = q.type
  formQuestionText.value = q.question
  formAnswer.value = q.reponse
  formWeight.value = q.ponderation
  formDifficulty.value = q.difficulty
  formSuccessRate.value = q.successRate
  isFormModalOpen.value = true
}

async function saveQuestion(): Promise<void> {
  if (!formQuestionText.value.trim()) {
    alert('Veuillez saisir le texte de la question.')
    return
  }

  try {
    if (isEditing.value && formQuestionId.value !== null) {
      await certService.updateQuestion(formQuestionId.value, {
        categorie: formCategory.value,
        type: formType.value,
        question: formQuestionText.value.trim(),
        reponse: formAnswer.value,
        ponderation: Number(formWeight.value) || 1,
        difficulty: formDifficulty.value,
        successRate: Number(formSuccessRate.value) || 85,
      })
      showNotification(`Question #${formQuestionId.value} modifiée avec succès.`)
    } else {
      await certService.createQuestion({
        categorie: formCategory.value,
        type: formType.value,
        question: formQuestionText.value.trim(),
        reponse: formAnswer.value,
        ponderation: Number(formWeight.value) || 1,
        difficulty: formDifficulty.value,
        successRate: Number(formSuccessRate.value) || 85,
        isActive: true,
      })
      showNotification('Nouvelle question ajoutée au référentiel avec succès.')
    }
    isFormModalOpen.value = false
    await loadData()
  } catch (err: any) {
    alert(err.message || "Erreur lors de l'enregistrement")
  }
}

function openDeleteModal(q: Pick<AdminCertificationQuestionEntity, 'id' | 'question'>): void {
  questionToDelete.value = q
  isDeleteModalOpen.value = true
}

async function confirmDelete(): Promise<void> {
  if (!questionToDelete.value) return
  try {
    await certService.deleteQuestion(questionToDelete.value.id)
    showNotification(`Question #${questionToDelete.value.id} supprimée du référentiel.`)
    isDeleteModalOpen.value = false
    questionToDelete.value = null
    await loadData()
  } catch (err: any) {
    alert(err.message || 'Erreur lors de la suppression')
  }
}

async function handleReset(): Promise<void> {
  if (!confirm('Réinitialiser le référentiel aux 20 questions officielles par défaut ?')) {
    return
  }
  try {
    await certService.resetToDefaults()
    showNotification('Le référentiel a été réinitialisé aux 20 questions officielles.')
    await loadData()
  } catch (err: any) {
    alert(err.message || 'Erreur lors de la réinitialisation')
  }
}

onMounted(() => {
  void loadData()
})
</script>

<template>
  <div class="min-h-screen bg-[#09090f] text-foreground font-body pb-16">
    <!-- En-tête Institutionnel Marianne DSFR -->
    <AdminHeader current-view-title="Référentiel certification" />

    <div class="container mx-auto px-4 sm:px-6 py-8 max-w-7xl space-y-6">
      <!-- Notification Toast -->
      <div
        v-if="notificationMessage"
        class="p-4 rounded-xl bg-emerald-500/15 border border-emerald-500/40 text-emerald-300 text-sm flex items-center justify-between animate-in fade-in slide-in-from-top-2 duration-200"
      >
        <div class="flex items-center gap-2">
          <CheckCircle2 class="w-4 h-4 text-emerald-400 shrink-0" />
          <span>{{ notificationMessage }}</span>
        </div>
        <button
          class="text-xs text-emerald-400 hover:underline cursor-pointer"
          @click="notificationMessage = null"
        >
          Fermer
        </button>
      </div>

      <!-- Titre & Statistiques du Référentiel -->
      <div class="flex flex-col lg:flex-row lg:items-center justify-between gap-4 border-b border-white/10 pb-6">
        <div>
          <div class="flex items-center gap-2.5">
            <h2 class="text-2xl font-bold font-heading text-white tracking-tight">
              Référentiel National des 20 Questions de Certification
            </h2>
            <Badge class="bg-[#000091] text-white border border-blue-400/30 text-xs font-bold font-heading">
              {{ questions.length }} questions
            </Badge>
          </div>
          <p class="text-xs sm:text-sm text-muted-foreground font-body mt-1">
            Gestion, consultation et pondération du questionnaire standardisé pour l'attribution du badge officiel.
          </p>
        </div>

        <div class="flex items-center gap-2.5 flex-wrap">
          <Button
            variant="outline"
            size="sm"
            class="border-white/15 hover:bg-white/10 text-white text-xs h-9 gap-1.5 cursor-pointer"
            @click="handleReset"
          >
            <RotateCcw class="w-3.5 h-3.5 text-amber-400" />
            <span>Réinitialiser (20 officielles)</span>
          </Button>

          <Button
            variant="default"
            size="sm"
            class="bg-[#000091] hover:bg-[#0000b8] text-white text-xs h-9 gap-1.5 cursor-pointer border border-blue-400/40 font-bold font-heading"
            @click="openCreateModal"
          >
            <Plus class="w-4 h-4" />
            <span>Ajouter une question</span>
          </Button>
        </div>
      </div>

      <!-- Résumé des Métriques du Référentiel -->
      <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-4 text-xs">
        <div class="p-4 rounded-xl bg-[#0e0e16] border border-white/10 flex items-center justify-between">
          <div>
            <p class="text-muted-foreground">Volume total</p>
            <p class="text-xl font-bold text-white mt-0.5 font-heading">{{ questions.length }} questions</p>
          </div>
          <BookOpen class="w-6 h-6 text-blue-400" />
        </div>

        <div class="p-4 rounded-xl bg-[#0e0e16] border border-white/10 flex items-center justify-between">
          <div>
            <p class="text-muted-foreground">Total des points</p>
            <p class="text-xl font-bold text-white mt-0.5 font-heading">{{ totalPoints }} points</p>
          </div>
          <Sliders class="w-6 h-6 text-purple-400" />
        </div>

        <div class="p-4 rounded-xl bg-[#0e0e16] border border-white/10 flex items-center justify-between">
          <div>
            <p class="text-muted-foreground">Validation requise</p>
            <p class="text-xl font-bold text-emerald-400 mt-0.5 font-heading">≥ 80% (16 / 20)</p>
          </div>
          <Award class="w-6 h-6 text-emerald-400" />
        </div>

        <div class="p-4 rounded-xl bg-[#0e0e16] border border-white/10 flex items-center justify-between">
          <div>
            <p class="text-muted-foreground">Pondération par défaut</p>
            <p class="text-xl font-bold text-white mt-0.5 font-heading">1 point / question</p>
          </div>
          <Layers class="w-6 h-6 text-amber-400" />
        </div>
      </div>

      <!-- Barre de Filtres & Recherche -->
      <div class="flex flex-col md:flex-row md:items-center justify-between gap-4">
        <!-- Filtres par catégorie -->
        <div class="flex items-center gap-1.5 overflow-x-auto py-1">
          <button
            v-for="cat in categories"
            :key="cat"
            class="px-3 py-1.5 rounded-lg border text-xs font-semibold cursor-pointer transition-all whitespace-nowrap"
            :class="[
              selectedCategory === cat
                ? 'bg-[#000091] text-white border-blue-400 shadow-sm font-bold'
                : 'bg-white/5 text-muted-foreground border-white/10 hover:text-white',
            ]"
            @click="selectedCategory = cat"
          >
            {{ cat }}
          </button>
        </div>

        <!-- Recherche textuelle -->
        <div class="relative w-full md:w-80">
          <Search class="w-4 h-4 absolute left-3.5 top-1/2 -translate-y-1/2 text-muted-foreground" />
          <input
            v-model="searchQuery"
            type="text"
            placeholder="Rechercher dans l'intitulé..."
            class="w-full h-9 pl-9 pr-4 bg-[#0e0e16] border border-white/15 rounded-xl text-xs text-white placeholder:text-muted-foreground focus:outline-none focus:border-blue-500 transition-colors"
          />
        </div>
      </div>

      <!-- Liste / Cartes des Questions -->
      <div v-if="isLoading" class="p-12 text-center text-muted-foreground text-sm">
        Chargement des questions du référentiel...
      </div>

      <div v-else-if="filteredQuestions.length === 0" class="p-12 rounded-2xl bg-[#0e0e16] border border-white/10 text-center space-y-2">
        <BookOpen class="w-10 h-10 text-muted-foreground/40 mx-auto" />
        <p class="text-sm font-semibold text-white">Aucune question ne correspond à votre filtre.</p>
        <p class="text-xs text-muted-foreground">Modifiez votre catégorie ou réinitialisez la recherche.</p>
      </div>

      <div v-else class="space-y-3">
        <div
          v-for="q in filteredQuestions"
          :key="q.id"
          class="p-4 sm:p-5 rounded-2xl bg-[#0e0e16] border border-white/10 hover:border-white/20 transition-all flex flex-col md:flex-row md:items-center justify-between gap-4"
        >
          <!-- Contenu de la Question -->
          <div class="flex-1 min-w-0 space-y-2">
            <div class="flex items-center gap-2 flex-wrap text-xs">
              <span class="font-bold text-blue-300 font-heading">#{{ q.id }}</span>
              <span class="px-2 py-0.5 rounded-full bg-white/5 border border-white/10 text-white/80 font-medium">
                {{ q.categorie }}
              </span>
              <span class="text-muted-foreground">Type : {{ q.type }}</span>
              <span class="text-amber-300/90 font-medium">Pondération : {{ q.ponderation }} pt(s)</span>
              <span
                class="px-2 py-0.5 rounded text-[10px] font-bold"
                :class="[
                  q.difficulty === 'Facile'
                    ? 'bg-emerald-500/15 text-emerald-300 border border-emerald-500/30'
                    : q.difficulty === 'Moyen'
                      ? 'bg-blue-500/15 text-blue-300 border border-blue-500/30'
                      : 'bg-rose-500/15 text-rose-300 border border-rose-500/30',
                ]"
              >
                {{ q.difficulty }}
              </span>
            </div>

            <p class="text-sm sm:text-base font-semibold text-white font-body leading-snug">
              {{ q.question }}
            </p>

            <div class="flex items-center gap-4 text-xs text-muted-foreground">
              <span>
                Réponse officielle attendue :
                <strong
                  class="font-bold ml-1"
                  :class="q.reponse === 'Vrai' || q.reponse === 'Oui' ? 'text-emerald-400' : 'text-rose-400'"
                >
                  {{ q.reponse }}
                </strong>
              </span>
              <span>Taux de réussite : <strong class="text-white">{{ q.successRate }}%</strong></span>
            </div>
          </div>

          <!-- Actions Administrateur -->
          <div class="flex items-center gap-2 shrink-0 self-end md:self-center">
            <Button
              variant="outline"
              size="sm"
              class="h-8 px-3 border-white/15 text-xs text-white hover:bg-white/10 gap-1.5 cursor-pointer"
              @click="openEditModal(q)"
            >
              <Edit2 class="w-3.5 h-3.5 text-blue-400" />
              <span>Modifier</span>
            </Button>

            <Button
              variant="ghost"
              size="sm"
              class="h-8 px-2.5 text-xs text-rose-400 hover:bg-rose-500/10 hover:text-rose-300 cursor-pointer"
              @click="openDeleteModal(q)"
            >
              <Trash2 class="w-3.5 h-3.5" />
            </Button>
          </div>
        </div>
      </div>
    </div>

    <!-- Modale d'Édition / Création de Question -->
    <Dialog :open="isFormModalOpen" @update:open="(v) => isFormModalOpen = v">
      <DialogContent class="bg-[#0e0e16] border border-white/15 text-white max-w-lg">
        <DialogHeader>
          <DialogTitle class="text-lg font-bold font-heading text-white">
            {{ isEditing ? `Modifier la question #${formQuestionId}` : 'Ajouter une question au référentiel' }}
          </DialogTitle>
          <DialogDescription class="text-xs text-muted-foreground font-body">
            Configurez l'énoncé, la catégorie thématique et la pondération pour le score de certification.
          </DialogDescription>
        </DialogHeader>

        <div class="space-y-4 py-3 text-xs">
          <!-- Intitulé / Énoncé -->
          <div class="space-y-1.5">
            <label class="font-bold text-white text-xs">Énoncé de la question :</label>
            <textarea
              v-model="formQuestionText"
              rows="3"
              class="w-full p-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-blue-500 placeholder:text-muted-foreground"
              placeholder="Saisissez l'énoncé clair et précis..."
            />
          </div>

          <!-- Catégorie & Type -->
          <div class="grid grid-cols-1 sm:grid-cols-2 gap-3">
            <div class="space-y-1.5">
              <label class="font-bold text-white text-xs">Catégorie :</label>
              <select
                v-model="formCategory"
                class="w-full h-10 px-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-blue-500"
              >
                <option value="Travail">Travail</option>
                <option value="Culture générale">Culture générale</option>
                <option value="Comportement en entreprise">Comportement en entreprise</option>
                <option value="Gestion de projets et d'équipe">Gestion de projets et d'équipe</option>
              </select>
            </div>

            <div class="space-y-1.5">
              <label class="font-bold text-white text-xs">Type de question :</label>
              <select
                v-model="formType"
                class="w-full h-10 px-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-blue-500"
                @change="formAnswer = formType === 'Oui/Non' ? 'Oui' : 'Vrai'"
              >
                <option value="Vrai/Faux">Vrai/Faux</option>
                <option value="Oui/Non">Oui/Non</option>
              </select>
            </div>
          </div>

          <!-- Réponse Attendue & Pondération -->
          <div class="grid grid-cols-1 sm:grid-cols-3 gap-3">
            <div class="space-y-1.5">
              <label class="font-bold text-white text-xs">Réponse attendue :</label>
              <select
                v-model="formAnswer"
                class="w-full h-10 px-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-blue-500"
              >
                <template v-if="formType === 'Oui/Non'">
                  <option value="Oui">Oui</option>
                  <option value="Non">Non</option>
                </template>
                <template v-else>
                  <option value="Vrai">Vrai</option>
                  <option value="Faux">Faux</option>
                </template>
              </select>
            </div>

            <div class="space-y-1.5">
              <label class="font-bold text-white text-xs">Pondération (pts) :</label>
              <input
                v-model.number="formWeight"
                type="number"
                min="1"
                max="5"
                class="w-full h-10 px-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-blue-500"
              />
            </div>

            <div class="space-y-1.5">
              <label class="font-bold text-white text-xs">Difficulté :</label>
              <select
                v-model="formDifficulty"
                class="w-full h-10 px-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-blue-500"
              >
                <option value="Facile">Facile</option>
                <option value="Moyen">Moyen</option>
                <option value="Difficile">Difficile</option>
              </select>
            </div>
          </div>
        </div>

        <DialogFooter class="flex sm:justify-between items-center gap-2 pt-2 border-t border-white/10">
          <Button
            variant="ghost"
            size="sm"
            class="text-xs text-muted-foreground hover:text-white cursor-pointer"
            @click="isFormModalOpen = false"
          >
            Annuler
          </Button>

          <Button
            variant="default"
            size="sm"
            class="bg-[#000091] hover:bg-[#0000b8] text-white text-xs font-bold h-9 cursor-pointer"
            @click="saveQuestion"
          >
            {{ isEditing ? 'Enregistrer les modifications' : 'Ajouter la question' }}
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>

    <!-- Modale de Confirmation de Suppression -->
    <Dialog :open="isDeleteModalOpen" @update:open="(v) => isDeleteModalOpen = v">
      <DialogContent class="bg-[#0e0e16] border border-white/15 text-white max-w-md">
        <DialogHeader>
          <DialogTitle class="text-base font-bold font-heading text-rose-400">
            Supprimer la question du référentiel ?
          </DialogTitle>
          <DialogDescription class="text-xs text-muted-foreground font-body">
            Cette action retirera la question du référentiel standardisé de certification.
          </DialogDescription>
        </DialogHeader>

        <div v-if="questionToDelete" class="p-3 rounded-xl bg-white/5 text-xs text-white/90">
          « {{ questionToDelete.question }} »
        </div>

        <DialogFooter class="flex sm:justify-between items-center gap-2 pt-2 border-t border-white/10">
          <Button
            variant="ghost"
            size="sm"
            class="text-xs text-muted-foreground hover:text-white cursor-pointer"
            @click="isDeleteModalOpen = false"
          >
            Annuler
          </Button>

          <Button
            variant="destructive"
            size="sm"
            class="bg-rose-600 hover:bg-rose-500 text-white text-xs font-bold h-9 cursor-pointer"
            @click="confirmDelete"
          >
            Supprimer définitivement
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  </div>
</template>
