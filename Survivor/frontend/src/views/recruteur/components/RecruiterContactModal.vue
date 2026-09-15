<script setup lang="ts">
import { ref, watch } from 'vue'
import { Send, Check, Sparkles } from 'lucide-vue-next'
import { Button } from '@/components/ui/button'
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
} from '@/components/ui/dialog'
import type { RecruiterContactStatus } from '@/core/domain/entities/RecruiterContact.entity'

export interface IContactModalTarget {
  id: string
  fullName: string
  headline?: string
  avatarUrl?: string
  location?: string
  isJebCertified?: boolean
}

const props = defineProps<{
  open: boolean
  candidate: IContactModalTarget | null
}>()

const emit = defineEmits<{
  (e: 'update:open', val: boolean): void
  (
    e: 'submit',
    payload: {
      candidateId: string
      candidateName: string
      candidateHeadline: string
      candidateAvatarUrl: string
      location?: string
      isJebCertified?: boolean
      message: string
      status: RecruiterContactStatus
    },
  ): void
}>()

const message = ref('')
const selectedTemplate = ref<'custom' | 'intro' | 'interview' | 'offer'>('intro')
const isSuccess = ref(false)

const templates = [
  {
    id: 'intro' as const,
    label: 'Prise de contact',
    text: (name: string) =>
      `Bonjour ${name}, votre profil et votre présentation sur ProfilsActifs ont retenu toute notre attention. Vos compétences correspondent particulièrement à nos projets en cours. Seriez-vous disponible pour un premier échange d'information ?`,
  },
  {
    id: 'interview' as const,
    label: 'Proposition d\'entretien',
    text: (name: string) =>
      `Bonjour ${name}, suite à l'examen attentif de vos réalisations et de votre certification, nous aimerions vous inviter à un entretien de recrutement pour un poste ouvert au sein de notre équipe. Quelles seraient vos disponibilités ces prochains jours ?`,
  },
  {
    id: 'offer' as const,
    label: 'Opportunité de poste',
    text: (name: string) =>
      `Bonjour ${name}, nous ouvrons un poste qui correspond étroitement à vos compétences clés. Nous serions ravis de vous présenter notre entreprise, les conditions du poste et d'échanger sur vos aspirations professionnelles.`,
  },
]

function applyTemplate(tplId: 'intro' | 'interview' | 'offer') {
  selectedTemplate.value = tplId
  if (props.candidate) {
    const found = templates.find((t) => t.id === tplId)
    if (found) {
      message.value = found.text(props.candidate.fullName)
    }
  }
}

watch(
  () => props.open,
  (isOpen) => {
    if (isOpen && props.candidate) {
      isSuccess.value = false
      applyTemplate('intro')
    }
  },
)

function handleSend() {
  if (!props.candidate || !message.value.trim()) return

  emit('submit', {
    candidateId: props.candidate.id,
    candidateName: props.candidate.fullName,
    candidateHeadline: props.candidate.headline || 'Candidat certifié ProfilsActifs',
    candidateAvatarUrl:
      props.candidate.avatarUrl ||
      'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=200&q=80',
    location: props.candidate.location,
    isJebCertified: props.candidate.isJebCertified,
    message: message.value.trim(),
    status: 'Prise de contact envoyée',
  })

  isSuccess.value = true
  setTimeout(() => {
    emit('update:open', false)
    isSuccess.value = false
  }, 1800)
}
</script>

<template>
  <Dialog :open="open" @update:open="(val: boolean) => emit('update:open', val)">
    <DialogContent class="bg-[var(--theme-bg-card,#11111a)] border-[var(--theme-border-subtle,rgba(255,255,255,0.2))] text-white max-w-lg">
      <DialogHeader>
        <DialogTitle class="text-lg sm:text-xl font-bold font-heading">
          Solliciter {{ candidate?.fullName }}
        </DialogTitle>
        <DialogDescription class="text-white/80 text-xs font-body">
          Envoyez une proposition directe. Cette prise de contact sera automatiquement enregistrée dans votre onglet « Suivi des Sollicitations ».
        </DialogDescription>
      </DialogHeader>

      <div v-if="!isSuccess" class="space-y-3.5 py-2">
        <!-- Sélecteur de modèles de messages -->
        <div class="space-y-1.5">
          <label class="text-[11px] font-bold font-heading text-white/80 uppercase tracking-wider flex items-center gap-1">
            <Sparkles class="w-3.5 h-3.5 text-amber-300" />
            <span>Modèles de messages rapides :</span>
          </label>
          <div class="flex items-center gap-1.5 overflow-x-auto no-scrollbar pb-1">
            <button
              v-for="tpl in templates"
              :key="tpl.id"
              type="button"
              class="text-[11px] font-heading px-2.5 py-1 rounded-lg border transition-all cursor-pointer whitespace-nowrap"
              :class="
                selectedTemplate === tpl.id
                  ? 'bg-[#000091] text-white border-[#000091] font-bold shadow-xs'
                  : 'bg-white/5 border-white/10 text-white/70 hover:bg-white/10'
              "
              @click="applyTemplate(tpl.id)"
            >
              {{ tpl.label }}
            </button>
          </div>
        </div>

        <!-- Zone de texte du message -->
        <div class="space-y-1.5">
          <label for="recruiter-contact-modal-message" class="text-xs font-bold font-heading text-white">
            Votre message professionnel <span class="text-red-400">*</span>
          </label>
          <textarea
            id="recruiter-contact-modal-message"
            v-model="message"
            rows="5"
            class="w-full bg-[var(--theme-bg-surface,#1c1c28)] border border-[var(--theme-border-subtle,rgba(255,255,255,0.2))] rounded-xl p-3 text-xs sm:text-sm text-white focus:outline-none focus-visible:ring-2 focus-visible:ring-[#000091] font-sans leading-relaxed"
            placeholder="Rédigez votre proposition d'embauche ou de prise de contact..."
          ></textarea>
        </div>
      </div>

      <!-- Écran de succès animé -->
      <div v-else class="py-6 flex flex-col items-center justify-center text-center space-y-2">
        <div class="w-12 h-12 rounded-full bg-emerald-500/20 text-emerald-400 border border-emerald-500/40 flex items-center justify-center mb-1">
          <Check class="w-6 h-6" />
        </div>
        <p class="font-bold text-base text-white font-heading">Proposition envoyée avec succès !</p>
        <p class="text-xs text-white/70 font-body">
          Le candidat a été notifié et la sollicitation a été enregistrée dans votre historique de suivi.
        </p>
      </div>

      <DialogFooter v-if="!isSuccess">
        <Button
          variant="ghost"
          class="text-white/80 hover:text-white cursor-pointer font-heading text-xs"
          @click="emit('update:open', false)"
        >
          Annuler
        </Button>
        <Button
          variant="secondaryCta"
          class="gap-2 font-bold cursor-pointer font-heading text-xs"
          :disabled="!message.trim()"
          @click="handleSend"
        >
          <Send class="w-4 h-4" />
          <span>Envoyer la proposition</span>
        </Button>
      </DialogFooter>
    </DialogContent>
  </Dialog>
</template>
