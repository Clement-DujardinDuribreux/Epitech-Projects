import { ref, shallowRef, computed, onMounted } from 'vue'
import {
  RecruiterService,
  type ISendContactInput,
  type IRecruiterTrackingStats,
} from '@/core/services/RecruiterService'
import type {
  RecruiterContactEntity,
  RecruiterContactStatus,
} from '@/core/domain/entities/RecruiterContact.entity'
import { useAuth } from '@/composables/useAuth'

export function useRecruiterTracking(service: RecruiterService = new RecruiterService()) {
  const auth = useAuth()
  const favorites = ref<string[]>([])
  const contacts = shallowRef<RecruiterContactEntity[]>([])
  const statusFilter = ref<RecruiterContactStatus | 'ALL'>('ALL')
  const searchQuery = ref<string>('')
  const isLoading = ref<boolean>(false)

  async function loadAll(): Promise<void> {
    if (!auth.isAuthenticated.value) return
    isLoading.value = true
    try {
      const [favList, contactList] = await Promise.all([
        service.getFavorites(),
        service.getContacts(),
      ])
      favorites.value = favList
      contacts.value = contactList
    } catch {
      // Erreur de chargement
    } finally {
      isLoading.value = false
    }
  }

  function isFavorite(candidateId: string): boolean {
    return favorites.value.includes(candidateId)
  }

  async function toggleFavorite(candidateId: string): Promise<boolean> {
    try {
      const isNowFav = await service.toggleFavorite(candidateId)
      if (isNowFav) {
        if (!favorites.value.includes(candidateId)) {
          favorites.value = [...favorites.value, candidateId]
        }
      } else {
        favorites.value = favorites.value.filter((id) => id !== candidateId)
      }
      return isNowFav
    } catch {
      return false
    }
  }

  async function sendContact(input: ISendContactInput): Promise<RecruiterContactEntity> {
    const created = await service.sendContact(input)
    contacts.value = [created, ...contacts.value]
    return created
  }

  async function updateContactStatus(
    contactId: string,
    status: RecruiterContactStatus,
  ): Promise<void> {
    try {
      const updated = await service.updateContactStatus(contactId, status)
      contacts.value = contacts.value.map((c) => (c.id === updated.id ? updated : c))
    } catch {
      // Erreur de mise à jour
    }
  }

  async function deleteContact(contactId: string): Promise<void> {
    try {
      await service.deleteContact(contactId)
      contacts.value = contacts.value.filter((c) => c.id !== contactId)
    } catch {
      // Erreur de suppression
    }
  }

  const filteredContacts = computed<RecruiterContactEntity[]>(() => {
    let list = contacts.value

    if (statusFilter.value !== 'ALL') {
      list = list.filter((c) => c.status === statusFilter.value)
    }

    if (searchQuery.value.trim()) {
      const q = searchQuery.value.toLowerCase().trim()
      list = list.filter(
        (c) =>
          c.candidateName.toLowerCase().includes(q) ||
          c.candidateHeadline.toLowerCase().includes(q) ||
          c.message.toLowerCase().includes(q) ||
          (c.location && c.location.toLowerCase().includes(q)),
      )
    }

    return list
  })

  const stats = computed<IRecruiterTrackingStats>(() => {
    let envoyes = 0
    let enAttente = 0
    let entretiens = 0

    for (const c of contacts.value) {
      if (c.status === 'Prise de contact envoyée') envoyes++
      else if (c.status === 'En attente de réponse') enAttente++
      else if (c.status === 'Entretien planifié') entretiens++
    }

    return {
      total: contacts.value.length,
      envoyes,
      enAttente,
      entretiens,
    }
  })

  onMounted(() => {
    loadAll()
  })

  return {
    favorites,
    contacts,
    statusFilter,
    searchQuery,
    isLoading,
    filteredContacts,
    stats,
    isFavorite,
    toggleFavorite,
    sendContact,
    updateContactStatus,
    deleteContact,
    loadAll,
  }
}
