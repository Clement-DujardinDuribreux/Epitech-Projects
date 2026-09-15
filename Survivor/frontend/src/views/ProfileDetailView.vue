<script setup lang="ts">
import { ref, onMounted, watch } from 'vue'
import { useRoute } from 'vue-router'
import { MessageSquare, Send, Check } from 'lucide-vue-next'
import { useCandidateProfile } from '@/composables/useCandidateProfile'
import { useDevicePreview } from '@/composables/useDevicePreview'
import ProfileHero from '@/components/profile/ProfileHero.vue'
import ProfileMetrics from '@/components/profile/ProfileMetrics.vue'
import ProfileAbout from '@/components/profile/ProfileAbout.vue'
import ProfileSkills from '@/components/profile/ProfileSkills.vue'
import DevicePreviewBar from '@/components/common/DevicePreviewBar.vue'
import { Button } from '@/components/ui/button'
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
} from '@/components/ui/dialog'

const route = useRoute()
const { currentFormat } = useDevicePreview()
const {
  candidate,
  isLoading,
  error,
  contactDialogOpen,
  contactSent,
  loadProfile,
  submitContact,
  openContactModal,
  closeContactModal,
} = useCandidateProfile()

const contactMessage = ref<string>('')

function handleOpenContact() {
  contactMessage.value = `Bonjour ${candidate.value?.fullName}, nous aimerions échanger avec vous suite à votre profil sur ProfilsActifs...`
  openContactModal()
}

onMounted(() => {
  const candidateId = (route.params.id as string) || 'lea-d'
  loadProfile(candidateId)
})

watch(
  () => route.params.id,
  (newId) => {
    if (newId) loadProfile(newId as string)
  },
)
</script>

<template>
  <div class="flex-1 w-full text-[var(--theme-text-primary,#ffffff)] flex flex-col justify-between select-none font-body pb-8">
    <DevicePreviewBar :active-format="currentFormat" @update:format="(f) => (currentFormat = f)" />

    <main
      class="w-full flex-1 flex flex-col items-center justify-center p-2 sm:p-4 transition-all duration-300 mx-auto"
      :class="{
        'max-w-[390px]': currentFormat === 'mobile',
        'max-w-[768px]': currentFormat === 'tablet',
        'max-w-2xl': currentFormat === 'desktop',
      }"
    >
      <div v-if="candidate" class="w-full bg-[var(--theme-bg-card,#0e0e16)] min-h-screen shadow-2xl border-x border-[var(--theme-border-subtle,rgba(255,255,255,0.15))] flex flex-col justify-between rounded-2xl overflow-hidden mt-2">
        <div class="space-y-2">
          <ProfileHero :candidate="candidate" />

          <ProfileMetrics :candidate="candidate" />

          <ProfileAbout :bio="candidate.bio" />

          <ProfileSkills :skills="candidate.skills" />

          <div class="p-5 flex items-center gap-3">
            <Button
              variant="secondaryCta"
              size="lg"
              class="flex-1 h-12 rounded-xl font-bold gap-2 text-sm uppercase tracking-wider shadow-lg transition-transform active:scale-95 cursor-pointer font-heading"
              @click="handleOpenContact"
              aria-label="Prendre contact avec ce candidat"
            >
              <MessageSquare class="w-4 h-4" />
              <span>Contacter</span>
            </Button>
          </div>
        </div>
      </div>

      <div v-else-if="isLoading" class="flex-1 flex flex-col items-center justify-center p-12 text-white/80" role="status">
        <span class="w-8 h-8 border-3 border-white/20 border-t-[#000091] rounded-full animate-spin mb-3"></span>
        <p class="text-sm font-heading">Chargement du profil candidat...</p>
      </div>

      <div v-else class="flex-1 flex flex-col items-center justify-center p-12 text-center text-white/80" role="alert">
        <p class="font-bold text-red-400 font-heading mb-2">{{ error || 'Profil introuvable' }}</p>
        <RouterLink to="/recruteur/dashboard">
          <Button variant="default" size="sm" class="mt-4 font-heading">
            Retourner au catalogue recruteur
          </Button>
        </RouterLink> -->
      </div>
    </main>

    <!-- Modale de contact direct accessible -->
    <Dialog :open="contactDialogOpen" @update:open="(val: boolean) => (contactDialogOpen = val)">
      <DialogContent class="bg-[var(--theme-bg-card,#11111a)] border-[var(--theme-border-subtle,rgba(255,255,255,0.2))] text-white max-w-md">
        <DialogHeader>
          <DialogTitle class="text-xl font-bold font-heading">
            Solliciter {{ candidate?.fullName }}
          </DialogTitle>
          <DialogDescription class="text-white/80 text-xs font-body">
            Envoyez un message direct et vos coordonnées d'entreprise à ce candidat.
          </DialogDescription>
        </DialogHeader>

        <div v-if="!contactSent" class="space-y-2 py-2">
          <label for="profile-contact-msg" class="text-xs font-bold font-heading text-white">
            Votre message professionnel <span class="text-red-400">*</span>
          </label>
          <textarea
            id="profile-contact-msg"
            v-model="contactMessage"
            rows="4"
            class="w-full bg-[var(--theme-bg-surface,#1c1c28)] border border-[var(--theme-border-subtle,rgba(255,255,255,0.2))] rounded-xl p-3 text-sm text-white focus:outline-none focus-visible:ring-2 focus-visible:ring-[#000091] font-sans leading-relaxed"
            placeholder="Écrivez votre message..."
          ></textarea>
        </div>

        <div v-else class="py-6 flex flex-col items-center justify-center text-center space-y-2">
          <div class="w-12 h-12 rounded-full bg-emerald-500/20 text-emerald-400 border border-emerald-500/40 flex items-center justify-center mb-1">
            <Check class="w-6 h-6" />
          </div>
          <p class="font-bold text-base text-white font-heading">Demande de contact envoyée !</p>
          <p class="text-xs text-white/70 font-body">Le candidat prendra contact avec votre établissement.</p>
        </div>

        <DialogFooter v-if="!contactSent">
          <Button variant="ghost" class="text-white/80 hover:text-white cursor-pointer font-heading" @click="closeContactModal">
            Annuler
          </Button>
          <Button variant="secondaryCta" class="gap-2 font-bold cursor-pointer font-heading" @click="submitContact">
            <Send class="w-4 h-4" />
            <span>Envoyer</span>
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  </div>
</template>
