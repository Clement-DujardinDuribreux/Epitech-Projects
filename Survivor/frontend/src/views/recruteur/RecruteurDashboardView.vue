<script setup lang="ts">
import { ref, shallowRef, computed, onMounted, watch } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import {
  Search,
  Filter,
  ShieldCheck,
  ChevronLeft,
  ChevronRight,
  RefreshCw,
  Bookmark,
} from 'lucide-vue-next'
import type { CandidateProfileEntity } from '@/core/domain/entities/CandidateProfile.entity'
import type { RecruiterContactStatus } from '@/core/domain/entities/RecruiterContact.entity'
import { CandidateFeedService } from '@/core/services/CandidateFeedService'
import { CandidateHttpRepository } from '@/core/infrastructure/repositories/CandidateHttpRepository'
import { RecruiterService } from '@/core/services/RecruiterService'
import { RecruiterHttpRepository } from '@/core/infrastructure/repositories/RecruiterHttpRepository'
import { useDevicePreview } from '@/composables/useDevicePreview'
import { useRecruiterTracking } from '@/composables/useRecruiterTracking'
import DevicePreviewBar from '@/components/common/DevicePreviewBar.vue'
import { Button } from '@/components/ui/button'

import RecruiterHeader, { type RecruiterTab } from './components/RecruiterHeader.vue'
import RecruiterCandidateCard from './components/RecruiterCandidateCard.vue'
import RecruiterTrackingView from './components/RecruiterTrackingView.vue'
import RecruiterContactModal, { type IContactModalTarget } from './components/RecruiterContactModal.vue'

const route = useRoute()
const router = useRouter()
const { currentFormat } = useDevicePreview()
const isMobileMode = computed(() => currentFormat.value === 'mobile')

const service = new CandidateFeedService(new CandidateHttpRepository())
const {
  favorites,
  filteredContacts,
  stats,
  searchQuery: trackingSearchQuery,
  statusFilter: trackingStatusFilter,
  isFavorite,
  toggleFavorite: trackingToggleFavorite,
  sendContact,
  updateContactStatus,
  deleteContact,
} = useRecruiterTracking(new RecruiterService(new RecruiterHttpRepository()))

// Onglet actif : 'catalogue' | 'favoris' | 'suivi'
const activeTab = ref<RecruiterTab>('catalogue')

// État Catalogue
const candidates = shallowRef<CandidateProfileEntity[]>([])
const searchQuery = ref('')
const selectedSkill = ref('')
const jebOnly = ref(false)
const currentPage = ref(1)
const totalItems = ref(0)
const totalPages = ref(1)
const isLoading = ref(false)
const error = ref<string | null>(null)

// État Favoris
const favoriteCandidates = shallowRef<CandidateProfileEntity[]>([])
const favSearchQuery = ref('')
const favSelectedSkill = ref('')
const isFavLoading = ref(false)

// Modale de contact
const isContactOpen = ref(false)
const selectedCandidateTarget = ref<IContactModalTarget | null>(null)

const skillCategories = [
  'Tous les secteurs',
  'Tech & Numérique',
  'Commerce & Vente',
  'Santé & Soins',
  'Artisanat & BTP',
  'Design & Création',
  'Finance & Gestion',
  'Hôtellerie & Restauration',
  'Logistique & Transport',
  'Ressources Humaines',
]

// Chargement Catalogue (20 profils par page)
async function loadCatalogueData() {
  isLoading.value = true
  error.value = null
  try {
    const result = await service.fetchCandidates({
      page: currentPage.value,
      limit: 20,
      query: searchQuery.value,
      skill: selectedSkill.value === 'Tous les secteurs' ? '' : selectedSkill.value,
      jebOnly: jebOnly.value,
    })
    candidates.value = result.items
    totalItems.value = result.total
    totalPages.value = result.totalPages
  } catch (err) {
    error.value = err instanceof Error ? err.message : 'Erreur lors du chargement des profils'
  } finally {
    isLoading.value = false
  }
}

// Chargement Favoris
async function loadFavoritesData() {
  if (favorites.value.length === 0) {
    favoriteCandidates.value = []
    return
  }
  isFavLoading.value = true
  try {
    const result = await service.fetchCandidatesByIds(favorites.value, {
      limit: 100,
      query: favSearchQuery.value,
      skill: favSelectedSkill.value === 'Tous les secteurs' ? '' : favSelectedSkill.value,
    })
    favoriteCandidates.value = result.items
  } catch {
    // Mode dégradé si erreur de chargement
  } finally {
    isFavLoading.value = false
  }
}

function handleTabChange(tab: RecruiterTab) {
  activeTab.value = tab
  router.replace({ query: { ...route.query, tab } })
  if (tab === 'favoris') {
    loadFavoritesData()
  } else if (tab === 'catalogue') {
    loadCatalogueData()
  }
}

function handleSearch() {
  currentPage.value = 1
  loadCatalogueData()
}

function handleSkillSelect(skill: string) {
  selectedSkill.value = skill
  currentPage.value = 1
  loadCatalogueData()
}

function resetFilters() {
  searchQuery.value = ''
  selectedSkill.value = ''
  jebOnly.value = false
  currentPage.value = 1
  loadCatalogueData()
}

function prevPage() {
  if (currentPage.value > 1) {
    currentPage.value--
    loadCatalogueData()
    window.scrollTo({ top: 0, behavior: 'smooth' })
  }
}

function nextPage() {
  if (currentPage.value < totalPages.value) {
    currentPage.value++
    loadCatalogueData()
    window.scrollTo({ top: 0, behavior: 'smooth' })
  }
}

async function handleToggleFavorite(candidateId: string) {
  await trackingToggleFavorite(candidateId)
  if (activeTab.value === 'favoris') {
    favoriteCandidates.value = favoriteCandidates.value.filter((c) => c.id !== candidateId)
  }
}

function openContactModal(candidate: CandidateProfileEntity | { id: string; fullName: string; headline?: string; avatarUrl?: string; location?: string; isJebCertified?: boolean }) {
  selectedCandidateTarget.value = {
    id: candidate.id,
    fullName: candidate.fullName,
    headline: candidate.headline,
    avatarUrl: 'media' in candidate ? candidate.media.avatarUrl : candidate.avatarUrl,
    location: candidate.location,
    isJebCertified: candidate.isJebCertified,
  }
  isContactOpen.value = true
}

async function handleSendContactSubmit(payload: {
  candidateId: string
  candidateName: string
  candidateHeadline: string
  candidateAvatarUrl: string
  location?: string
  isJebCertified?: boolean
  message: string
  status?: RecruiterContactStatus
}) {
  await sendContact(payload)
  try {
    await service.contactCandidate(payload.candidateId)
  } catch {
    // Erreur silencieuse
  }
}

function handleRecontact(candidateId: string, candidateName: string) {
  selectedCandidateTarget.value = {
    id: candidateId,
    fullName: candidateName,
  }
  isContactOpen.value = true
}

onMounted(() => {
  const queryTab = route.query.tab as RecruiterTab
  if (queryTab && ['catalogue', 'favoris', 'suivi'].includes(queryTab)) {
    activeTab.value = queryTab
  }
  loadCatalogueData()
  loadFavoritesData()
})

watch(
  () => route.query.tab,
  (newTab) => {
    const validTab =
      newTab && ['catalogue', 'favoris', 'suivi'].includes(newTab as RecruiterTab)
        ? (newTab as RecruiterTab)
        : 'catalogue'
    activeTab.value = validTab
    if (validTab === 'favoris') loadFavoritesData()
  },
)

watch(jebOnly, () => {
  currentPage.value = 1
  loadCatalogueData()
})
</script>

<template>
  <div class="w-full font-body relative pb-16">
    <!-- Barre de prévisualisation responsive -->
    <DevicePreviewBar :active-format="currentFormat" @update:format="(f) => (currentFormat = f)" />

    <div
      class="w-full mx-auto px-2 sm:px-4 py-4 space-y-6 transition-all duration-300"
      :class="{
        'max-w-[390px]': currentFormat === 'mobile',
        'max-w-[768px]': currentFormat === 'tablet',
        'max-w-7xl': currentFormat === 'desktop',
      }"
    >
      <!-- En-tête Recruteur avec onglets synchronisés -->
      <RecruiterHeader
        :active-tab="activeTab"
        :favorites-count="favorites.length"
        :contacts-count="stats.total"
        :total-candidates="totalItems"
        :is-mobile-mode="isMobileMode"
        @update:active-tab="handleTabChange"
      />

      <!-- ======================================================== -->
      <!-- ONGLET 1 : CATALOGUE DES PROFILS CANDIDATS               -->
      <!-- ======================================================== -->
      <div v-if="activeTab === 'catalogue'" class="space-y-6">
        <!-- Barre de recherche & Filtres -->
        <section
          class="p-3.5 sm:p-5 rounded-2xl bg-card border border-border shadow-xs space-y-3.5"
          aria-label="Filtres de recherche du catalogue"
        >
          <div class="flex flex-col sm:flex-row gap-2.5">
            <div class="relative flex-1">
              <Search class="absolute left-3.5 top-1/2 -translate-y-1/2 w-4 h-4 text-muted-foreground" />
              <input
                v-model="searchQuery"
                type="search"
                placeholder="Métier, compétence, ville (ex: Vente, React, Lyon...)"
                class="w-full bg-muted/40 border border-input rounded-xl pl-10 pr-4 py-2 sm:py-2.5 text-xs sm:text-sm text-foreground focus-visible:ring-2 focus-visible:ring-[#000091] focus:outline-none font-sans"
                @keydown.enter="handleSearch"
              />
            </div>

            <Button
              variant="default"
              size="default"
              class="gap-2 font-bold font-heading h-9 sm:h-10 px-5 shrink-0 text-xs sm:text-sm cursor-pointer"
              @click="handleSearch"
            >
              <Search class="w-4 h-4" />
              <span>Rechercher</span>
            </Button>
          </div>

          <!-- Filtres par catégorie -->
          <div class="space-y-2 pt-2 border-t border-border/50">
            <div class="flex items-center justify-between gap-2">
              <span class="text-[11px] font-bold font-heading text-muted-foreground uppercase tracking-wider">
                Secteurs d'activité :
              </span>
              <label class="flex items-center gap-1.5 text-xs font-semibold font-heading text-foreground cursor-pointer select-none">
                <input
                  v-model="jebOnly"
                  type="checkbox"
                  class="w-3.5 h-3.5 rounded border-input text-[#000091] focus-visible:ring-2 focus-visible:ring-[#000091]"
                />
                <span class="flex items-center gap-1 text-[11px] sm:text-xs">
                  <ShieldCheck class="w-3.5 h-3.5 text-[#f5c842]" />
                  <span>Certification JEB seulement</span>
                </span>
              </label>
            </div>

            <!-- Chips défilables -->
            <div
              class="flex items-center gap-1.5 overflow-x-auto pb-1.5 pt-0.5 no-scrollbar flex-nowrap sm:flex-wrap"
              role="group"
              aria-label="Secteurs d'activité"
            >
              <button
                v-for="skill in skillCategories"
                :key="skill"
                type="button"
                class="text-[11px] sm:text-xs px-3 py-1 sm:py-1.5 rounded-full border transition-all cursor-pointer font-medium font-heading shrink-0 whitespace-nowrap"
                :class="
                  selectedSkill === skill || (!selectedSkill && skill === 'Tous les secteurs')
                    ? 'bg-[#000091] text-white border-[#000091] shadow-xs'
                    : 'bg-muted/40 border-border text-foreground hover:bg-muted'
                "
                @click="handleSkillSelect(skill)"
              >
                {{ skill }}
              </button>
            </div>
          </div>
        </section>

        <!-- Résumé & Réinitialisation -->
        <div class="flex items-center justify-between text-xs text-muted-foreground font-heading px-1">
          <p>
            <strong class="text-foreground">{{ totalItems }}</strong> profil{{ totalItems > 1 ? 's' : '' }} disponible{{ totalItems > 1 ? 's' : '' }}
            <span v-if="totalPages > 1">· Page {{ currentPage }} / {{ totalPages }} (20 par page)</span>
          </p>

          <button
            v-if="searchQuery || selectedSkill || jebOnly"
            type="button"
            class="flex items-center gap-1 text-[#D97706] hover:underline font-bold cursor-pointer text-xs"
            @click="resetFilters"
          >
            <RefreshCw class="w-3.5 h-3.5" />
            <span>Réinitialiser</span>
          </button>
        </div>

        <!-- Chargement -->
        <div v-if="isLoading" class="py-16 flex flex-col items-center justify-center text-muted-foreground" role="status">
          <span class="w-8 h-8 border-3 border-border border-t-[#000091] rounded-full animate-spin mb-3"></span>
          <p class="text-sm font-heading">Chargement des profils candidats...</p>
        </div>

        <!-- Erreur -->
        <div v-else-if="error" class="py-12 p-6 rounded-2xl bg-destructive/10 border border-destructive/30 text-center space-y-3" role="alert">
          <p class="font-bold text-destructive font-heading">Erreur lors de la récupération des données</p>
          <p class="text-xs text-muted-foreground font-body">{{ error }}</p>
          <Button variant="default" size="sm" @click="loadCatalogueData">
            Réessayer
          </Button>
        </div>

        <!-- État vide -->
        <div v-else-if="candidates.length === 0" class="py-14 text-center space-y-3 bg-card border border-border rounded-2xl p-6">
          <div class="w-12 h-12 rounded-full bg-muted/60 flex items-center justify-center mx-auto text-muted-foreground">
            <Filter class="w-6 h-6" />
          </div>
          <h2 class="text-base font-bold font-heading text-foreground">
            Aucun profil ne correspond à vos critères
          </h2>
          <p class="text-xs text-muted-foreground max-w-sm mx-auto font-body">
            Essayez d'élargir votre recherche en modifiant vos filtres ou mots-clés.
          </p>
          <Button variant="default" size="sm" class="mt-2 font-heading" @click="resetFilters">
            Afficher tous les profils mis en avant
          </Button>
        </div>

        <!-- Grille responsive de 20 candidats -->
        <section
          v-else
          class="grid gap-4"
          :class="{
            'grid-cols-1': isMobileMode,
            'grid-cols-1 sm:grid-cols-2': currentFormat === 'tablet',
            'grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4': currentFormat === 'desktop',
          }"
          aria-label="Liste des profils candidats"
        >
          <RecruiterCandidateCard
            v-for="candidate in candidates"
            :key="candidate.id"
            :candidate="candidate"
            :is-favorite="isFavorite(candidate.id)"
            @toggle-favorite="handleToggleFavorite"
            @open-contact="openContactModal"
          />
        </section>

        <!-- Pagination serveur (20 candidats par page) -->
        <nav
          v-if="totalPages > 1"
          class="pt-4 sm:pt-6 border-t border-border flex items-center justify-between gap-2 font-heading"
          aria-label="Navigation dans les pages de résultats"
        >
          <Button
            variant="outline"
            size="sm"
            class="gap-1 text-xs h-9 px-3"
            :disabled="currentPage === 1"
            @click="prevPage"
          >
            <ChevronLeft class="w-4 h-4" />
            <span class="hidden sm:inline">Page précédente</span>
            <span class="sm:hidden">Préc.</span>
          </Button>

          <span class="text-xs text-muted-foreground font-heading">
            Page <strong class="text-foreground">{{ currentPage }}</strong> / <strong class="text-foreground">{{ totalPages }}</strong>
          </span>

          <Button
            variant="outline"
            size="sm"
            class="gap-1 text-xs h-9 px-3"
            :disabled="currentPage === totalPages"
            @click="nextPage"
          >
            <span class="hidden sm:inline">Page suivante</span>
            <span class="sm:hidden">Suiv.</span>
            <ChevronRight class="w-4 h-4" />
          </Button>
        </nav>
      </div>

      <!-- ======================================================== -->
      <!-- ONGLET 2 : MES FAVORIS                                   -->
      <!-- ======================================================== -->
      <div v-else-if="activeTab === 'favoris'" class="space-y-6">
        <!-- Barre de recherche dans les favoris -->
        <div class="p-3.5 sm:p-5 rounded-2xl bg-card border border-border shadow-xs flex flex-col sm:flex-row gap-2.5 items-center justify-between">
          <div class="relative flex-1 w-full">
            <Search class="absolute left-3.5 top-1/2 -translate-y-1/2 w-4 h-4 text-muted-foreground" />
            <input
              v-model="favSearchQuery"
              type="search"
              placeholder="Rechercher dans mes favoris..."
              class="w-full bg-muted/40 border border-input rounded-xl pl-10 pr-4 py-2 text-xs sm:text-sm text-foreground focus-visible:ring-2 focus-visible:ring-[#000091] focus:outline-none font-sans"
              @input="loadFavoritesData"
            />
          </div>

          <div class="flex items-center gap-2 self-start sm:self-auto shrink-0 text-xs text-muted-foreground font-heading">
            <Bookmark class="w-4 h-4 text-amber-400 fill-amber-400" />
            <span><strong class="text-foreground">{{ favorites.length }}</strong> profil{{ favorites.length > 1 ? 's' : '' }} sauvegardé{{ favorites.length > 1 ? 's' : '' }}</span>
          </div>
        </div>

        <!-- Chargement favoris -->
        <div v-if="isFavLoading" class="py-16 text-center text-muted-foreground font-heading text-sm">
          <span class="w-8 h-8 border-3 border-border border-t-[#000091] rounded-full animate-spin inline-block mb-3"></span>
          <p>Chargement de vos profils favoris...</p>
        </div>

        <!-- État vide si aucun favori -->
        <div
          v-else-if="favorites.length === 0 || favoriteCandidates.length === 0"
          class="py-16 text-center space-y-4 bg-card border border-border rounded-2xl p-8"
        >
          <div class="w-14 h-14 rounded-full bg-amber-400/15 text-amber-400 border border-amber-400/30 flex items-center justify-center mx-auto">
            <Bookmark class="w-7 h-7" />
          </div>
          <h2 class="text-lg font-bold font-heading text-foreground">
            Aucun profil dans vos favoris pour le moment
          </h2>
          <p class="text-xs sm:text-sm text-muted-foreground max-w-md mx-auto font-body">
            Ajoutez des profils à vos favoris en cliquant sur l'icône étoile ou marque-page depuis le catalogue pour les retrouver facilement ici.
          </p>
          <Button
            variant="default"
            size="default"
            class="mt-2 font-heading font-bold text-xs cursor-pointer"
            @click="handleTabChange('catalogue')"
          >
            Explorer le catalogue des candidats
          </Button>
        </div>

        <!-- Grille des favoris -->
        <section
          v-else
          class="grid gap-4"
          :class="{
            'grid-cols-1': isMobileMode,
            'grid-cols-1 sm:grid-cols-2': currentFormat === 'tablet',
            'grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4': currentFormat === 'desktop',
          }"
          aria-label="Liste de vos profils favoris"
        >
          <RecruiterCandidateCard
            v-for="candidate in favoriteCandidates"
            :key="candidate.id"
            :candidate="candidate"
            :is-favorite="true"
            @toggle-favorite="handleToggleFavorite"
            @open-contact="openContactModal"
          />
        </section>
      </div>

      <!-- ======================================================== -->
      <!-- ONGLET 3 : SUIVI DES CANDIDATS CONTACTÉS                 -->
      <!-- ======================================================== -->
      <div v-else-if="activeTab === 'suivi'" class="space-y-6">
        <RecruiterTrackingView
          :contacts="filteredContacts"
          :stats="stats"
          :search-query="trackingSearchQuery"
          :status-filter="trackingStatusFilter"
          @update:search-query="(v) => (trackingSearchQuery = v)"
          @update:status-filter="(v) => (trackingStatusFilter = v)"
          @update-status="updateContactStatus"
          @delete-contact="deleteContact"
          @recontact="handleRecontact"
        />
      </div>
    </div>

    <!-- Modale de contact réutilisable -->
    <RecruiterContactModal
      :open="isContactOpen"
      :candidate="selectedCandidateTarget"
      @update:open="(val) => (isContactOpen = val)"
      @submit="handleSendContactSubmit"
    />
  </div>
</template>
