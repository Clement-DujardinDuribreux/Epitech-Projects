import { ref, shallowRef } from 'vue'
import type { CandidateProfileEntity } from '@/core/domain/entities/CandidateProfile.entity'
import { CandidateFeedService } from '@/core/services/CandidateFeedService'
import { CandidateHttpRepository } from '@/core/infrastructure/repositories/CandidateHttpRepository'

import { InteractionNotificationService } from '@/core/services/InteractionNotificationService'

export function useCandidateProfile(
  service: CandidateFeedService = new CandidateFeedService(new CandidateHttpRepository()),
) {
  const candidate = shallowRef<CandidateProfileEntity | null>(null)
  const isLoading = ref<boolean>(false)
  const error = ref<string | null>(null)
  const contactDialogOpen = ref<boolean>(false)
  const contactSent = ref<boolean>(false)

  async function loadProfile(id: string): Promise<void> {
    isLoading.value = true
    error.value = null
    try {
      const found = await service.fetchCandidateById(id)
      if (!found) {
        throw new Error(`Profil candidat #${id} introuvable`)
      }
      candidate.value = found
    } catch (err) {
      error.value = err instanceof Error ? err.message : 'Erreur de chargement du profil'
    } finally {
      isLoading.value = false
    }
  }

  async function submitContact(): Promise<void> {
    if (!candidate.value) return
    try {
      const updated = await service.contactCandidate(candidate.value.id)
      candidate.value = updated
      contactSent.value = true

      // Exigence 2.3 : Notification du candidat lors d'une nouvelle interaction recruteur
      InteractionNotificationService.notifyRecruiterContact({
        candidateId: candidate.value.id,
        candidateName: candidate.value.fullName,
        recruiterCompany: 'Recruteur Partenaire',
        message: 'Prise de contact initiée depuis votre profil public.',
      })
    } catch (err) {
      console.error(err)
    }
  }

  function openContactModal(): void {
    contactSent.value = false
    contactDialogOpen.value = true
  }

  function closeContactModal(): void {
    contactDialogOpen.value = false
  }

  return {
    candidate,
    isLoading,
    error,
    contactDialogOpen,
    contactSent,
    loadProfile,
    submitContact,
    openContactModal,
    closeContactModal,
  }
}
