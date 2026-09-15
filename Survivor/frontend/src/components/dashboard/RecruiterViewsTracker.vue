<script setup lang="ts">
import { ref, computed } from 'vue'
import {
  Eye,
  TrendingUp,
  Calendar,
  Building2,
  Video,
  Clock,
  Sparkles,
  BarChart3,
  UserCheck,
} from 'lucide-vue-next'
import { Badge } from '@/components/ui/badge'
import type { RecruiterViewsStatsVO } from '@/core/domain/value-objects/RecruiterViewsStats.vo'

const props = defineProps<{
  stats: RecruiterViewsStatsVO | null
}>()

const timeRange = ref<'7days' | '4weeks'>('7days')

const activeChartData = computed(() => {
  if (!props.stats) return []
  if (timeRange.value === '7days') {
    return props.stats.dailyViewsLast7Days.map((d) => ({
      label: d.label,
      count: d.count,
    }))
  }
  return props.stats.weeklyViewsLast4Weeks.map((w) => ({
    label: w.label,
    count: w.count,
  }))
})

const maxViewCount = computed(() => {
  const counts = activeChartData.value.map((d) => d.count)
  const max = Math.max(...counts, 1)
  return Math.ceil(max * 1.2)
})

function getBarHeightPercent(count: number): number {
  if (maxViewCount.value <= 0) return 0
  return Math.min(100, Math.round((count / maxViewCount.value) * 100))
}
</script>

<template>
  <div class="space-y-6">
    <!-- Indicateurs clés KPI -->
    <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-4">
      <!-- Total Vues -->
      <div class="rounded-xl bg-[#121218] border border-white/10 p-4 relative overflow-hidden group hover:border-[#1a6fff]/40 transition-colors">
        <div class="flex items-center justify-between">
          <span class="text-xs font-medium text-white/60 font-body">Total Vues Recruteurs</span>
          <div class="w-8 h-8 rounded-lg bg-[#1a6fff]/10 flex items-center justify-center text-[#1a6fff]">
            <Eye class="w-4 h-4" />
          </div>
        </div>
        <div class="mt-3 flex items-baseline gap-2">
          <span class="text-2xl font-bold font-heading text-white">
            {{ stats ? stats.formatNumber(stats.totalViews) : '—' }}
          </span>
          <span class="text-xs text-[#00d4ff] flex items-center font-medium">
            <Sparkles class="w-3 h-3 mr-1 inline" /> Cumul
          </span>
        </div>
        <p class="text-[11px] text-white/40 mt-1 font-body">Visites uniques par des profils vérifiés</p>
      </div>

      <!-- Vues Hebdomadaires -->
      <div class="rounded-xl bg-[#121218] border border-white/10 p-4 relative overflow-hidden group hover:border-[#00d4ff]/40 transition-colors">
        <div class="flex items-center justify-between">
          <span class="text-xs font-medium text-white/60 font-body">Consultations Hebdo</span>
          <div class="w-8 h-8 rounded-lg bg-[#00d4ff]/10 flex items-center justify-center text-[#00d4ff]">
            <Calendar class="w-4 h-4" />
          </div>
        </div>
        <div class="mt-3 flex items-baseline gap-2">
          <span class="text-2xl font-bold font-heading text-white">
            {{ stats ? stats.weeklyViews : '—' }}
          </span>
          <span
            v-if="stats"
            class="text-xs font-semibold px-1.5 py-0.5 rounded bg-emerald-500/15 text-emerald-400 flex items-center"
          >
            <TrendingUp class="w-3 h-3 mr-0.5 inline" />
            {{ stats.formatPercent(stats.weeklyChangePercent) }}
          </span>
        </div>
        <p class="text-[11px] text-white/40 mt-1 font-body">Sur les 7 derniers jours</p>
      </div>

      <!-- Vues Mensuelles -->
      <div class="rounded-xl bg-[#121218] border border-white/10 p-4 relative overflow-hidden group hover:border-purple-500/40 transition-colors">
        <div class="flex items-center justify-between">
          <span class="text-xs font-medium text-white/60 font-body">Consultations Mois</span>
          <div class="w-8 h-8 rounded-lg bg-purple-500/10 flex items-center justify-center text-purple-400">
            <BarChart3 class="w-4 h-4" />
          </div>
        </div>
        <div class="mt-3 flex items-baseline gap-2">
          <span class="text-2xl font-bold font-heading text-white">
            {{ stats ? stats.monthlyViews : '—' }}
          </span>
          <span
            v-if="stats"
            class="text-xs font-semibold px-1.5 py-0.5 rounded bg-emerald-500/15 text-emerald-400 flex items-center"
          >
            <TrendingUp class="w-3 h-3 mr-0.5 inline" />
            {{ stats.formatPercent(stats.monthlyChangePercent) }}
          </span>
        </div>
        <p class="text-[11px] text-white/40 mt-1 font-body">Sur les 30 derniers jours</p>
      </div>

      <!-- Taux de visionnage vidéo -->
      <div class="rounded-xl bg-[#121218] border border-white/10 p-4 relative overflow-hidden group hover:border-[#f5c842]/40 transition-colors">
        <div class="flex items-center justify-between">
          <span class="text-xs font-medium text-white/60 font-body">Visionnages Vidéo</span>
          <div class="w-8 h-8 rounded-lg bg-[#f5c842]/10 flex items-center justify-center text-[#f5c842]">
            <Video class="w-4 h-4" />
          </div>
        </div>
        <div class="mt-3 flex items-baseline gap-2">
          <span class="text-2xl font-bold font-heading text-white">68%</span>
          <Badge variant="secondary" class="text-[10px] bg-white/5 text-[#f5c842] border-[#f5c842]/20">
            Fort impact
          </Badge>
        </div>
        <p class="text-[11px] text-white/40 mt-1 font-body">Des recruteurs ont lu votre pitch</p>
      </div>
    </div>

    <!-- Graphique visuel de consultation -->
    <div class="rounded-xl bg-[#121218] border border-white/10 p-6 space-y-6">
      <div class="flex flex-col sm:flex-row sm:items-center justify-between gap-4">
        <div>
          <h3 class="text-base font-bold font-heading text-white flex items-center gap-2">
            <BarChart3 class="w-4 h-4 text-[#00d4ff]" />
            Évolution des consultations par les recruteurs
          </h3>
          <p class="text-xs text-white/50 font-body mt-0.5">
            Suivez l'attractivité de votre profil et de votre vidéo auprès des entreprises
          </p>
        </div>

        <div class="flex items-center gap-1 bg-[#1a1a24] p-1 rounded-lg border border-white/10 self-start sm:self-auto">
          <button
            type="button"
            class="px-3 py-1.5 text-xs font-medium rounded-md transition-all"
            :class="timeRange === '7days' ? 'bg-[#1a6fff] text-white shadow-sm' : 'text-white/60 hover:text-white'"
            @click="timeRange = '7days'"
          >
            7 derniers jours
          </button>
          <button
            type="button"
            class="px-3 py-1.5 text-xs font-medium rounded-md transition-all"
            :class="timeRange === '4weeks' ? 'bg-[#1a6fff] text-white shadow-sm' : 'text-white/60 hover:text-white'"
            @click="timeRange = '4weeks'"
          >
            4 dernières semaines
          </button>
        </div>
      </div>

      <!-- Histogramme visuel avec Tailwind & CSS -->
      <div class="pt-4 pb-2">
        <div class="h-48 flex items-end gap-3 sm:gap-6 border-b border-white/10 px-2 sm:px-4">
          <div
            v-for="(item, idx) in activeChartData"
            :key="idx"
            class="flex-1 flex flex-col items-center gap-2 group h-full justify-end"
          >
            <!-- Valeur affichée au survol ou permanente -->
            <span class="text-[11px] font-mono font-semibold text-white/70 group-hover:text-[#00d4ff] transition-colors">
              {{ item.count }}
            </span>

            <!-- Barre visuelle animée avec dégradé Néon -->
            <div class="w-full max-w-[48px] bg-white/5 rounded-t-md overflow-hidden relative flex items-end h-full">
              <div
                class="w-full rounded-t-md bg-gradient-to-t from-[#1a6fff] to-[#00d4ff] transition-all duration-500 group-hover:brightness-125 group-hover:shadow-[0_0_15px_rgba(0,212,255,0.4)]"
                :style="{ height: `${getBarHeightPercent(item.count)}%` }"
              />
            </div>

            <!-- Libellé du jour ou de la semaine -->
            <span class="text-[10px] sm:text-xs text-white/50 text-center font-body truncate w-full group-hover:text-white transition-colors">
              {{ item.label }}
            </span>
          </div>
        </div>
      </div>
    </div>

    <!-- Dernières visites recruteurs -->
    <div class="rounded-xl bg-[#121218] border border-white/10 p-6 space-y-4">
      <div class="flex items-center justify-between">
        <h3 class="text-base font-bold font-heading text-white flex items-center gap-2">
          <UserCheck class="w-4 h-4 text-[#1a6fff]" />
          Dernières visites d'entreprises & recruteurs
        </h3>
        <span class="text-xs text-white/40 font-body">Historique récent</span>
      </div>

      <div class="divide-y divide-white/5">
        <div
          v-for="visit in stats?.recentVisits || []"
          :key="visit.id"
          class="py-3 flex flex-col sm:flex-row sm:items-center justify-between gap-3 hover:bg-white/[0.02] px-2 rounded-lg transition-colors"
        >
          <div class="flex items-center gap-3">
            <div class="w-10 h-10 rounded-lg bg-[#1a1a24] border border-white/10 flex items-center justify-center text-white/80 shrink-0">
              <Building2 class="w-5 h-5 text-[#00d4ff]" />
            </div>
            <div>
              <div class="flex items-center gap-2">
                <span class="font-medium text-sm text-white font-heading">{{ visit.company }}</span>
                <span class="text-xs text-white/40">· {{ visit.recruiterName }}</span>
              </div>
              <p class="text-xs text-white/50 font-body">{{ visit.recruiterRole || 'Recruteur' }}</p>
            </div>
          </div>

          <div class="flex items-center gap-3 sm:self-center self-end">
            <span
              v-if="visit.viewedVideo"
              class="inline-flex items-center gap-1 text-[11px] px-2 py-0.5 rounded-full bg-[#1a6fff]/15 text-[#00d4ff] border border-[#1a6fff]/30 font-medium"
            >
              <Video class="w-3 h-3" /> Vidéo consultée
            </span>
            <span class="text-xs text-white/40 flex items-center gap-1 font-body">
              <Clock class="w-3.5 h-3.5" /> {{ visit.visitedAt }}
            </span>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>
