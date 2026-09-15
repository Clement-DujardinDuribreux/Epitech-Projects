<script setup lang="ts">
import { ref, computed } from 'vue'
import {
  Search,
  Send,
  Clock,
  CalendarCheck,
  Building,
  MapPin,
  ShieldCheck,
  Eye,
  Trash2,
  Filter,
  CheckCircle2,
  ChevronDown,
  MessageSquare,
} from 'lucide-vue-next'
import type {
  RecruiterContactEntity,
  RecruiterContactStatus,
} from '@/core/domain/entities/RecruiterContact.entity'
import type { IRecruiterTrackingStats } from '@/core/services/RecruiterService'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'

const props = defineProps<{
  contacts: RecruiterContactEntity[]
  stats: IRecruiterTrackingStats
  searchQuery: string
  statusFilter: RecruiterContactStatus | 'ALL'
}>()

const emit = defineEmits<{
  (e: 'update:searchQuery', val: string): void
  (e: 'update:statusFilter', val: RecruiterContactStatus | 'ALL'): void
  (e: 'updateStatus', contactId: string, status: RecruiterContactStatus): void
  (e: 'deleteContact', contactId: string): void
  (e: 'recontact', candidateId: string, candidateName: string): void
}>()

const statusOptions: Array<{ id: RecruiterContactStatus | 'ALL'; label: string; count: number }> =
  computed<Array<{ id: RecruiterContactStatus | 'ALL'; label: string; count: number }>>(() => [
    { id: 'ALL', label: 'Toutes les sollicitations', count: props.stats.total },
    { id: 'Prise de contact envoyée', label: 'Prise de contact envoyée', count: props.stats.envoyes },
    { id: 'En attente de réponse', label: 'En attente de réponse', count: props.stats.enAttente },
    { id: 'Entretien planifié', label: 'Entretien planifié', count: props.stats.entretiens },
  ]).value

const activeMenuContactId = ref<string | null>(null)

function toggleStatusMenu(contactId: string) {
  activeMenuContactId.value = activeMenuContactId.value === contactId ? null : contactId
}

function handleSelectStatus(contactId: string, newStatus: RecruiterContactStatus) {
  emit('updateStatus', contactId, newStatus)
  activeMenuContactId.value = null
}

function formatDate(iso: string): string {
  try {
    const d = new Date(iso)
    return d.toLocaleDateString('fr-FR', {
      day: 'numeric',
      month: 'short',
      year: 'numeric',
      hour: '2-digit',
      minute: '2-digit',
    })
  } catch {
    return iso
  }
}

function getStatusBadgeClass(status: RecruiterContactStatus): string {
  switch (status) {
    case 'Prise de contact envoyée':
      return 'bg-blue-500/15 text-blue-400 border-blue-500/30'
    case 'En attente de réponse':
      return 'bg-amber-500/15 text-amber-300 border-amber-500/30'
    case 'Entretien planifié':
      return 'bg-emerald-500/15 text-emerald-300 border-emerald-500/30'
    default:
      return 'bg-muted text-muted-foreground'
  }
}

const allStatuses: RecruiterContactStatus[] = [
  'Prise de contact envoyée',
  'En attente de réponse',
  'Entretien planifié',
]
</script>

<template>
  <section class="space-y-6" aria-label="Suivi des candidats contactés">
    <!-- Cartes KPI de synthèse -->
    <div class="grid grid-cols-2 lg:grid-cols-4 gap-3 sm:gap-4">
      <!-- Total contactés -->
      <div
        class="p-4 rounded-2xl bg-card border border-border shadow-xs flex items-center gap-3 cursor-pointer hover:border-[#000091]/40 transition-colors"
        :class="{ 'ring-2 ring-[#000091]': statusFilter === 'ALL' }"
        @click="emit('update:statusFilter', 'ALL')"
      >
        <div class="w-10 h-10 rounded-xl bg-[#000091]/10 text-[#000091] flex items-center justify-center shrink-0">
          <Building class="w-5 h-5" />
        </div>
        <div>
          <p class="text-xs text-muted-foreground font-heading">Total sollicités</p>
          <p class="text-xl sm:text-2xl font-bold font-heading text-foreground">{{ stats.total }}</p>
        </div>
      </div>

      <!-- Prise de contact envoyée -->
      <div
        class="p-4 rounded-2xl bg-card border border-border shadow-xs flex items-center gap-3 cursor-pointer hover:border-blue-500/40 transition-colors"
        :class="{ 'ring-2 ring-blue-500': statusFilter === 'Prise de contact envoyée' }"
        @click="emit('update:statusFilter', 'Prise de contact envoyée')"
      >
        <div class="w-10 h-10 rounded-xl bg-blue-500/10 text-blue-400 flex items-center justify-center shrink-0">
          <Send class="w-5 h-5" />
        </div>
        <div>
          <p class="text-xs text-muted-foreground font-heading">Prise de contact</p>
          <p class="text-xl sm:text-2xl font-bold font-heading text-foreground">{{ stats.envoyes }}</p>
        </div>
      </div>

      <!-- En attente de réponse -->
      <div
        class="p-4 rounded-2xl bg-card border border-border shadow-xs flex items-center gap-3 cursor-pointer hover:border-amber-500/40 transition-colors"
        :class="{ 'ring-2 ring-amber-500': statusFilter === 'En attente de réponse' }"
        @click="emit('update:statusFilter', 'En attente de réponse')"
      >
        <div class="w-10 h-10 rounded-xl bg-amber-500/10 text-amber-300 flex items-center justify-center shrink-0">
          <Clock class="w-5 h-5" />
        </div>
        <div>
          <p class="text-xs text-muted-foreground font-heading">En attente</p>
          <p class="text-xl sm:text-2xl font-bold font-heading text-foreground">{{ stats.enAttente }}</p>
        </div>
      </div>

      <!-- Entretien planifié -->
      <div
        class="p-4 rounded-2xl bg-card border border-border shadow-xs flex items-center gap-3 cursor-pointer hover:border-emerald-500/40 transition-colors"
        :class="{ 'ring-2 ring-emerald-500': statusFilter === 'Entretien planifié' }"
        @click="emit('update:statusFilter', 'Entretien planifié')"
      >
        <div class="w-10 h-10 rounded-xl bg-emerald-500/10 text-emerald-400 flex items-center justify-center shrink-0">
          <CalendarCheck class="w-5 h-5" />
        </div>
        <div>
          <p class="text-xs text-muted-foreground font-heading">Entretiens</p>
          <p class="text-xl sm:text-2xl font-bold font-heading text-foreground">{{ stats.entretiens }}</p>
        </div>
      </div>
    </div>

    <!-- Barre d'outils : Recherche & Filtre par statut -->
    <div class="p-3.5 sm:p-5 rounded-2xl bg-card border border-border shadow-xs space-y-3.5">
      <div class="relative">
        <Search class="absolute left-3.5 top-1/2 -translate-y-1/2 w-4 h-4 text-muted-foreground" />
        <input
          :value="searchQuery"
          type="search"
          placeholder="Rechercher par nom de candidat, poste ou mot-clé dans le message..."
          class="w-full bg-muted/40 border border-input rounded-xl pl-10 pr-4 py-2 sm:py-2.5 text-xs sm:text-sm text-foreground focus-visible:ring-2 focus-visible:ring-[#000091] focus:outline-none font-sans"
          @input="emit('update:searchQuery', ($event.target as HTMLInputElement).value)"
        />
      </div>

      <!-- Filtres de statuts en chips -->
      <div class="flex items-center gap-1.5 overflow-x-auto pb-1 pt-1 no-scrollbar flex-nowrap sm:flex-wrap">
        <button
          v-for="opt in statusOptions"
          :key="opt.id"
          type="button"
          class="text-xs px-3 py-1.5 rounded-full border transition-all cursor-pointer font-heading font-semibold shrink-0 flex items-center gap-1.5"
          :class="
            statusFilter === opt.id
              ? 'bg-[#000091] text-white border-[#000091] shadow-xs'
              : 'bg-muted/40 border-border text-muted-foreground hover:text-foreground hover:bg-muted'
          "
          @click="emit('update:statusFilter', opt.id)"
        >
          <span>{{ opt.label }}</span>
          <span
            class="text-[10px] px-1.5 py-0.2 rounded-full font-mono font-bold"
            :class="statusFilter === opt.id ? 'bg-white/25 text-white' : 'bg-muted text-foreground'"
          >
            {{ opt.count }}
          </span>
        </button>
      </div>
    </div>

    <!-- État vide si aucune sollicitation ne correspond -->
    <div
      v-if="contacts.length === 0"
      class="py-14 text-center space-y-3 bg-card border border-border rounded-2xl p-6"
    >
      <div class="w-12 h-12 rounded-full bg-muted/60 flex items-center justify-center mx-auto text-muted-foreground">
        <Filter class="w-6 h-6" />
      </div>
      <h2 class="text-base font-bold font-heading text-foreground">
        Aucune sollicitation trouvée
      </h2>
      <p class="text-xs text-muted-foreground max-w-sm mx-auto font-body">
        Modifiez vos filtres ou effectuez une prise de contact depuis le catalogue pour enrichir votre tableau de bord.
      </p>
      <Button
        v-if="searchQuery || statusFilter !== 'ALL'"
        variant="outline"
        size="sm"
        class="mt-2 font-heading text-xs"
        @click="emit('update:searchQuery', ''); emit('update:statusFilter', 'ALL')"
      >
        Réinitialiser les filtres
      </Button>
    </div>

    <!-- Liste des sollicitations envoyées -->
    <div v-else class="space-y-4">
      <article
        v-for="contact in contacts"
        :key="contact.id"
        class="p-4 sm:p-5 rounded-2xl bg-card border border-border shadow-xs hover:border-[#000091]/40 transition-all space-y-4 relative"
      >
        <div class="flex flex-col sm:flex-row sm:items-center justify-between gap-3 border-b border-border/60 pb-3.5">
          <!-- Candidat ciblé -->
          <div class="flex items-center gap-3">
            <div class="relative w-12 h-12 rounded-xl overflow-hidden bg-black shrink-0 border border-border">
              <img
                :src="contact.candidateAvatarUrl"
                :alt="`Photo de ${contact.candidateName}`"
                class="w-full h-full object-cover"
              />
            </div>

            <div>
              <div class="flex items-center gap-2 flex-wrap">
                <RouterLink
                  :to="`/profils/${contact.candidateId}`"
                  class="font-bold font-heading text-sm sm:text-base text-foreground hover:text-[#000091] dark:hover:text-[#90b8ff] transition-colors"
                >
                  {{ contact.candidateName }}
                </RouterLink>

                <Badge
                  v-if="contact.isJebCertified"
                  variant="jeb"
                  class="text-[10px] gap-1 px-1.5 py-0"
                >
                  <ShieldCheck class="w-3 h-3 text-[#f5c842]" />
                  <span>Certifié</span>
                </Badge>
              </div>

              <p class="text-xs text-muted-foreground font-body">
                {{ contact.candidateHeadline }}
              </p>

              <p v-if="contact.location" class="text-[11px] text-muted-foreground/80 flex items-center gap-1 mt-0.5 font-heading">
                <MapPin class="w-3 h-3 text-muted-foreground" />
                <span>{{ contact.location }}</span>
              </p>
            </div>
          </div>

          <!-- Date & Statut avec sélecteur de transition -->
          <div class="flex flex-col sm:items-end gap-2 shrink-0">
            <span class="text-[11px] text-muted-foreground font-mono">
              Envoyé le {{ formatDate(contact.date) }}
            </span>

            <!-- Badge de statut & Menu de modification -->
            <div class="relative">
              <button
                type="button"
                class="flex items-center gap-1.5 px-3 py-1 rounded-full text-xs font-heading font-semibold border transition-all cursor-pointer shadow-2xs"
                :class="getStatusBadgeClass(contact.status)"
                @click="toggleStatusMenu(contact.id)"
                aria-label="Changer le statut du candidat"
              >
                <span>{{ contact.status }}</span>
                <ChevronDown class="w-3.5 h-3.5 opacity-80" />
              </button>

              <!-- Menu déroulant accessible pour changer de statut -->
              <div
                v-if="activeMenuContactId === contact.id"
                class="absolute right-0 top-full mt-1.5 z-20 w-52 p-1.5 rounded-xl bg-popover border border-border shadow-lg space-y-1 animate-in fade-in-50 zoom-in-95"
              >
                <p class="text-[10px] font-bold font-heading text-muted-foreground uppercase px-2 py-1">
                  Changer le statut :
                </p>
                <button
                  v-for="st in allStatuses"
                  :key="st"
                  type="button"
                  class="w-full text-left px-2.5 py-1.5 rounded-lg text-xs font-heading font-medium flex items-center justify-between transition-colors cursor-pointer"
                  :class="
                    contact.status === st
                      ? 'bg-[#000091]/10 text-[#000091] font-bold'
                      : 'hover:bg-muted text-foreground'
                  "
                  @click="handleSelectStatus(contact.id, st)"
                >
                  <span>{{ st }}</span>
                  <CheckCircle2 v-if="contact.status === st" class="w-3.5 h-3.5 text-[#000091]" />
                </button>
              </div>
            </div>
          </div>
        </div>

        <!-- Corps du message envoyé -->
        <div class="bg-muted/30 border border-border/50 rounded-xl p-3 sm:p-3.5 space-y-1">
          <p class="text-[11px] font-bold font-heading text-muted-foreground uppercase tracking-wider">
            Message transmis au candidat :
          </p>
          <p class="text-xs sm:text-sm text-foreground/90 font-body leading-relaxed whitespace-pre-line">
            « {{ contact.message }} »
          </p>
        </div>

        <!-- Barre d'actions rapides -->
        <div class="flex items-center justify-between gap-2 pt-1">
          <div class="flex items-center gap-2">
            <RouterLink :to="`/profils/${contact.candidateId}`">
              <Button variant="outline" size="sm" class="h-8 gap-1.5 text-xs font-heading">
                <Eye class="w-3.5 h-3.5" />
                <span>Voir profil</span>
              </Button>
            </RouterLink>

            <Button
              variant="secondaryCta"
              size="sm"
              class="h-8 gap-1.5 text-xs font-heading font-bold"
              @click="emit('recontact', contact.candidateId, contact.candidateName)"
            >
              <MessageSquare class="w-3.5 h-3.5" />
              <span>Nouveau message</span>
            </Button>
          </div>

          <Button
            variant="ghost"
            size="sm"
            class="h-8 px-2 text-muted-foreground hover:text-destructive cursor-pointer text-xs"
            title="Supprimer de l'historique"
            aria-label="Supprimer de l'historique"
            @click="emit('deleteContact', contact.id)"
          >
            <Trash2 class="w-4 h-4" />
          </Button>
        </div>
      </article>
    </div>
  </section>
</template>
