<script setup lang="ts">
import { ShieldCheck, Eye, RotateCcw } from 'lucide-vue-next'
import { useDevicePreview } from '@/composables/useDevicePreview'
import { useCertificationQuiz } from '@/composables/useCertificationQuiz'
import DevicePreviewBar from '@/components/common/DevicePreviewBar.vue'
import QuizViewer from '@/components/certification/QuizViewer.vue'
import { Badge } from '@/components/ui/badge'
import { Button } from '@/components/ui/button'

const { currentFormat } = useDevicePreview()

const {
  currentIndex,
  totalQuestions,
  currentCategory,
  answeredCount,
  isReviewing,
  isFinished,
  goToReview,
  resetQuiz,
} = useCertificationQuiz()
</script>

<template>
  <div class="h-dvh overflow-hidden bg-[var(--theme-bg-main,#09090f)] text-[var(--theme-text-primary,#ffffff)] flex flex-col items-center select-none font-body">
    <!-- Barre d'outils simulateur de formats d'écran -->
    <div class="w-full py-2 flex justify-center shrink-0">
      <DevicePreviewBar :active-format="currentFormat" @update:format="(f) => (currentFormat = f)" />
    </div>

    <main
      class="w-full flex-1 min-h-0 flex flex-col items-center justify-start overflow-hidden transition-all duration-300 pb-3"
      :class="{
        'max-w-[420px]': currentFormat === 'mobile',
        'max-w-[800px]': currentFormat === 'tablet',
        'max-w-none px-6': currentFormat === 'desktop',
      }"
    >
      <!-- Mode Desktop -->
      <div
        v-if="currentFormat === 'desktop'"
        class="relative w-full h-full flex flex-col max-w-5xl mx-auto"
      >
        <!-- En-tête Desktop avec progression claire et catégorie en cours -->
        <header class="w-full flex items-center justify-between py-2 px-4 mb-2 bg-[#12121a] rounded-xl border border-white/10 shrink-0">
          <div class="flex items-center gap-3">
            <div class="flex items-center gap-2 text-[var(--theme-accent-primary,#1a6fff)] font-bold text-sm font-heading">
              <ShieldCheck class="w-5 h-5" />
              <span>Badge de certification JEB</span>
            </div>

            <!-- Progression claire : Question X sur 20 -->
            <Badge
              v-if="!isFinished"
              variant="pill"
              class="bg-white/10 text-white font-bold text-xs px-3 py-1 border border-white/15"
            >
              <template v-if="isReviewing">
                Revue des réponses ({{ answeredCount }}/{{ totalQuestions || 20 }})
              </template>
              <template v-else>
                Question {{ currentIndex + 1 }} sur {{ totalQuestions || 20 }}
              </template>
            </Badge>

            <!-- Catégorie en cours -->
            <Badge
              v-if="!isFinished && !isReviewing && currentCategory"
              variant="pill"
              class="bg-[var(--theme-accent-primary,#1a6fff)]/15 text-[var(--theme-accent-primary,#1a6fff)] font-medium text-xs px-3 py-1 border border-[var(--theme-accent-primary,#1a6fff)]/30"
            >
              Catégorie : {{ currentCategory }}
            </Badge>
          </div>

          <!-- Actions d'en-tête (Revue des réponses & Réinitialisation) -->
          <div class="flex items-center gap-2.5">
            <Button
              v-if="!isFinished && !isReviewing"
              variant="outline"
              size="sm"
              class="h-8 gap-1.5 text-xs font-semibold border-white/20 hover:bg-white/10 text-white cursor-pointer"
              @click="goToReview"
            >
              <Eye class="w-3.5 h-3.5 text-[var(--theme-accent-primary,#1a6fff)]" />
              <span>Revoir les réponses avant validation finale</span>
            </Button>

            <Button
              variant="ghost"
              size="sm"
              class="h-8 text-xs text-white/50 hover:text-red-400 hover:bg-red-500/10 cursor-pointer"
              title="Réinitialiser la progression"
              @click="resetQuiz"
            >
              <RotateCcw class="w-3.5 h-3.5 mr-1" />
              <span>Réinitialiser</span>
            </Button>
          </div>
        </header>

        <!-- Visualiseur Quiz -->
        <div class="flex-1 min-h-0 w-full rounded-2xl bg-[#0e0e16] border border-white/10 shadow-2xl overflow-hidden flex flex-col">
          <QuizViewer bare />
        </div>
      </div>

      <!-- Mode Mobile / Tablette -->
      <div
        v-else
        class="w-full h-full bg-[var(--theme-bg-card,#0e0e16)] shadow-2xl border-x border-[var(--theme-border-subtle,rgba(255,255,255,0.1))] flex flex-col justify-start rounded-2xl overflow-hidden"
      >
        <!-- En-tête Mobile/Tablette avec Progression et Catégorie -->
        <header class="px-4 pt-3 pb-2 border-b border-white/10 bg-[#12121a] shrink-0 space-y-1.5">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-2 text-[var(--theme-accent-primary,#1a6fff)] font-bold text-xs font-heading">
              <ShieldCheck class="w-4 h-4" />
              <span>Certification JEB</span>
            </div>

            <!-- Progression claire Question X sur 20 -->
            <Badge
              v-if="!isFinished"
              variant="pill"
              class="bg-white/10 text-white font-bold text-[10px] px-2.5 py-0.5 border border-white/15"
            >
              {{ isReviewing ? 'Revue finale' : `Question ${currentIndex + 1} / ${totalQuestions || 20}` }}
            </Badge>
          </div>

          <!-- Catégorie en cours & Bouton Revue -->
          <div v-if="!isFinished" class="flex items-center justify-between text-[11px]">
            <span v-if="!isReviewing && currentCategory" class="text-white/70 font-medium truncate max-w-[200px]">
              {{ currentCategory }}
            </span>
            <span v-else-if="isReviewing" class="text-white/70 font-medium">
              {{ answeredCount }}/{{ totalQuestions || 20 }} répondues
            </span>

            <button
              v-if="!isReviewing"
              type="button"
              class="text-[11px] text-[var(--theme-accent-primary,#1a6fff)] font-semibold hover:underline flex items-center gap-1 cursor-pointer"
              @click="goToReview"
            >
              <Eye class="w-3 h-3" />
              <span>Revoir avant validation</span>
            </button>
          </div>
        </header>

        <div class="flex-1 min-h-0 flex flex-col">
          <QuizViewer />
        </div>
      </div>
    </main>
  </div>
</template>
