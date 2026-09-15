<script setup lang="ts">
import { ref, computed } from 'vue'
import { ArrowLeft, ShieldCheck, Check, Play, X } from 'lucide-vue-next'
import { CandidateProfileEntity } from '@/core/domain/entities/CandidateProfile.entity'
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'
import { useAuth } from '@/composables/useAuth'

defineProps<{
  candidate: CandidateProfileEntity
}>()

const auth = useAuth()
const isPlayingVideo = ref(false)

const backUrl = computed(() => {
  if (auth.isJobSeeker.value) {
    return '/dashboard'
  }
  return '/recruteur/dashboard'
})
</script>

<template>
  <div class="relative w-full overflow-hidden flex flex-col">
    <div class="relative w-full h-[400px] md:h-[440px] bg-black">
      <!-- Image Poster quand la vidéo n'est pas lancée -->
      <img
        v-if="!isPlayingVideo"
        :src="candidate.media.posterUrl"
        :alt="`Photo de ${candidate.fullName}`"
        class="w-full h-full object-cover"
      />

      <!-- Lecteur vidéo in-situ (Exigence 3.2 : Prévisionnement sans quitter le profil) -->
      <div v-else class="absolute inset-0 z-10 bg-black flex items-center justify-center">
        <video
          :src="candidate.media.videoUrl"
          controls
          autoplay
          playsinline
          class="w-full h-full object-contain"
          @ended="isPlayingVideo = false"
        >
          <track
            v-for="(track, index) in candidate.media.subtitles"
            :key="index"
            kind="subtitles"
            :src="track.src"
            :srclang="track.srcLang"
            :label="track.label"
            :default="track.default"
          />
          Votre navigateur ne prend pas en charge la lecture vidéo.
        </video>
        <button
          type="button"
          class="absolute top-4 right-16 z-30 flex items-center gap-1.5 px-3 py-1.5 rounded-full bg-black/70 hover:bg-black/90 text-white text-xs font-heading font-medium border border-white/20 backdrop-blur-md transition-colors cursor-pointer"
          @click="isPlayingVideo = false"
          aria-label="Fermer le prévisionnement vidéo"
        >
          <X class="w-3.5 h-3.5" />
          <span>Fermer la vidéo</span>
        </button>
      </div>

      <!-- Bouton interactif pour lancer le pitch vidéo directement -->
      <div v-if="!isPlayingVideo" class="absolute inset-0 flex items-center justify-center z-15 pointer-events-none">
        <button
          type="button"
          class="pointer-events-auto flex items-center gap-2.5 px-5 py-3 rounded-full bg-black/60 hover:bg-black/85 text-white font-heading font-semibold text-sm border border-white/25 backdrop-blur-md shadow-2xl transition-all transform hover:scale-105 active:scale-95 cursor-pointer group"
          @click="isPlayingVideo = true"
          aria-label="Lire la vidéo du candidat sans quitter la page"
        >
          <div class="w-9 h-9 rounded-full bg-[#000091] group-hover:bg-[#1212ff] flex items-center justify-center text-white shadow-md transition-colors">
            <Play class="w-4 h-4 fill-current ml-0.5" />
          </div>
          <span>Voir le pitch vidéo</span>
        </button>
      </div>

      <div
        v-if="!isPlayingVideo"
        class="absolute inset-0 bg-gradient-to-b from-black/50 via-transparent to-[#0e0e10] pointer-events-none"
      />

      <div class="absolute top-4 left-4 z-20">
        <RouterLink :to="backUrl">
          <Button
            size="icon"
            variant="ghost"
            class="w-10 h-10 rounded-full bg-black/50 backdrop-blur-md text-white hover:bg-black/70 border border-white/15 cursor-pointer"
            :aria-label="auth.isJobSeeker.value ? 'Retourner à mon espace' : 'Retourner au catalogue recruteur'"
          >
            <ArrowLeft class="w-5 h-5" />
          </Button>
        </RouterLink>
      </div>

      <div v-if="!isPlayingVideo" class="absolute bottom-4 left-5 right-5 flex items-end gap-4 z-20">
        <div class="relative">
          <Avatar size="xl" class="border-3 border-[#0e0e10] shadow-2xl">
            <AvatarImage :src="candidate.media.avatarUrl" :alt="candidate.fullName" />
            <AvatarFallback>{{ candidate.fullName.slice(0, 2) }}</AvatarFallback>
          </Avatar>
          <div
            v-if="candidate.isJebCertified"
            class="absolute -bottom-1 -right-1 w-6 h-6 rounded-full bg-[#0047ff] border-2 border-[#0e0e10] flex items-center justify-center text-white text-xs shadow-md"
            title="Certifié"
          >
            <Check class="w-3.5 h-3.5 stroke-[3]" />
          </div>
        </div>

        <div class="flex-1 pb-1">
          <h1 class="text-2xl md:text-3xl font-bold text-white tracking-tight font-heading leading-tight">
            {{ candidate.fullName }}<span v-if="candidate.age">, {{ candidate.age }} ans</span>
          </h1>
          <p class="text-sm text-white/90 font-body font-normal">
            {{ candidate.headline }}
          </p>
        </div>
      </div>
    </div>

    <div class="px-5 pt-3">
      <Badge
        v-if="candidate.isJebCertified"
        variant="jeb"
        class="text-xs px-3.5 py-1.5 gap-2 shadow-md font-heading"
      >
        <ShieldCheck class="w-4 h-4 text-[#f5c842]" />
        <span>Certification validé · Aptitudes certifiées</span>
      </Badge>
    </div>
  </div>
</template>
