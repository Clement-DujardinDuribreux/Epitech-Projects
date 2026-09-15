<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import {
  ArrowLeft,
  Send,
  Check,
  ShieldCheck,
  MapPin,
  Sparkles,
  Building,
} from 'lucide-vue-next'
import { CandidateFeedService } from '@/core/services/CandidateFeedService'
import { CandidateHttpRepository } from '@/core/infrastructure/repositories/CandidateHttpRepository'
import type { CandidateProfileEntity } from '@/core/domain/entities/CandidateProfile.entity'
import { useRecruiterTracking } from '@/composables/useRecruiterTracking'
import { useDevicePreview } from '@/composables/useDevicePreview'
import DevicePreviewBar from '@/components/common/DevicePreviewBar.vue'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'

const route = useRoute()
const router = useRouter()
const { currentFormat } = useDevicePreview()
const candidateFeedService = new CandidateFeedService(new CandidateHttpRepository())
const { sendContact } = useRecruiterTracking()

const candidatId = (route.params.candidatId as string) || 'lea-d'
const candidate = ref<CandidateProfileEntity | null>(null)
const isLoading = ref(true)
const error = ref<string | null>(null)

const message = ref('')
const selectedTemplate = ref<'intro' | 'interview' | 'offer'>('intro')
const isSubmitted = ref(false)

const templates = [
  {
    id: 'intro' as const,
    label: 'Prise de contact directe',
    text: (name: string) =>
      `Bonjour ${name}, votre profil et votre pitch vidéo sur ProfilsActifs ont retenu toute notre attention. Vos compétences correspondent à nos projets en développement. Seriez-vous ouvert(e) à un premier échange d'information ?`,
  },
  {
    id: 'interview' as const,
    label: 'Proposition d\'entretien',
    text: (name: string) =>
      `Bonjour ${name}, suite à l'analyse de votre parcours et de votre certification, nous aimerions vous proposer un entretien de recrutement cette semaine. Quelles seraient vos disponibilités en visioconférence ?`,
  },
  {
    id: 'offer' as const,
    label: 'Opportunité de poste',
    text: (name: string) =>
      `Bonjour ${name}, notre entreprise recrute actuellement et votre profil nous semble en parfaite adéquation avec le poste. Nous serions ravis de vous présenter notre équipe et nos conditions d'embauche.`,
  },
]

function applyTemplate(tplId: 'intro' | 'interview' | 'offer') {
  selectedTemplate.value = tplId
  if (candidate.value) {
    const found = templates.find((t) => t.id === tplId)
    if (found) {
      message.value = found.text(candidate.value.fullName)
    }
  }
}

async function loadCandidate() {
  isLoading.value = true
  error.value = null
  try {
    const found = await candidateFeedService.fetchCandidateById(candidatId)
    if (!found) {
      throw new Error(`Candidat #${candidatId} introuvable`)
    }
    candidate.value = found
    applyTemplate('intro')
  } catch (err) {
    error.value = err instanceof Error ? err.message : 'Erreur lors du chargement du profil'
  } finally {
    isLoading.value = false
  }
}

async function handleSubmit() {
  if (!candidate.value || !message.value.trim()) return

  await sendContact({
    candidateId: candidate.value.id,
    candidateName: candidate.value.fullName,
    candidateHeadline: candidate.value.headline,
    candidateAvatarUrl: candidate.value.media.avatarUrl,
    location: candidate.value.location,
    isJebCertified: candidate.value.isJebCertified,
    message: message.value.trim(),
    status: 'Prise de contact envoyée',
  })

  isSubmitted.value = true
}

onMounted(() => {
  loadCandidate()
})
</script>

<template>
  <div class="w-full font-body relative pb-16">
    <DevicePreviewBar :active-format="currentFormat" @update:format="(f) => (currentFormat = f)" />

    <div
      class="w-full mx-auto px-3 sm:px-6 py-6 space-y-6 transition-all duration-300"
      :class="{
        'max-w-[390px]': currentFormat === 'mobile',
        'max-w-[768px]': currentFormat === 'tablet',
        'max-w-3xl': currentFormat === 'desktop',
      }"
    >
      <!-- Bouton Retour au tableau de bord -->
      <div class="flex items-center justify-between">
        <button
          type="button"
          class="flex items-center gap-2 text-xs font-heading font-semibold text-muted-foreground hover:text-foreground cursor-pointer transition-colors"
          @click="router.push('/recruteur/dashboard')"
        >
          <ArrowLeft class="w-4 h-4" />
          <span>Retour à l'Espace Recruteur</span>
        </button>

        <span class="text-xs font-mono text-muted-foreground">
          Candidat #{{ candidatId }}
        </span>
      </div>

      <!-- État de chargement -->
      <div v-if="isLoading" class="py-16 text-center text-muted-foreground font-heading text-sm">
        <span class="w-8 h-8 border-3 border-border border-t-[#000091] rounded-full animate-spin inline-block mb-3"></span>
        <p>Chargement du profil candidat...</p>
      </div>

      <!-- État d'erreur -->
      <div v-else-if="error" class="p-6 rounded-2xl bg-destructive/10 border border-destructive/30 text-center space-y-3">
        <p class="font-bold text-destructive font-heading">{{ error }}</p>
        <Button variant="default" size="sm" @click="router.push('/recruteur/dashboard')">
          Retour au catalogue
        </Button>
      </div>

      <!-- Formulaire ou Écran de confirmation -->
      <div v-else-if="candidate" class="space-y-6">
        <!-- Récapitulatif du candidat ciblé -->
        <section class="p-4 sm:p-5 rounded-2xl bg-card border border-border shadow-xs flex flex-col sm:flex-row items-start sm:items-center justify-between gap-4">
          <div class="flex items-center gap-3.5">
            <div class="w-14 h-14 rounded-2xl overflow-hidden bg-black shrink-0 border border-border">
              <img
                :src="candidate.media.posterUrl"
                :alt="`Photo de ${candidate.fullName}`"
                class="w-full h-full object-cover"
              />
            </div>
            <div class="space-y-1">
              <div class="flex items-center gap-2 flex-wrap">
                <h1 class="text-base sm:text-lg font-bold font-heading text-foreground">
                  {{ candidate.fullName }}
                </h1>
                <Badge v-if="candidate.isJebCertified" variant="jeb" class="text-[10px] gap-1 px-2 py-0.5">
                  <ShieldCheck class="w-3 h-3 text-[#f5c842]" />
                  <span>Certifié JEB</span>
                </Badge>
              </div>
              <p class="text-xs text-muted-foreground font-body">{{ candidate.headline }}</p>
              <p class="text-[11px] text-muted-foreground flex items-center gap-1 font-heading">
                <MapPin class="w-3 h-3" />
                <span>{{ candidate.location }}</span>
              </p>
            </div>
          </div>

          <RouterLink :to="`/profils/${candidate.id}`">
            <Button variant="outline" size="sm" class="text-xs font-heading shrink-0">
              Voir la fiche complète
            </Button>
          </RouterLink>
        </section>

        <!-- Confirmation après envoi -->
        <section
          v-if="isSubmitted"
          class="p-8 rounded-2xl bg-card border border-emerald-500/30 text-center space-y-4 shadow-sm"
        >
          <div class="w-14 h-14 rounded-full bg-emerald-500/15 text-emerald-400 border border-emerald-500/30 flex items-center justify-center mx-auto">
            <Check class="w-8 h-8" />
          </div>
          <h2 class="text-xl font-bold font-heading text-foreground">
            Sollicitation transmise à {{ candidate.fullName }} !
          </h2>
          <p class="text-xs sm:text-sm text-muted-foreground font-body max-w-md mx-auto">
            Votre proposition d'échange a bien été envoyée. Elle apparaît dès maintenant avec le statut
            <strong class="text-foreground">« Prise de contact envoyée »</strong> dans votre tableau de suivi.
          </p>

          <div class="pt-4 flex flex-col sm:flex-row items-center justify-center gap-3">
            <Button
              variant="default"
              size="default"
              class="font-bold font-heading text-xs sm:text-sm cursor-pointer"
              @click="router.push({ path: '/recruteur/dashboard', query: { tab: 'suivi' } })"
            >
              Consulter le suivi des sollicitations
            </Button>

            <Button
              variant="outline"
              size="default"
              class="font-heading text-xs sm:text-sm cursor-pointer"
              @click="router.push('/recruteur/dashboard')"
            >
              Retourner au catalogue
            </Button>
          </div>
        </section>

        <!-- Formulaire de message professionnel -->
        <section
          v-else
          class="p-5 sm:p-7 rounded-2xl bg-card border border-border shadow-xs space-y-5"
        >
          <div>
            <h2 class="text-base sm:text-lg font-bold font-heading text-foreground">
              Rédiger votre proposition
            </h2>
            <p class="text-xs text-muted-foreground mt-0.5 font-body">
              Personnalisez votre message pour maximiser vos chances d'obtenir une réponse rapide.
            </p>
          </div>

          <!-- Modèles rapides -->
          <div class="space-y-2">
            <label class="text-[11px] font-bold font-heading text-muted-foreground uppercase tracking-wider flex items-center gap-1.5">
              <Sparkles class="w-3.5 h-3.5 text-amber-400" />
              <span>Modèles de messages disponibles :</span>
            </label>
            <div class="flex items-center gap-2 overflow-x-auto no-scrollbar pb-1">
              <button
                v-for="tpl in templates"
                :key="tpl.id"
                type="button"
                class="text-xs font-heading px-3 py-1.5 rounded-xl border transition-all cursor-pointer whitespace-nowrap font-medium"
                :class="
                  selectedTemplate === tpl.id
                    ? 'bg-[#000091] text-white border-[#000091] font-bold shadow-xs'
                    : 'bg-muted/40 border-border text-foreground hover:bg-muted'
                "
                @click="applyTemplate(tpl.id)"
              >
                {{ tpl.label }}
              </button>
            </div>
          </div>

          <!-- Zone de saisie -->
          <div class="space-y-2">
            <label for="full-contact-msg" class="text-xs font-bold font-heading text-foreground">
              Votre message à destination de {{ candidate.fullName }} <span class="text-red-500">*</span>
            </label>
            <textarea
              id="full-contact-msg"
              v-model="message"
              rows="6"
              class="w-full bg-muted/40 border border-input rounded-xl p-3.5 text-xs sm:text-sm text-foreground focus:outline-none focus-visible:ring-2 focus-visible:ring-[#000091] font-sans leading-relaxed"
              placeholder="Exprimez vos intentions de recrutement et proposez un créneau d'échange..."
            ></textarea>
            <div class="flex items-center justify-between text-[11px] text-muted-foreground font-mono">
              <span>Caractères : {{ message.length }}</span>
              <span>Statut initial : « Prise de contact envoyée »</span>
            </div>
          </div>

          <!-- Rappel de conformité RGPD & Coordonnées -->
          <div class="p-3.5 rounded-xl bg-muted/30 border border-border/50 text-[11px] text-muted-foreground flex items-center gap-2.5">
            <Building class="w-4 h-4 text-[#000091] shrink-0" />
            <span>
              Vos coordonnées d'entreprise vérifiées seront automatiquement partagées avec ce demandeur d'emploi conformément à la politique RGPD de la plateforme.
            </span>
          </div>

          <!-- Bouton de soumission -->
          <div class="flex items-center justify-end gap-3 pt-2">
            <Button
              variant="outline"
              size="default"
              class="text-xs font-heading cursor-pointer"
              @click="router.push('/recruteur/dashboard')"
            >
              Annuler
            </Button>

            <Button
              variant="secondaryCta"
              size="default"
              class="gap-2 font-bold font-heading text-xs sm:text-sm cursor-pointer"
              :disabled="!message.trim()"
              @click="handleSubmit"
            >
              <Send class="w-4 h-4" />
              <span>Envoyer la proposition directe</span>
            </Button>
          </div>
        </section>
      </div>
    </div>
  </div>
</template>
