<script setup lang="ts">
import { ref, shallowRef, computed, onMounted } from 'vue'
import {
  Inbox,
  Eye,
  FileEdit,
  TrendingUp,
  ShieldCheck,
} from 'lucide-vue-next'
import { RecruiterViewsMockRepository } from '@/core/infrastructure/repositories/RecruiterViewsMockRepository'
import { JobSeekerHttpRepository } from '@/core/infrastructure/repositories/JobSeekerHttpRepository'
import {
  RecruiterSolicitationEntity,
  type SolicitationStatus,
} from '@/core/domain/entities/RecruiterSolicitation.entity'
import { RecruiterViewsStatsVO } from '@/core/domain/value-objects/RecruiterViewsStats.vo'
import type { IJobSeekerSolicitationRaw } from '@/core/interfaces/IJobSeekerRepository'
import { useDevicePreview } from '@/composables/useDevicePreview'
import { useCandidateProfileEditor } from '@/composables/useCandidateProfileEditor'
import DevicePreviewBar from '@/components/common/DevicePreviewBar.vue'
import { Tabs, TabsList, TabsTrigger, TabsContent } from '@/components/ui/tabs'
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar'
import { Badge } from '@/components/ui/badge'

import RecruiterViewsTracker from '@/components/dashboard/RecruiterViewsTracker.vue'
import RecruiterInbox from '@/components/dashboard/RecruiterInbox.vue'
import CandidateProfileEditor from '@/components/dashboard/CandidateProfileEditor.vue'
import CandidateVideoSection from '@/components/dashboard/CandidateVideoSection.vue'

const viewsRepo = new RecruiterViewsMockRepository()
const jobSeekerRepo = new JobSeekerHttpRepository()
const {
  candidateProfile,
  videoLink,
  videoValid,
  videoReason,
  profileInitialData,
  load: loadProfileEditor,
  handleSaveProfile,
  handleSaveVideo,
} = useCandidateProfileEditor(jobSeekerRepo)

function mapRawSolicitation(item: IJobSeekerSolicitationRaw): RecruiterSolicitationEntity {
  const first = item.recruiter_firstname || ''
  const last = item.recruiter_name || ''
  const recruiterName = `${first} ${last}`.trim() || 'Recruteur Partenaire'
  const loc = item.recruiter_location ? ` (${item.recruiter_location})` : ''
  const company = `Entreprise Partenaire${loc}`
  const dateStr = item.created_at
    ? new Date(item.created_at).toLocaleDateString('fr-FR', {
        day: '2-digit',
        month: 'short',
        hour: '2-digit',
        minute: '2-digit',
      })
    : "Aujourd'hui"

  const status: SolicitationStatus = item.is_archived
    ? 'archived'
    : item.is_read
      ? 'read'
      : 'unread'

  return new RecruiterSolicitationEntity({
    id: String(item.id),
    recruiterName,
    company,
    recruiterRole: 'Responsable Recrutement',
    avatarUrl: `https://i.pravatar.cc/150?u=${encodeURIComponent(item.recruiter_id || item.id)}`,
    date: dateStr,
    subject: `Contact Recruteur - ${recruiterName}`,
    message: item.message || 'Prise de contact suite à votre profil actif.',
    status,
    reply: item.reply || undefined,
    repliedAt: item.replied_at || undefined,
    createdAt: item.created_at || new Date().toISOString(),
  })
}

const { currentFormat } = useDevicePreview()

const activeTab = ref<'inbox' | 'views' | 'profile'>('inbox')
const stats = shallowRef<RecruiterViewsStatsVO | null>(null)
const solicitations = shallowRef<RecruiterSolicitationEntity[]>([])
const sectorsList = ref<string[]>([])
const isLoading = ref(true)

const unreadCount = computed(() => {
  return solicitations.value.filter((s) => s.isUnread).length
})

const candidateFullName = computed(() => {
  if (!candidateProfile.value) return 'Alex Martin'
  const full = `${candidateProfile.value.firstname || ''} ${candidateProfile.value.name || ''}`.trim()
  return full.length > 0 ? full : 'Alex Martin'
})

const candidateInitials = computed(() => {
  return candidateFullName.value.slice(0, 2).toUpperCase()
})

const candidateLocation = computed(() => {
  return candidateProfile.value?.localisation || 'Paris, 75'
})

const candidateHeadline = computed(() => {
  const sector = sectorsList.value[0] || 'Développeur full stack'
  return `${sector} · ${candidateLocation.value} · Disponible immédiatement`
})

const isJebCertified = computed(() => {
  return Boolean(candidateProfile.value?.certificated)
})

async function loadDashboardData(): Promise<void> {
  isLoading.value = true
  try {
    stats.value = await viewsRepo.getStats()

    const [interactionsRes, sectsRes] = await Promise.allSettled([
      jobSeekerRepo.getInteractions(),
      jobSeekerRepo.getSectors(),
    ])
    await loadProfileEditor()

    if (interactionsRes.status === 'fulfilled') {
      const interactions = interactionsRes.value

      // 1. Mise à jour du compteur de vues de profil
      if (typeof interactions.viewsCount === 'number') {
        const baseProps = stats.value?.props || {
          totalViews: interactions.viewsCount,
          weeklyViews: Math.round(interactions.viewsCount * 0.1),
          weeklyChangePercent: 12,
          monthlyViews: interactions.viewsCount,
          monthlyChangePercent: 8,
          dailyViewsLast7Days: [],
          weeklyViewsLast4Weeks: [],
          recentVisits: [],
        }
        stats.value = new RecruiterViewsStatsVO({
          ...baseProps,
          totalViews: interactions.viewsCount,
        })
      }

      // 2. Mise à jour de la liste des sollicitations reçues dans l'inbox avec les vrais messages, noms des recruteurs et dates
      solicitations.value = Array.isArray(interactions.solicitations)
        ? interactions.solicitations.map(mapRawSolicitation)
        : []
    }

    if (sectsRes.status === 'fulfilled') {
      sectorsList.value = sectsRes.value
    }
  } catch {
    // Mode dégradé sécurisé
  } finally {
    isLoading.value = false
  }
}

function applySolicitationUpdate(id: string, raw: IJobSeekerSolicitationRaw): void {
  const index = solicitations.value.findIndex((s) => s.id === id)
  if (index !== -1) {
    solicitations.value.splice(index, 1, mapRawSolicitation(raw))
  }
}

async function handleMarkRead(id: string): Promise<void> {
  try {
    applySolicitationUpdate(id, await jobSeekerRepo.markSolicitationRead(id, true))
  } catch {
    // Gestion silencieuse d'erreur
  }
}

async function handleMarkUnread(id: string): Promise<void> {
  try {
    applySolicitationUpdate(id, await jobSeekerRepo.markSolicitationRead(id, false))
  } catch {
    // Gestion silencieuse d'erreur
  }
}

async function handleArchive(id: string): Promise<void> {
  try {
    applySolicitationUpdate(id, await jobSeekerRepo.archiveSolicitation(id, true))
  } catch {
    // Gestion silencieuse d'erreur
  }
}

async function handleUnarchive(id: string): Promise<void> {
  try {
    applySolicitationUpdate(id, await jobSeekerRepo.archiveSolicitation(id, false))
  } catch {
    // Gestion silencieuse d'erreur
  }
}

async function handleReply(id: string, message: string): Promise<void> {
  try {
    applySolicitationUpdate(id, await jobSeekerRepo.replyToSolicitation(id, message))
  } catch {
    // Gestion silencieuse d'erreur
  }
}

onMounted(() => {
  loadDashboardData()
})
</script>

<template>
  <div class="flex-1 bg-[#09090f] text-white min-h-screen">
    <!-- Barre de prévisualisation responsive (Mobile / Tablet / Desktop) -->
    <DevicePreviewBar :active-format="currentFormat" @update:format="(f) => (currentFormat = f)" />

    <div
      class="w-full mx-auto px-4 sm:px-6 lg:px-8 py-8 space-y-8 transition-all duration-300"
      :class="{
        'max-w-[390px]': currentFormat === 'mobile',
        'max-w-[768px]': currentFormat === 'tablet',
        'max-w-7xl': currentFormat === 'desktop',
      }"
    >
      <!-- En-tête profil candidat & aperçu instantané -->
      <div class="rounded-2xl bg-[#121218] border border-white/10 p-6 relative overflow-hidden">
        <div class="absolute -right-10 -top-10 w-48 h-48 bg-[#1a6fff]/10 rounded-full blur-3xl pointer-events-none" />

        <div class="flex flex-col md:flex-row md:items-center justify-between gap-6 relative z-10">
          <div class="flex items-center gap-4">
            <div class="relative">
              <Avatar size="xl" class="ring-2 ring-[#00d4ff]/40">
                <AvatarImage v-if="candidateProfile?.avatar_url" :src="candidateProfile.avatar_url" :alt="candidateFullName" />
                <AvatarFallback class="bg-[#1a1a24] text-[#00d4ff] font-heading font-bold">{{ candidateInitials }}</AvatarFallback>
              </Avatar>
              <span
                class="absolute -bottom-1 -right-1 w-5 h-5 rounded-full bg-emerald-500 border-2 border-[#121218] flex items-center justify-center"
                title="Profil actif"
              />
            </div>

            <div class="space-y-1">
              <div class="flex flex-wrap items-center gap-2">
                <h1 class="text-2xl font-bold font-heading text-white">{{ candidateFullName }}</h1>
                <Badge v-if="isJebCertified" class="bg-emerald-500/15 text-emerald-400 border-emerald-500/30 gap-1 text-xs">
                  <ShieldCheck class="w-3.5 h-3.5" /> Badge de certification JEB
                </Badge>
              </div>
              <p class="text-xs sm:text-sm text-white/60 font-body">
                {{ candidateHeadline }}
              </p>
            </div>
          </div>

          <!-- Résumé des métriques rapides -->
          <div class="flex items-center gap-3 sm:gap-6 border-t sm:border-t-0 border-white/10 pt-4 sm:pt-0">
            <div class="text-center sm:text-right">
              <span class="text-xs text-white/40 block font-body">Vues profil</span>
              <span class="text-lg font-bold font-heading text-white flex items-center gap-1 justify-center sm:justify-end">
                <Eye class="w-4 h-4 text-[#00d4ff]" />
                {{ stats ? stats.formatNumber(stats.totalViews) : '1 420' }}
              </span>
            </div>

            <div class="h-8 w-px bg-white/10" />

            <div class="text-center sm:text-right">
              <span class="text-xs text-white/40 block font-body">Cette semaine</span>
              <span class="text-lg font-bold font-heading text-emerald-400 flex items-center gap-0.5 justify-center sm:justify-end">
                <TrendingUp class="w-4 h-4" />
                +{{ stats ? stats.weeklyViews : '84' }}
              </span>
            </div>

            <div class="h-8 w-px bg-white/10" />

            <div class="text-center sm:text-right">
              <span class="text-xs text-white/40 block font-body">Messages</span>
              <span class="text-lg font-bold font-heading text-[#1a6fff] flex items-center gap-1 justify-center sm:justify-end">
                <Inbox class="w-4 h-4" />
                {{ solicitations.length }}
              </span>
            </div>
          </div>
        </div>
      </div>

      <!-- Navigation par Onglets principaux de l'Espace Demandeur -->
      <Tabs v-model="activeTab" class="space-y-6">
        <TabsList class="bg-[#121218] border border-white/10 p-1 rounded-xl h-auto flex flex-wrap gap-1">
          <TabsTrigger
            value="inbox"
            class="gap-2 px-4 py-2.5 text-xs sm:text-sm font-heading rounded-lg data-[state=active]:bg-[#1a6fff] data-[state=active]:text-white transition-all flex items-center"
          >
            <Inbox class="w-4 h-4" />
            <span>Boîte de réception recruteurs</span>
            <span
              v-if="unreadCount > 0"
              class="px-2 py-0.5 rounded-full text-[11px] font-bold bg-[#00d4ff] text-[#09090f] ml-1"
            >
              {{ unreadCount }}
            </span>
          </TabsTrigger>

          <TabsTrigger
            value="views"
            class="gap-2 px-4 py-2.5 text-xs sm:text-sm font-heading rounded-lg data-[state=active]:bg-[#1a6fff] data-[state=active]:text-white transition-all flex items-center"
          >
            <Eye class="w-4 h-4" />
            <span>Suivi des vues recruteurs</span>
          </TabsTrigger>

          <TabsTrigger
            value="profile"
            class="gap-2 px-4 py-2.5 text-xs sm:text-sm font-heading rounded-lg data-[state=active]:bg-[#1a6fff] data-[state=active]:text-white transition-all flex items-center"
          >
            <FileEdit class="w-4 h-4" />
            <span>Édition Profil & Vidéo</span>
          </TabsTrigger>
        </TabsList>

        <!-- Contenu Onglet 1 : Boîte de réception recruteurs -->
        <TabsContent value="inbox" class="focus-visible:outline-none">
          <RecruiterInbox
            :solicitations="solicitations"
            @mark-read="handleMarkRead"
            @mark-unread="handleMarkUnread"
            @archive="handleArchive"
            @unarchive="handleUnarchive"
            @reply="handleReply"
          />
        </TabsContent>

        <!-- Contenu Onglet 2 : Suivi des vues du profil -->
        <TabsContent value="views" class="focus-visible:outline-none">
          <RecruiterViewsTracker :stats="stats" />
        </TabsContent>

        <!-- Contenu Onglet 3 : Édition du profil et Vidéo -->
        <TabsContent value="profile" class="focus-visible:outline-none space-y-6">
          <div class="grid grid-cols-1 lg:grid-cols-2 gap-6">
            <CandidateProfileEditor :initial-data="profileInitialData" @save="handleSaveProfile" />
            <CandidateVideoSection :initial-url="videoLink" :is-valid="videoValid" :rejection-reason="videoReason" @save="handleSaveVideo" />
          </div>
        </TabsContent>
      </Tabs>
    </div>
  </div>
</template>
