<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { RouterLink, useRoute } from 'vue-router'
import {
  LayoutDashboard,
  ShieldAlert,
  FileCheck2,
  Lock,
  ChevronRight,
  ExternalLink,
} from 'lucide-vue-next'
import { ModerationService } from '@/core/services/ModerationService'

defineProps<{
  currentViewTitle: string
}>()

const route = useRoute()
const moderationService = new ModerationService()
const pendingModerationCount = ref(0)

onMounted(async () => {
  try {
    const counts = await moderationService.getCounts()
    pendingModerationCount.value = counts.totalToModerate
  } catch {
    // Non-blocking
  }
})

const navTabs = [
  {
    to: '/admin',
    name: 'AdminDashboard',
    label: 'Tableau de bord (KPIs)',
    icon: LayoutDashboard,
    badge: null,
  },
  {
    to: '/admin/moderation',
    name: 'AdminModeration',
    label: 'Modération des vidéos',
    icon: ShieldAlert,
    badge: pendingModerationCount,
  },
  {
    to: '/admin/certification',
    name: 'AdminCertification',
    label: 'Référentiel certification',
    icon: FileCheck2,
    badge: '20',
  },
]
</script>

<template>
  <header class="w-full bg-[#000091] text-white select-none border-b border-[#000070] shadow-md">
    <!-- En-tête Institutionnel Marianne DSFR -->
    <div class="container mx-auto px-4 sm:px-6 py-3.5">
      <div class="flex flex-col sm:flex-row sm:items-center justify-between gap-3">
        <!-- Logo & Titre Institutionnel -->
        <div class="flex items-center gap-3">
          <div class="flex flex-col justify-center px-2 py-1 bg-white text-[#000091] rounded font-heading font-black text-xs tracking-tighter leading-none border border-white/20">
            <span>RÉPUBLIQUE</span>
            <span>FRANÇAISE</span>
          </div>

          <div class="h-8 w-px bg-white/20 hidden sm:block" />

          <div>
            <div class="flex items-center gap-2">
              <span class="text-xs font-semibold uppercase tracking-wider text-blue-200 font-heading">
                Plateforme Nationale Démonstrateur
              </span>
              <span class="inline-flex items-center gap-1 text-[11px] bg-blue-900/60 text-blue-100 border border-blue-400/30 px-2 py-0.5 rounded-full font-medium">
                <Lock class="w-3 h-3 text-amber-300" />
                Administration Back-Office
              </span>
            </div>
            <h1 class="text-lg sm:text-xl font-bold tracking-tight text-white font-heading">
              ProfilsActifs — Espace Administration
            </h1>
          </div>
        </div>

        <!-- Liens rapides / Vue publique -->
        <div class="flex items-center gap-2 self-start sm:self-center">
          <RouterLink
            to="/recruteur/dashboard"
            class="text-xs font-semibold text-white/80 hover:text-white flex items-center gap-1 px-2.5 py-1.5 rounded-lg bg-white/10 hover:bg-white/20 transition-colors border border-white/15"
          >
            <span>Catalogue public</span>
            <ExternalLink class="w-3.5 h-3.5" />
          </RouterLink>
        </div>
      </div>

      <!-- Fil d'Ariane (Breadcrumbs) -->
      <nav aria-label="Fil d'Ariane" class="mt-3 pt-2.5 border-t border-white/10 flex items-center gap-1.5 text-xs text-white/70">
        <RouterLink to="/" class="hover:text-white transition-colors">Accueil</RouterLink>
        <ChevronRight class="w-3.5 h-3.5 opacity-50" />
        <RouterLink to="/admin" class="hover:text-white transition-colors">Administration</RouterLink>
        <ChevronRight class="w-3.5 h-3.5 opacity-50" />
        <span class="text-white font-semibold">{{ currentViewTitle }}</span>
      </nav>
    </div>

    <!-- Barre d'Onglets Navigation DSFR -->
    <div class="bg-[#000075] border-t border-white/10 px-4 sm:px-6">
      <div class="container mx-auto flex items-center gap-2 overflow-x-auto py-1">
        <RouterLink
          v-for="tab in navTabs"
          :key="tab.to"
          :to="tab.to"
          class="flex items-center gap-2 px-4 py-2.5 text-xs sm:text-sm font-semibold rounded-t-lg transition-all border-b-2"
          :class="[
            route.path === tab.to
              ? 'bg-[#09090f] text-white border-amber-400 font-bold shadow-inner'
              : 'text-white/75 hover:text-white hover:bg-white/10 border-transparent',
          ]"
        >
          <component :is="tab.icon" class="w-4 h-4 shrink-0" />
          <span class="whitespace-nowrap">{{ tab.label }}</span>
          <span
            v-if="tab.badge"
            class="ml-1 text-[10px] font-bold px-1.5 py-0.5 rounded-full"
            :class="[
              typeof tab.badge === 'number' || (typeof tab.badge === 'object' && tab.badge.value > 0)
                ? 'bg-amber-400 text-blue-950'
                : 'bg-white/20 text-white',
            ]"
          >
            {{ typeof tab.badge === 'object' ? tab.badge.value : tab.badge }}
          </span>
        </RouterLink>
      </div>
    </div>
  </header>
</template>
