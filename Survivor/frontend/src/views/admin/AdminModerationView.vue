<script setup lang="ts">
import { ref, shallowRef, computed, onMounted } from 'vue'
import {
  ShieldAlert,
  CheckCircle2,
  XCircle,
  AlertTriangle,
  Play,
  Clock,
  User,
  Search,
  Check,
} from 'lucide-vue-next'
import AdminHeader from '@/components/admin/AdminHeader.vue'
import type { ModerationFilter } from '@/core/services/ModerationService'
import { ModerationItemEntity, type ModerationStatus } from '@/core/domain/entities/ModerationItem.entity'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
} from '@/components/ui/dialog'
import { HttpClient } from '@/core/infrastructure/http/HttpClient'

interface IPendingMediaItem {
  id: string | number
  user_id: string
  video_url: string
  valid: number | boolean
  validate_by?: string | null
  reason?: string | null
  firstname?: string
  name?: string
  localisation?: string
  headline?: string
}

function mapPendingMediaToItem(raw: IPendingMediaItem): ModerationItemEntity {
  const candidateName = `${raw.firstname || ''} ${raw.name || ''}`.trim() || 'Candidat ProfilsActifs'

  let status: ModerationStatus = 'pending'
  let rejectionReason: string | undefined = undefined
  let rejectionComment: string | undefined = undefined

  if (raw.valid === 1 || raw.valid === true) {
    status = 'approved'
  } else if (raw.reason || raw.validate_by) {
    status = 'rejected'
    if (raw.reason) {
      const parts = raw.reason.split(' — ')
      rejectionReason = parts[0] || raw.reason
      rejectionComment = parts.length > 1 ? parts.slice(1).join(' — ') : undefined
    }
  }

  return new ModerationItemEntity({
    id: String(raw.id),
    candidateId: raw.user_id,
    candidateName,
    candidateHeadline: raw.headline || 'Candidat ProfilsActifs',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=200&q=80',
    videoUrl: raw.video_url || 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 45,
    status,
    rejectionReason,
    rejectionComment,
    submittedAt: new Date().toISOString(),
  })
}

const httpClient = new HttpClient()

const items = shallowRef<ModerationItemEntity[]>([])
const isLoading = ref(true)
const activeFilter = ref<ModerationFilter>('to_moderate')
const searchQuery = ref('')
const notificationMessage = ref<string | null>(null)
const counts = ref({ pending: 0, flagged: 0, approved: 0, rejected: 0, totalToModerate: 0 })

// Lecteur vidéo interactif
const activePlayingId = ref<string | null>(null)
const videoPlayerRefs = ref<Record<string, HTMLVideoElement>>({})

// Modale de rejet
const isRejectModalOpen = ref(false)
const itemToReject = ref<ModerationItemEntity | null>(null)
const selectedRejectReason = ref('Qualité audio inaudible ou bruit excessif')
const customRejectComment = ref('')

const REJECTION_REASONS = [
  'Qualité audio inaudible ou bruit excessif',
  'Contenu non professionnel ou hors sujet',
  'Non-respect RGPD / Divulgation de données sensibles',
  'Durée non conforme (dépassement des 60 secondes)',
  'Propos inappropriés ou non conformes à la charte républicaine',
  'Qualité vidéo ou éclairage insuffisant',
  'Autre motif spécifique',
]

async function loadItems(): Promise<void> {
  isLoading.value = true
  try {
    const pendingRes = await httpClient.get<IPendingMediaItem[]>('/admin/media/pending')
    const allItems = Array.isArray(pendingRes) ? pendingRes.map(mapPendingMediaToItem) : []

    const pendingCount = allItems.filter((i) => i.status === 'pending').length
    const flaggedCount = allItems.filter((i) => i.status === 'flagged').length
    const approvedCount = allItems.filter((i) => i.status === 'approved').length
    const rejectedCount = allItems.filter((i) => i.status === 'rejected').length

    counts.value = {
      pending: pendingCount,
      flagged: flaggedCount,
      approved: approvedCount,
      rejected: rejectedCount,
      totalToModerate: pendingCount + flaggedCount,
    }

    if (activeFilter.value === 'all') {
      items.value = allItems
    } else if (activeFilter.value === 'to_moderate') {
      items.value = allItems.filter((i) => i.status === 'pending' || i.status === 'flagged')
    } else {
      items.value = allItems.filter((i) => i.status === activeFilter.value)
    }
  } catch (err) {
    console.warn('[AdminModerationView] Erreur appel GET /admin/media/pending:', err)
    items.value = []
  } finally {
    isLoading.value = false
  }
}

const filteredItems = computed(() => {
  if (!searchQuery.value.trim()) return items.value
  const q = searchQuery.value.toLowerCase()
  return items.value.filter(
    (item) =>
      item.props.candidateName.toLowerCase().includes(q) ||
      item.props.candidateHeadline.toLowerCase().includes(q) ||
      (item.props.reportReason && item.props.reportReason.toLowerCase().includes(q)) ||
      (item.props.rejectionReason && item.props.rejectionReason.toLowerCase().includes(q)),
  )
})

function togglePlayVideo(id: string): void {
  const el = videoPlayerRefs.value[id]
  if (!el) return

  if (activePlayingId.value === id) {
    el.pause()
    activePlayingId.value = null
  } else {
    // Pause any other playing video
    if (activePlayingId.value && videoPlayerRefs.value[activePlayingId.value]) {
      videoPlayerRefs.value[activePlayingId.value]?.pause()
    }
    el.play().catch(() => {})
    activePlayingId.value = id
  }
}

function setVideoRef(id: string, el: unknown): void {
  if (el instanceof HTMLVideoElement) {
    videoPlayerRefs.value[id] = el
  }
}

function showNotification(msg: string): void {
  notificationMessage.value = msg
  setTimeout(() => {
    notificationMessage.value = null
  }, 4000)
}

async function handleApprove(item: ModerationItemEntity): Promise<void> {
  try {
    await httpClient.patch<unknown>(`/admin/media/valid/${encodeURIComponent(item.id)}`)
    showNotification(`La vidéo de ${item.props.candidateName} a été validée et publiée.`)
    await loadItems()
  } catch (err: unknown) {
    const msg = err instanceof Error ? err.message : 'Erreur lors de la validation'
    alert(msg)
  }
}

function openRejectModal(item: ModerationItemEntity): void {
  itemToReject.value = item
  selectedRejectReason.value = REJECTION_REASONS[0]
  customRejectComment.value = ''
  isRejectModalOpen.value = true
}

async function confirmReject(): Promise<void> {
  if (!itemToReject.value) return

  try {
    await httpClient.patch<unknown>(
      `/admin/media/reject/${encodeURIComponent(itemToReject.value.id)}`,
      {
        reason: selectedRejectReason.value,
        comment: customRejectComment.value || undefined,
      }
    )
    showNotification(`La vidéo de ${itemToReject.value.props.candidateName} a été rejetée (${selectedRejectReason.value}).`)
    isRejectModalOpen.value = false
    itemToReject.value = null
    await loadItems()
  } catch (err: unknown) {
    const msg = err instanceof Error ? err.message : 'Erreur lors du rejet'
    alert(msg)
  }
}

onMounted(() => {
  void loadItems()
})
</script>

<template>
  <div class="min-h-screen bg-[#09090f] text-foreground font-body pb-16">
    <!-- En-tête Institutionnel Marianne DSFR -->
    <AdminHeader current-view-title="Modération des vidéos" />

    <div class="container mx-auto px-4 sm:px-6 py-8 max-w-7xl space-y-6">
      <!-- Notification Toast -->
      <div
        v-if="notificationMessage"
        class="p-4 rounded-xl bg-emerald-500/15 border border-emerald-500/40 text-emerald-300 text-sm flex items-center justify-between animate-in fade-in slide-in-from-top-2 duration-200"
      >
        <div class="flex items-center gap-2">
          <CheckCircle2 class="w-4 h-4 text-emerald-400 shrink-0" />
          <span>{{ notificationMessage }}</span>
        </div>
        <button
          class="text-xs text-emerald-400 hover:underline cursor-pointer"
          @click="notificationMessage = null"
        >
          Fermer
        </button>
      </div>

      <!-- En-tête de page & Compteurs -->
      <div class="flex flex-col lg:flex-row lg:items-center justify-between gap-4 border-b border-white/10 pb-6">
        <div>
          <div class="flex items-center gap-2">
            <h2 class="text-2xl font-bold font-heading text-white tracking-tight">
              File de Modération des Vidéos
            </h2>
            <Badge
              v-if="counts.totalToModerate > 0"
              class="bg-amber-500 text-blue-950 font-bold text-xs"
            >
              {{ counts.totalToModerate }} à traiter
            </Badge>
          </div>
          <p class="text-xs sm:text-sm text-muted-foreground font-body mt-1">
            Contrôle déontologique, validation de conformité RGPD et modération des signalements.
          </p>
        </div>

        <!-- Badges d'état récapitulatifs -->
        <div class="flex items-center gap-2 flex-wrap text-xs">
          <button
            class="px-3 py-1.5 rounded-lg border text-xs font-semibold cursor-pointer transition-all flex items-center gap-1.5"
            :class="[
              activeFilter === 'to_moderate'
                ? 'bg-amber-500/20 text-amber-300 border-amber-500/50 shadow-sm'
                : 'bg-white/5 text-muted-foreground border-white/10 hover:text-white',
            ]"
            @click="activeFilter = 'to_moderate'; loadItems()"
          >
            <ShieldAlert class="w-3.5 h-3.5 text-amber-400" />
            <span>À modérer ({{ counts.totalToModerate }})</span>
          </button>

          <button
            class="px-3 py-1.5 rounded-lg border text-xs font-semibold cursor-pointer transition-all flex items-center gap-1.5"
            :class="[
              activeFilter === 'flagged'
                ? 'bg-red-500/20 text-red-300 border-red-500/50 shadow-sm'
                : 'bg-white/5 text-muted-foreground border-white/10 hover:text-white',
            ]"
            @click="activeFilter = 'flagged'; loadItems()"
          >
            <AlertTriangle class="w-3.5 h-3.5 text-red-400" />
            <span>Signalées ({{ counts.flagged }})</span>
          </button>

          <button
            class="px-3 py-1.5 rounded-lg border text-xs font-semibold cursor-pointer transition-all flex items-center gap-1.5"
            :class="[
              activeFilter === 'pending'
                ? 'bg-blue-500/20 text-blue-300 border-blue-500/50 shadow-sm'
                : 'bg-white/5 text-muted-foreground border-white/10 hover:text-white',
            ]"
            @click="activeFilter = 'pending'; loadItems()"
          >
            <Clock class="w-3.5 h-3.5 text-blue-400" />
            <span>En attente ({{ counts.pending }})</span>
          </button>

          <button
            class="px-3 py-1.5 rounded-lg border text-xs font-semibold cursor-pointer transition-all flex items-center gap-1.5"
            :class="[
              activeFilter === 'approved'
                ? 'bg-emerald-500/20 text-emerald-300 border-emerald-500/50 shadow-sm'
                : 'bg-white/5 text-muted-foreground border-white/10 hover:text-white',
            ]"
            @click="activeFilter = 'approved'; loadItems()"
          >
            <CheckCircle2 class="w-3.5 h-3.5 text-emerald-400" />
            <span>Validées ({{ counts.approved }})</span>
          </button>

          <button
            class="px-3 py-1.5 rounded-lg border text-xs font-semibold cursor-pointer transition-all flex items-center gap-1.5"
            :class="[
              activeFilter === 'rejected'
                ? 'bg-rose-500/20 text-rose-300 border-rose-500/50 shadow-sm'
                : 'bg-white/5 text-muted-foreground border-white/10 hover:text-white',
            ]"
            @click="activeFilter = 'rejected'; loadItems()"
          >
            <XCircle class="w-3.5 h-3.5 text-rose-400" />
            <span>Rejetées ({{ counts.rejected }})</span>
          </button>

          <button
            class="px-3 py-1.5 rounded-lg border text-xs font-semibold cursor-pointer transition-all"
            :class="[
              activeFilter === 'all'
                ? 'bg-white/20 text-white border-white/40 shadow-sm'
                : 'bg-white/5 text-muted-foreground border-white/10 hover:text-white',
            ]"
            @click="activeFilter = 'all'; loadItems()"
          >
            Toutes
          </button>
        </div>
      </div>

      <!-- Barre de recherche -->
      <div class="relative max-w-md">
        <Search class="w-4 h-4 absolute left-3.5 top-1/2 -translate-y-1/2 text-muted-foreground" />
        <input
          v-model="searchQuery"
          type="text"
          placeholder="Rechercher par candidat, métier ou motif..."
          class="w-full h-10 pl-10 pr-4 bg-[#0e0e16] border border-white/15 rounded-xl text-xs text-white placeholder:text-muted-foreground focus:outline-none focus:border-blue-500 transition-colors"
        />
      </div>

      <!-- Liste des Vidéos de la File de Modération -->
      <div v-if="isLoading" class="p-12 text-center text-muted-foreground text-sm">
        Chargement de la file de modération...
      </div>

      <div v-else-if="filteredItems.length === 0" class="p-12 rounded-2xl bg-[#0e0e16] border border-white/10 text-center space-y-2">
        <ShieldAlert class="w-10 h-10 text-muted-foreground/40 mx-auto" />
        <p class="text-sm font-semibold text-white">Aucune vidéo à afficher dans cette catégorie.</p>
        <p class="text-xs text-muted-foreground">Toutes les vidéos signalées ou en attente ont été traitées.</p>
      </div>

      <div v-else class="space-y-4">
        <div
          v-for="item in filteredItems"
          :key="item.id"
          class="p-5 rounded-2xl bg-[#0e0e16] border transition-all"
          :class="[
            item.status === 'flagged'
              ? 'border-amber-500/40 bg-[#141210]'
              : item.status === 'pending'
                ? 'border-blue-500/30'
                : 'border-white/10',
          ]"
        >
          <div class="flex flex-col lg:flex-row gap-6 items-start">
            <!-- Lecteur Vidéo & Aperçu (Taille standard 220px 16:9 ou 9:16) -->
            <div class="relative w-full sm:w-64 aspect-video sm:aspect-4/3 bg-black rounded-xl overflow-hidden border border-white/15 shrink-0 group">
              <video
                :ref="(el) => setVideoRef(item.id, el)"
                :src="item.props.videoUrl"
                :poster="item.props.posterUrl"
                class="w-full h-full object-cover"
                playsinline
                preload="metadata"
                controls
              />

              <!-- Overlay bouton Play rapide si en pause -->
              <button
                v-if="activePlayingId !== item.id"
                class="absolute inset-0 bg-black/40 group-hover:bg-black/20 flex items-center justify-center transition-all cursor-pointer"
                @click="togglePlayVideo(item.id)"
                aria-label="Lire la vidéo"
              >
                <div class="w-12 h-12 rounded-full bg-[#000091]/90 border border-white/30 text-white flex items-center justify-center shadow-lg transition-transform group-hover:scale-110">
                  <Play class="w-5 h-5 ml-0.5 fill-current" />
                </div>
              </button>

              <div class="absolute bottom-2 right-2 px-2 py-0.5 rounded bg-black/80 text-white text-[10px] font-bold font-heading">
                {{ item.props.durationSeconds }}s
              </div>
            </div>

            <!-- Détails du Candidat & Signalement -->
            <div class="flex-1 min-w-0 space-y-3 w-full">
              <!-- Ligne Candidat & Statut -->
              <div class="flex flex-col sm:flex-row sm:items-center justify-between gap-2">
                <div class="flex items-center gap-3">
                  <img
                    :src="item.props.candidateAvatarUrl"
                    :alt="item.props.candidateName"
                    class="w-10 h-10 rounded-full object-cover border border-white/15 shrink-0"
                  />
                  <div>
                    <h3 class="font-bold text-sm sm:text-base text-white font-heading">
                      {{ item.props.candidateName }}
                    </h3>
                    <p class="text-xs text-muted-foreground">{{ item.props.candidateHeadline }}</p>
                  </div>
                </div>

                <!-- Badge de statut -->
                <div class="flex items-center gap-2">
                  <Badge
                    v-if="item.status === 'flagged'"
                    class="bg-amber-500/20 text-amber-300 border border-amber-500/40 text-xs font-bold gap-1"
                  >
                    <AlertTriangle class="w-3 h-3" />
                    <span>Signalée urgente</span>
                  </Badge>

                  <Badge
                    v-else-if="item.status === 'pending'"
                    class="bg-blue-500/20 text-blue-300 border border-blue-500/40 text-xs font-semibold gap-1"
                  >
                    <Clock class="w-3 h-3" />
                    <span>En attente initiale</span>
                  </Badge>

                  <Badge
                    v-else-if="item.status === 'approved'"
                    class="bg-emerald-500/20 text-emerald-300 border border-emerald-500/40 text-xs font-semibold gap-1"
                  >
                    <CheckCircle2 class="w-3 h-3" />
                    <span>Validée & Publiée</span>
                  </Badge>

                  <Badge
                    v-else-if="item.status === 'rejected'"
                    class="bg-rose-500/20 text-rose-300 border border-rose-500/40 text-xs font-semibold gap-1"
                  >
                    <XCircle class="w-3 h-3" />
                    <span>Rejetée</span>
                  </Badge>
                </div>
              </div>

              <!-- Bloc Motif de Signalement si applicable -->
              <div
                v-if="item.status === 'flagged' && item.props.reportReason"
                class="p-3.5 rounded-xl bg-amber-500/10 border border-amber-500/30 text-xs space-y-1.5"
              >
                <div class="flex items-center justify-between text-amber-300 font-semibold">
                  <span class="flex items-center gap-1.5">
                    <AlertTriangle class="w-3.5 h-3.5 shrink-0" />
                    <span>Motif du signalement :</span>
                  </span>
                  <span class="text-[10px] uppercase font-bold tracking-wider px-2 py-0.5 rounded bg-amber-500/20 border border-amber-500/30">
                    {{ item.props.reportSource === 'auto_ai' ? 'Contrôle IA Automatique' : 'Signalé par un recruteur' }}
                  </span>
                </div>
                <p class="text-amber-100/90 font-medium leading-relaxed">
                  « {{ item.props.reportReason }} »
                </p>
                <p v-if="item.props.reportedAt" class="text-[10px] text-amber-300/70">
                  Signalé le {{ new Date(item.props.reportedAt).toLocaleDateString('fr-FR', { hour: '2-digit', minute: '2-digit' }) }}
                </p>
              </div>

              <!-- Bloc Motif de Rejet si rejetée -->
              <div
                v-else-if="item.status === 'rejected' && item.props.rejectionReason"
                class="p-3.5 rounded-xl bg-rose-500/10 border border-rose-500/30 text-xs space-y-1"
              >
                <p class="font-bold text-rose-300">Motif de rejet : {{ item.props.rejectionReason }}</p>
                <p v-if="item.props.rejectionComment" class="text-rose-100/90 italic">
                  « {{ item.props.rejectionComment }} »
                </p>
              </div>

              <!-- Horodatage & Informations de soumission -->
              <div class="text-[11px] text-muted-foreground flex items-center gap-4">
                <span>Déposée le {{ new Date(item.props.submittedAt).toLocaleDateString('fr-FR') }}</span>
                <span v-if="item.props.reviewedAt">Traité le {{ new Date(item.props.reviewedAt).toLocaleDateString('fr-FR') }}</span>
              </div>

              <!-- Boutons d'Action Obligatoires : Valider la vidéo & Rejeter la vidéo -->
              <div class="pt-2 flex items-center gap-3 flex-wrap">
                <Button
                  v-if="item.status !== 'approved'"
                  variant="default"
                  size="sm"
                  class="bg-emerald-600 hover:bg-emerald-500 text-white text-xs font-bold gap-1.5 h-9 cursor-pointer"
                  @click="handleApprove(item)"
                >
                  <Check class="w-4 h-4" />
                  <span>Valider la vidéo</span>
                </Button>

                <Button
                  v-if="item.status !== 'rejected'"
                  variant="outline"
                  size="sm"
                  class="border-rose-500/40 text-rose-400 hover:bg-rose-500/10 text-xs font-semibold gap-1.5 h-9 cursor-pointer"
                  @click="openRejectModal(item)"
                >
                  <XCircle class="w-4 h-4" />
                  <span>Rejeter la vidéo</span>
                </Button>

                <Button
                  v-if="item.status === 'approved' || item.status === 'rejected'"
                  variant="ghost"
                  size="sm"
                  class="text-xs text-muted-foreground hover:text-white h-9 cursor-pointer"
                  @click="openRejectModal(item)"
                >
                  <span>Changer le statut / Modifier motif</span>
                </Button>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- Modale de Sélection du Motif de Rejet -->
    <Dialog :open="isRejectModalOpen" @update:open="(v) => isRejectModalOpen = v">
      <DialogContent class="bg-[#0e0e16] border border-white/15 text-white max-w-lg">
        <DialogHeader>
          <DialogTitle class="text-lg font-bold font-heading text-white flex items-center gap-2">
            <XCircle class="w-5 h-5 text-rose-400" />
            <span>Rejeter la vidéo candidate</span>
          </DialogTitle>
          <DialogDescription class="text-xs text-muted-foreground font-body">
            Veuillez sélectionner le motif institutionnel du rejet. Le candidat recevra une notification explicative l'invitant à soumettre une nouvelle prise.
          </DialogDescription>
        </DialogHeader>

        <div class="space-y-4 py-3 text-xs">
          <!-- Candidat ciblé -->
          <div v-if="itemToReject" class="p-3 rounded-xl bg-white/5 border border-white/10 flex items-center gap-3">
            <User class="w-4 h-4 text-blue-400" />
            <span class="font-semibold text-white">{{ itemToReject.props.candidateName }}</span>
            <span class="text-muted-foreground">({{ itemToReject.props.candidateHeadline }})</span>
          </div>

          <!-- Sélection du motif -->
          <div class="space-y-1.5">
            <label class="font-bold text-white text-xs">Motif principal de rejet :</label>
            <select
              v-model="selectedRejectReason"
              class="w-full h-10 px-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-rose-400"
            >
              <option v-for="r in REJECTION_REASONS" :key="r" :value="r">
                {{ r }}
              </option>
            </select>
          </div>

          <!-- Commentaire d'explication pédagogique pour le candidat -->
          <div class="space-y-1.5">
            <label class="font-bold text-white text-xs">
              Commentaire complémentaire (visible par le candidat) :
            </label>
            <textarea
              v-model="customRejectComment"
              rows="3"
              placeholder="Ex: Merci de réenregistrer votre vidéo dans un lieu calme sans musique de fond..."
              class="w-full p-3 bg-[#161622] border border-white/20 rounded-xl text-xs text-white focus:outline-none focus:border-rose-400 placeholder:text-muted-foreground"
            />
          </div>
        </div>

        <DialogFooter class="flex sm:justify-between items-center gap-2 pt-2 border-t border-white/10">
          <Button
            variant="ghost"
            size="sm"
            class="text-xs text-muted-foreground hover:text-white cursor-pointer"
            @click="isRejectModalOpen = false"
          >
            Annuler
          </Button>

          <Button
            variant="destructive"
            size="sm"
            class="bg-rose-600 hover:bg-rose-500 text-white text-xs font-bold h-9 cursor-pointer"
            @click="confirmReject"
          >
            Confirmer le rejet
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  </div>
</template>
