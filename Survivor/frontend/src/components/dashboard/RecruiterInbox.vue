<script setup lang="ts">
import { ref, computed } from 'vue'
import {
  Mail,
  MailOpen,
  Archive,
  ArchiveRestore,
  Send,
  Building2,
  Calendar,
  CheckCircle2,
  MessageSquare,
} from 'lucide-vue-next'
import { Badge } from '@/components/ui/badge'
import { Button } from '@/components/ui/button'
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar'
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
} from '@/components/ui/dialog'
import type { RecruiterSolicitationEntity } from '@/core/domain/entities/RecruiterSolicitation.entity'

const props = defineProps<{
  solicitations: RecruiterSolicitationEntity[]
}>()

const emit = defineEmits<{
  (e: 'mark-read', id: string): void
  (e: 'mark-unread', id: string): void
  (e: 'archive', id: string): void
  (e: 'unarchive', id: string): void
  (e: 'reply', id: string, message: string): void
}>()

const activeTab = ref<'all' | 'unread' | 'archived'>('all')

const unreadCount = computed(() => {
  return props.solicitations.filter((s) => s.isUnread).length
})

const filteredSolicitations = computed(() => {
  if (activeTab.value === 'unread') {
    return props.solicitations.filter((s) => s.isUnread)
  }
  if (activeTab.value === 'archived') {
    return props.solicitations.filter((s) => s.isArchived)
  }
  // 'all': exclude archived from default inbox view, or show non-archived
  return props.solicitations.filter((s) => !s.isArchived)
})

// Modale de réponse
const isReplyModalOpen = ref(false)
const targetSolicitation = ref<RecruiterSolicitationEntity | null>(null)
const replyText = ref('')
const replySuccessMessage = ref<string | null>(null)

function openReplyModal(item: RecruiterSolicitationEntity): void {
  targetSolicitation.value = item
  replyText.value = `Bonjour ${item.props.recruiterName},\n\nMerci pour votre intérêt pour mon profil et ma vidéo de présentation. C'est avec plaisir que j'échangerai avec vous au sujet de cette opportunité chez ${item.props.company}.\n\nBien cordialement,\nAlex Martin`
  replySuccessMessage.value = null
  isReplyModalOpen.value = true
}

function handleSendReply(): void {
  if (!targetSolicitation.value || !replyText.value.trim()) return
  emit('reply', targetSolicitation.value.id, replyText.value)
  replySuccessMessage.value = `Votre réponse a été envoyée avec succès à ${targetSolicitation.value.props.recruiterName} (${targetSolicitation.value.props.company}) !`
  setTimeout(() => {
    isReplyModalOpen.value = false
    targetSolicitation.value = null
    replyText.value = ''
    replySuccessMessage.value = null
  }, 1600)
}
</script>

<template>
  <div class="space-y-6">
    <!-- En-tête et onglets de filtrage -->
    <div class="flex flex-col sm:flex-row sm:items-center justify-between gap-4">
      <div>
        <h2 class="text-xl font-bold font-heading text-white flex items-center gap-2">
          <MessageSquare class="w-5 h-5 text-[#1a6fff]" />
          Boîte de réception recruteurs
        </h2>
        <p class="text-xs text-white/50 font-body mt-0.5">
          Consultez et répondez aux opportunités transmises directement par les recruteurs
        </p>
      </div>

      <div class="flex items-center gap-1 bg-[#1a1a24] p-1 rounded-lg border border-white/10 self-start sm:self-auto">
        <button
          type="button"
          class="px-3 py-1.5 text-xs font-medium rounded-md transition-all flex items-center gap-1.5"
          :class="activeTab === 'all' ? 'bg-[#1a6fff] text-white shadow-sm' : 'text-white/60 hover:text-white'"
          @click="activeTab = 'all'"
        >
          Tous reçus
        </button>

        <button
          type="button"
          class="px-3 py-1.5 text-xs font-medium rounded-md transition-all flex items-center gap-1.5"
          :class="activeTab === 'unread' ? 'bg-[#1a6fff] text-white shadow-sm' : 'text-white/60 hover:text-white'"
          @click="activeTab = 'unread'"
        >
          Non lus
          <span
            v-if="unreadCount > 0"
            class="px-1.5 py-0.2 rounded-full text-[10px] font-bold bg-[#00d4ff] text-[#09090f]"
          >
            {{ unreadCount }}
          </span>
        </button>

        <button
          type="button"
          class="px-3 py-1.5 text-xs font-medium rounded-md transition-all flex items-center gap-1.5"
          :class="activeTab === 'archived' ? 'bg-[#1a6fff] text-white shadow-sm' : 'text-white/60 hover:text-white'"
          @click="activeTab = 'archived'"
        >
          <Archive class="w-3.5 h-3.5" />
          Archivés
        </button>
      </div>
    </div>

    <!-- Liste vide -->
    <div
      v-if="filteredSolicitations.length === 0"
      class="rounded-xl bg-[#121218] border border-white/10 p-12 text-center space-y-3"
    >
      <div class="w-12 h-12 rounded-full bg-white/5 flex items-center justify-center text-white/40 mx-auto">
        <MailOpen class="w-6 h-6" />
      </div>
      <h3 class="text-base font-semibold text-white font-heading">Aucun message dans cette section</h3>
      <p class="text-xs text-white/50 max-w-sm mx-auto font-body">
        Vos sollicitations reçues par les entreprises s'afficheront directement ici.
      </p>
    </div>

    <!-- Liste des sollicitations -->
    <div v-else class="space-y-4">
      <div
        v-for="item in filteredSolicitations"
        :key="item.id"
        class="rounded-xl border p-5 sm:p-6 transition-all duration-200 relative overflow-hidden"
        :class="[
          item.isUnread
            ? 'bg-[#14141e] border-[#1a6fff]/40 shadow-[0_0_20px_rgba(26,111,255,0.08)]'
            : 'bg-[#121218] border-white/10 hover:border-white/20',
        ]"
      >
        <!-- Ligne d'accentuation visuelle pour les non-lus -->
        <div
          v-if="item.isUnread"
          class="absolute left-0 top-0 bottom-0 w-1 bg-gradient-to-b from-[#1a6fff] to-[#00d4ff]"
        />

        <div class="flex flex-col md:flex-row md:items-start justify-between gap-4">
          <!-- Infos recruteur -->
          <div class="flex items-start gap-3.5">
            <Avatar size="lg">
              <AvatarImage v-if="item.props.avatarUrl" :src="item.props.avatarUrl" :alt="item.props.recruiterName" />
              <AvatarFallback class="bg-[#1a1a24] text-[#00d4ff] font-heading font-semibold">
                {{ item.props.recruiterName.slice(0, 2).toUpperCase() }}
              </AvatarFallback>
            </Avatar>

            <div class="space-y-1">
              <div class="flex flex-wrap items-center gap-2">
                <span class="font-bold text-base text-white font-heading">
                  {{ item.props.recruiterName }}
                </span>

                <span class="text-xs text-white/40">·</span>

                <span class="text-sm font-semibold text-[#00d4ff] flex items-center gap-1 font-heading">
                  <Building2 class="w-3.5 h-3.5" />
                  {{ item.props.company }}
                </span>

                <!-- Badge Statut -->
                <Badge
                  v-if="item.isUnread"
                  class="bg-[#1a6fff]/20 text-[#00d4ff] border border-[#1a6fff]/40 text-[11px] font-semibold"
                >
                  Non lu
                </Badge>
                <Badge
                  v-else-if="!item.isArchived"
                  variant="outline"
                  class="bg-white/5 text-white/60 border-white/10 text-[11px]"
                >
                  Lu
                </Badge>
                <Badge
                  v-else
                  variant="secondary"
                  class="bg-white/5 text-white/40 border-white/10 text-[11px]"
                >
                  Archivé
                </Badge>
              </div>

              <div class="flex items-center gap-2 text-xs text-white/50 font-body">
                <span v-if="item.props.recruiterRole">{{ item.props.recruiterRole }}</span>
                <span v-if="item.props.recruiterRole">·</span>
                <span class="flex items-center gap-1 text-white/40">
                  <Calendar class="w-3 h-3" />
                  {{ item.props.date }}
                </span>
              </div>
            </div>
          </div>

          <!-- Boutons d'action -->
          <div class="flex items-center gap-2 flex-wrap self-end md:self-start shrink-0">
            <!-- Bouton Marquer comme lu / non lu -->
            <Button
              v-if="item.isUnread"
              variant="outline"
              size="sm"
              class="border-white/10 hover:bg-white/10 text-xs gap-1.5 h-8"
              @click="emit('mark-read', item.id)"
            >
              <MailOpen class="w-3.5 h-3.5 text-white/70" />
              Marquer comme lu
            </Button>
            <Button
              v-else
              variant="ghost"
              size="sm"
              class="text-white/40 hover:text-white hover:bg-white/5 text-xs gap-1.5 h-8"
              @click="emit('mark-unread', item.id)"
            >
              <Mail class="w-3.5 h-3.5" />
              Marquer non lu
            </Button>

            <!-- Bouton Répondre -->
            <Button
              variant="default"
              size="sm"
              class="bg-[#1a6fff] hover:bg-[#155bd6] text-white text-xs gap-1.5 h-8 font-medium shadow-sm"
              @click="openReplyModal(item)"
            >
              <Send class="w-3.5 h-3.5" />
              Répondre
            </Button>

            <!-- Bouton Archiver / Restaurer -->
            <Button
              v-if="!item.isArchived"
              variant="ghost"
              size="sm"
              class="text-white/40 hover:text-white hover:bg-white/5 text-xs gap-1.5 h-8"
              @click="emit('archive', item.id)"
              title="Archiver ce message"
            >
              <Archive class="w-3.5 h-3.5" />
              Archiver
            </Button>
            <Button
              v-else
              variant="ghost"
              size="sm"
              class="text-white/60 hover:text-white hover:bg-white/5 text-xs gap-1.5 h-8"
              @click="emit('unarchive', item.id)"
              title="Désarchiver ce message"
            >
              <ArchiveRestore class="w-3.5 h-3.5" />
              Restaurer
            </Button>
          </div>
        </div>

        <!-- Objet et Message complet -->
        <div class="mt-4 pt-3 border-t border-white/5 space-y-2">
          <h4 class="text-sm font-semibold text-white/90 font-heading">
            {{ item.props.subject }}
          </h4>
          <p class="text-sm text-white/80 font-body leading-relaxed whitespace-pre-line bg-[#0e0e16]/60 p-3.5 rounded-lg border border-white/5">
            {{ item.props.message }}
          </p>
        </div>

        <!-- Réponse déjà envoyée si existante -->
        <div
          v-if="item.replyContent"
          class="mt-3 p-3 rounded-lg bg-emerald-500/10 border border-emerald-500/20 text-xs font-body text-emerald-300 space-y-1"
        >
          <div class="flex items-center gap-1.5 font-semibold font-heading">
            <CheckCircle2 class="w-3.5 h-3.5 text-emerald-400" />
            Votre réponse envoyée au recruteur :
          </div>
          <p class="text-white/80 italic pl-5 whitespace-pre-line">
            "{{ item.replyContent }}"
          </p>
        </div>
      </div>
    </div>

    <!-- Modale de réponse au recruteur -->
    <Dialog :open="isReplyModalOpen" @update:open="(val) => (isReplyModalOpen = val)">
      <DialogContent class="bg-[#121218] border border-white/15 text-white max-w-lg">
        <DialogHeader>
          <DialogTitle class="text-lg font-bold font-heading text-white flex items-center gap-2">
            <Send class="w-4 h-4 text-[#1a6fff]" />
            Répondre à {{ targetSolicitation?.props.recruiterName }}
          </DialogTitle>
          <DialogDescription class="text-xs text-white/60 font-body">
            Entreprise : <span class="text-white font-medium">{{ targetSolicitation?.props.company }}</span>
            — Objet : {{ targetSolicitation?.props.subject }}
          </DialogDescription>
        </DialogHeader>

        <div class="space-y-3 py-2">
          <!-- Rappel court du message initial -->
          <div class="p-3 rounded-md bg-[#1a1a24] border border-white/10 text-xs text-white/60 font-body line-clamp-3">
            <span class="font-semibold text-white/80">Message initial :</span> {{ targetSolicitation?.props.message }}
          </div>

          <div class="space-y-1">
            <label class="text-xs font-medium text-white/70 font-body">Votre message de réponse</label>
            <textarea
              v-model="replyText"
              rows="6"
              class="w-full rounded-md bg-[#1a1a24] border border-white/15 p-3 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] resize-none font-body leading-relaxed"
              placeholder="Écrivez votre réponse ici..."
            />
          </div>

          <div
            v-if="replySuccessMessage"
            class="p-3 rounded-md bg-emerald-500/15 border border-emerald-500/30 text-emerald-400 text-xs font-heading font-medium flex items-center gap-2"
          >
            <CheckCircle2 class="w-4 h-4 shrink-0" />
            <span>{{ replySuccessMessage }}</span>
          </div>
        </div>

        <DialogFooter class="flex justify-end gap-2">
          <Button
            type="button"
            variant="ghost"
            class="text-white/60 hover:text-white"
            @click="isReplyModalOpen = false"
          >
            Annuler
          </Button>
          <Button
            type="button"
            class="bg-[#1a6fff] hover:bg-[#155bd6] text-white gap-1.5"
            :disabled="!replyText.trim() || !!replySuccessMessage"
            @click="handleSendReply"
          >
            <Send class="w-4 h-4" />
            Envoyer la réponse
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  </div>
</template>
