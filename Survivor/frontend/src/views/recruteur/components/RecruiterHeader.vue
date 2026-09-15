<script setup lang="ts">
import { computed } from 'vue'
import { Users, Bookmark, SendHorizontal } from 'lucide-vue-next'
import { Badge } from '@/components/ui/badge'

export type RecruiterTab = 'catalogue' | 'favoris' | 'suivi'

const props = defineProps<{
  activeTab: RecruiterTab
  favoritesCount: number
  contactsCount: number
  totalCandidates: number
  isMobileMode: boolean
}>()

const emit = defineEmits<{
  (e: 'update:activeTab', tab: RecruiterTab): void
}>()

const tabs = computed(() => [
  {
    id: 'catalogue' as RecruiterTab,
    label: 'Catalogue des Candidats',
    shortLabel: 'Catalogue',
    icon: Users,
    badge: props.totalCandidates > 0 ? props.totalCandidates : undefined,
  },
  {
    id: 'favoris' as RecruiterTab,
    label: 'Mes Favoris',
    shortLabel: 'Favoris',
    icon: Bookmark,
    badge: props.favoritesCount,
    highlightBadge: props.favoritesCount > 0,
  },
  {
    id: 'suivi' as RecruiterTab,
    label: 'Suivi des Sollicitations',
    shortLabel: 'Suivi contacts',
    icon: SendHorizontal,
    badge: props.contactsCount,
    highlightBadge: props.contactsCount > 0,
  },
])
</script>

<template>
  <header class="space-y-4">
    <!-- En-tête titre & actions rapides -->
    <div class="flex flex-col sm:flex-row sm:items-center justify-between gap-3">
      <div>
        <div class="flex items-center gap-2 flex-wrap">
          <h1 class="text-xl sm:text-2xl md:text-3xl font-bold font-heading text-foreground tracking-tight">
            Espace Recruteur
          </h1>
          <span
            v-if="isMobileMode"
            class="px-2 py-0.5 rounded text-[10px] font-bold font-heading bg-[#000091]/20 text-[#000091] border border-[#000091]/30"
          >
            Mobile 390px
          </span>
        </div>
        <p class="text-xs sm:text-sm text-muted-foreground mt-1 font-body">
          Sourcing de talents, gestion des favoris et suivi complet des sollicitations d'embauche.
        </p>
      </div>

      <div class="flex items-center gap-2 self-start sm:self-auto">
        <!-- <RouterLink to="/feed">
          <Button variant="outline" size="sm" class="h-9 gap-1.5 text-xs font-heading font-semibold">
            <Play class="w-3.5 h-3.5 text-[#000091]" />
            <span>Mode Flux Vidéo</span>
          </Button>
        </RouterLink> -->
      </div>
    </div>

    <!-- Navigation par onglets de l'Espace Recruteur -->
    <nav
      class="flex items-center gap-1.5 p-1 rounded-xl bg-card border border-border overflow-x-auto no-scrollbar shadow-xs"
      role="tablist"
      aria-label="Navigation Espace Recruteur"
    >
      <button
        v-for="tab in tabs"
        :key="tab.id"
        type="button"
        role="tab"
        :aria-selected="activeTab === tab.id"
        class="flex items-center gap-2 px-3.5 py-2 rounded-lg text-xs sm:text-sm font-heading font-semibold transition-all cursor-pointer whitespace-nowrap shrink-0"
        :class="
          activeTab === tab.id
            ? 'bg-[#000091] text-white shadow-sm'
            : 'text-muted-foreground hover:text-foreground hover:bg-muted/40'
        "
        @click="emit('update:activeTab', tab.id)"
      >
        <component :is="tab.icon" class="w-4 h-4" />
        <span class="hidden sm:inline">{{ tab.label }}</span>
        <span class="sm:hidden">{{ tab.shortLabel }}</span>

        <Badge
          v-if="tab.badge !== undefined"
          variant="secondary"
          class="text-[10px] h-4.5 min-w-4.5 px-1.5 py-0 flex items-center justify-center rounded-full font-mono font-bold"
          :class="
            activeTab === tab.id
              ? 'bg-white/20 text-white border-0'
              : tab.highlightBadge
              ? 'bg-[#000091]/15 text-[#000091] border-transparent'
              : 'bg-muted text-muted-foreground'
          "
        >
          {{ tab.badge }}
        </Badge>
      </button>
    </nav>
  </header>
</template>
