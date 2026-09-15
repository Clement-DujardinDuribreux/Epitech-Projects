<script setup lang="ts">
import { ref, computed, watch, onBeforeUnmount } from 'vue'
import { Video, Upload, Link as LinkIcon, AlertTriangle, CheckCircle2, XCircle } from 'lucide-vue-next'
import { Tabs, TabsList, TabsTrigger, TabsContent } from '@/components/ui/tabs'
import { Button } from '@/components/ui/button'
import { JobSeekerHttpRepository } from '@/core/infrastructure/repositories/JobSeekerHttpRepository'

const props = defineProps<{
  initialUrl?: string
  isValid?: boolean
  rejectionReason?: string | null
}>()

const emit = defineEmits<{
  (e: 'save', link: string): void
}>()

const jobSeekerRepo = new JobSeekerHttpRepository()

const videoMode = ref<'iframe' | 'upload'>('iframe')
const videoIframeUrl = ref(
  props.initialUrl || 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4'
)
const videoFile = ref<File | null>(null)
const videoFilePreview = ref<string | null>(null)
const videoUploadError = ref<string | null>(null)
const isSavedSuccess = ref(false)
const isUploading = ref(false)
const uploadSuccessMessage = ref<string | null>(null)

watch(
  () => props.initialUrl,
  (newUrl) => {
    if (newUrl) {
      videoIframeUrl.value = newUrl
    }
  }
)

// 100 Mo maximum selon la section 3.1 du Cahier des charges v1.0
const MAX_VIDEO_SIZE_BYTES = 100 * 1024 * 1024

function onVideoFileChange(event: Event): void {
  videoUploadError.value = null
  uploadSuccessMessage.value = null
  const input = event.target as HTMLInputElement
  const file = input.files?.[0] ?? null

  if (file && file.size > MAX_VIDEO_SIZE_BYTES) {
    const sizeMo = (file.size / (1024 * 1024)).toFixed(1)
    videoUploadError.value = `Le fichier sélectionné est trop volumineux (${sizeMo} Mo). La taille maximale autorisée est de 100 Mo conformément aux spécifications.`
    videoFile.value = null
    if (videoFilePreview.value) {
      URL.revokeObjectURL(videoFilePreview.value)
      videoFilePreview.value = null
    }
    input.value = ''
    return
  }

  if (videoFilePreview.value) URL.revokeObjectURL(videoFilePreview.value)
  videoFile.value = file
  videoFilePreview.value = file ? URL.createObjectURL(file) : null
}

onBeforeUnmount(() => {
  if (videoFilePreview.value) URL.revokeObjectURL(videoFilePreview.value)
})

const canPreviewIframe = computed(() => {
  return videoMode.value === 'iframe' && videoIframeUrl.value.trim().length > 0
})

const isDirectVideo = computed(() => {
  const url = videoIframeUrl.value.trim().toLowerCase()
  return url.endsWith('.mp4') || url.endsWith('.webm') || url.endsWith('.ogg')
})

function handleSaveVideo(): void {
  const link = videoIframeUrl.value.trim()
  if (!link) return
  emit('save', link)
  isSavedSuccess.value = true
  setTimeout(() => {
    isSavedSuccess.value = false
  }, 2500)
}

async function handleUploadVideo(): Promise<void> {
  if (!videoFile.value) return
  isUploading.value = true
  videoUploadError.value = null
  uploadSuccessMessage.value = null

  try {
    const res = await jobSeekerRepo.uploadVideo(videoFile.value)
    if (res && res.link) {
      videoIframeUrl.value = res.link
      emit('save', res.link)
      uploadSuccessMessage.value = 'Vidéo téléversée et enregistrée avec succès !'
      isSavedSuccess.value = true
      setTimeout(() => {
        isSavedSuccess.value = false
      }, 3500)
    }
  } catch (err: unknown) {
    videoUploadError.value =
      err instanceof Error ? err.message : "Erreur lors du téléversement de la vidéo."
  } finally {
    isUploading.value = false
  }
}

defineExpose({
  handleUploadVideo,
})
</script>

<template>
  <div class="rounded-xl bg-[#121218] border border-white/10 p-6 space-y-6">
    <div class="flex items-center justify-between border-b border-white/10 pb-4">
      <div>
        <h3 class="text-base font-bold font-heading text-white flex items-center gap-2">
          <Video class="w-4 h-4 text-[#00d4ff]" />
          Vidéo de présentation (Pitch candidat)
        </h3>
        <p class="text-xs text-white/50 font-body mt-0.5">
          Format recommandé : 30 à 60 secondes. Fichiers MP4 ou WebM — 100 Mo maximum.
        </p>
      </div>

      <div v-if="isSavedSuccess" class="flex items-center gap-1.5 text-xs text-emerald-400 font-medium font-heading">
        <CheckCircle2 class="w-4 h-4 text-emerald-400" />
        Vidéo enregistrée !
      </div>
    </div>

    <!-- Bannière de rejet visible par le candidat -->
    <div
      v-if="isValid === false && rejectionReason"
      class="p-4 rounded-xl bg-red-500/10 border border-red-500/30 space-y-2"
      role="alert"
    >
      <div class="flex items-center gap-2">
        <XCircle class="w-5 h-5 text-red-400 shrink-0" />
        <span class="text-sm font-bold text-red-400 font-heading">
          Votre vidéo a été refusée par l'équipe de modération
        </span>
      </div>
      <p class="text-xs text-red-300/90 font-body ml-7">
        <span class="font-semibold">Motif :</span> {{ rejectionReason }}
      </p>
      <p class="text-xs text-white/50 font-body ml-7">
        Veuillez soumettre une nouvelle vidéo conforme aux exigences ci-dessous.
      </p>
    </div>

    <Tabs v-model="videoMode">
      <TabsList class="bg-[#1a1a24] border border-white/10">
        <TabsTrigger value="iframe" class="gap-1.5 text-xs">
          <LinkIcon class="w-3.5 h-3.5" /> Lien externe / iframe
        </TabsTrigger>
        <TabsTrigger value="upload" class="gap-1.5 text-xs">
          <Upload class="w-3.5 h-3.5" /> Upload direct (&lt; 100 Mo)
        </TabsTrigger>
      </TabsList>

      <!-- Mode Lien / Iframe -->
      <TabsContent value="iframe" class="space-y-4 pt-2">
        <div class="space-y-1">
          <label class="text-xs font-medium text-white/70 font-body">URL de la vidéo ou iframe</label>
          <div class="flex gap-2">
            <input
              v-model="videoIframeUrl"
              type="url"
              class="flex-1 rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] font-body"
              placeholder="https://www.youtube.com/... ou https://vimeo.com/..."
            />
            <Button
              type="button"
              class="bg-[#1a6fff] hover:bg-[#155bd6] text-white text-xs px-4"
              @click="handleSaveVideo"
            >
              Enregistrer
            </Button>
          </div>
        </div>

        <div v-if="canPreviewIframe" class="space-y-2">
          <span class="text-xs text-white/50 font-body">Prévisualisation du lecteur :</span>
          <div class="aspect-video rounded-lg overflow-hidden border border-white/10 bg-black/60 relative">
            <video
              v-if="isDirectVideo"
              :src="videoIframeUrl"
              controls
              class="w-full h-full object-cover"
            />
            <iframe
              v-else
              :src="videoIframeUrl"
              class="w-full h-full"
              allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
              allowfullscreen
            />
          </div>
        </div>
      </TabsContent>

      <!-- Mode Upload direct avec contrôle strict de taille -->
      <TabsContent value="upload" class="space-y-4 pt-2">
        <label
          class="flex flex-col items-center justify-center gap-2.5 rounded-lg border-2 border-dashed border-white/20 py-10 text-white/60 text-sm cursor-pointer hover:border-[#00d4ff]/60 hover:bg-white/[0.02] transition-all"
        >
          <div class="w-12 h-12 rounded-full bg-white/5 flex items-center justify-center text-white/80">
            <Upload class="w-6 h-6 text-[#00d4ff]" />
          </div>
          <div class="text-center space-y-1">
            <span class="font-medium text-white block">
              {{ videoFile ? videoFile.name : 'Sélectionner un fichier vidéo' }}
            </span>
            <span class="text-xs text-white/50 block">Formats acceptés : MP4 ou WebM</span>
            <span class="text-[11px] font-semibold text-[#00d4ff] block">
              Contrôle strict : Limite maximale de 100 Mo
            </span>
          </div>
          <input
            type="file"
            accept="video/mp4,video/webm"
            class="hidden"
            @change="onVideoFileChange"
          />
        </label>

        <!-- Message d'erreur si fichier > 100 Mo -->
        <div
          v-if="videoUploadError"
          class="p-3.5 rounded-lg bg-red-500/15 border border-red-500/30 text-red-400 text-xs font-heading font-medium flex items-center gap-2.5"
          role="alert"
        >
          <AlertTriangle class="w-5 h-5 shrink-0 text-red-400" />
          <span>{{ videoUploadError }}</span>
        </div>

        <!-- Prévisualisation du fichier uploadé valide -->
        <div v-if="videoFilePreview" class="space-y-2">
          <div class="flex items-center gap-2 text-xs text-emerald-400 font-medium">
            <CheckCircle2 class="w-4 h-4" />
            Fichier valide (&lt; 100 Mo). Prévisualisation :
          </div>
          <div class="aspect-video rounded-lg overflow-hidden border border-white/10 bg-black">
            <video :src="videoFilePreview" controls class="w-full h-full object-cover" />
          </div>
        </div>

        <!-- Actions de validation / upload direct -->
        <div class="flex flex-col sm:flex-row items-stretch sm:items-center justify-between gap-3 pt-2">
          <div v-if="uploadSuccessMessage" class="flex items-center gap-2 text-xs text-emerald-400 font-medium font-heading">
            <CheckCircle2 class="w-4 h-4 text-emerald-400 shrink-0" />
            <span>{{ uploadSuccessMessage }}</span>
          </div>
          <div v-else-if="isSavedSuccess" class="flex items-center gap-2 text-xs text-emerald-400 font-medium font-heading">
            <CheckCircle2 class="w-4 h-4 text-emerald-400 shrink-0" />
            <span>Vidéo enregistrée avec succès !</span>
          </div>
          <div v-else />

          <Button
            type="button"
            :disabled="!videoFile || isUploading"
            class="bg-[#1a6fff] hover:bg-[#155bd6] text-white text-xs px-5 py-2 font-bold cursor-pointer disabled:opacity-50 gap-2 shrink-0 self-end"
            @click="handleUploadVideo"
          >
            <Upload v-if="!isUploading" class="w-4 h-4" />
            <div v-else class="w-4 h-4 border-2 border-white border-t-transparent rounded-full animate-spin" />
            <span>{{ isUploading ? 'Téléversement en cours...' : 'Enregistrer / Uploader la vidéo' }}</span>
          </Button>
        </div>
      </TabsContent>
    </Tabs>
  </div>
</template>
