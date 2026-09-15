<script setup lang="ts">
import { computed } from 'vue'
import {
  ShieldCheck,
  MapPin,
  Eye,
  MessageSquare,
  Bookmark,
  Star,
} from 'lucide-vue-next'
import type { CandidateProfileEntity } from '@/core/domain/entities/CandidateProfile.entity'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'

const props = defineProps<{
  candidate: CandidateProfileEntity
  isFavorite: boolean
}>()

const emit = defineEmits<{
  (e: 'toggleFavorite', candidateId: string): void
  (e: 'openContact', candidate: CandidateProfileEntity): void
}>()

const durationLabel = computed(() => {
  const seconds = props.candidate.media.durationSeconds ?? 40
  return `${seconds}s`
})
</script>

<template>
  <article
    class="bg-card border border-border rounded-2xl overflow-hidden shadow-xs hover:border-[#000091]/50 hover:shadow-md transition-all flex flex-col justify-between group relative"
  >
    <!-- Vignette photo/vidéo & superpositions -->
    <div class="relative h-44 sm:h-48 bg-black overflow-hidden">
      <img
        :src="candidate.media.posterUrl"
        :alt="`Photo de ${candidate.fullName}`"
        class="w-full h-full object-cover group-hover:scale-105 transition-transform duration-300"
        loading="lazy"
      />
      <div class="absolute inset-0 bg-gradient-to-t from-black/90 via-black/30 to-transparent"></div>

      <!-- Badge JEB -->
      <div v-if="candidate.isJebCertified" class="absolute top-2.5 left-2.5">
        <Badge variant="jeb" class="text-[10px] gap-1 shadow-md bg-black/75 backdrop-blur-md border border-white/10">
          <ShieldCheck class="w-3 h-3 text-[#f5c842]" />
          <span>Certifié</span>
        </Badge>
      </div>

      <!-- Bouton Favori (Bookmark / Étoile) interactif -->
      <button
        type="button"
        class="absolute top-2.5 right-2.5 w-8 h-8 rounded-full flex items-center justify-center transition-all cursor-pointer shadow-md backdrop-blur-md"
        :class="
          isFavorite
            ? 'bg-amber-400 text-slate-900 ring-2 ring-amber-300/80 scale-105'
            : 'bg-black/60 text-white hover:bg-black/85 hover:text-amber-300 border border-white/15'
        "
        :title="isFavorite ? 'Retirer des favoris' : 'Ajouter aux favoris'"
        :aria-label="isFavorite ? `Retirer ${candidate.fullName} des favoris` : `Ajouter ${candidate.fullName} aux favoris`"
        @click.stop="emit('toggleFavorite', candidate.id)"
      >
        <Star v-if="isFavorite" class="w-4 h-4 fill-slate-900 text-slate-900" />
        <Bookmark v-else class="w-4 h-4" />
      </button>

      <!-- Durée vidéo -->
      <div class="absolute bottom-2.5 right-2.5 px-2 py-0.5 rounded bg-black/70 backdrop-blur-md text-[10px] font-mono text-white/90 border border-white/10">
        {{ durationLabel }}
      </div>

      <!-- Nom & Métier superposé sur la photo -->
      <div class="absolute bottom-2.5 left-2.5 right-14 text-left text-white">
        <h2 class="font-bold font-heading text-sm sm:text-base leading-tight truncate drop-shadow">
          {{ candidate.fullName }}
        </h2>
        <p class="text-[11px] text-white/80 font-body truncate drop-shadow">
          {{ candidate.headline }}
        </p>
      </div>
    </div>

    <!-- Contenu de la fiche candidat -->
    <div class="p-3.5 sm:p-4 flex-1 flex flex-col justify-between gap-3">
      <div class="space-y-2">
        <div class="flex items-center justify-between text-[11px] text-muted-foreground font-heading">
          <span class="flex items-center gap-1 truncate">
            <MapPin class="w-3.5 h-3.5 text-muted-foreground shrink-0" />
            <span class="truncate">{{ candidate.location }}</span>
          </span>
          <span v-if="candidate.age">{{ candidate.age }} ans</span>
        </div>

        <p class="text-xs text-muted-foreground font-body line-clamp-2 leading-relaxed">
          {{ candidate.bio }}
        </p>

        <!-- Tags compétences -->
        <div class="flex flex-wrap gap-1 pt-1">
          <Badge
            v-for="skill in candidate.skills.slice(0, 3)"
            :key="skill"
            variant="secondary"
            class="text-[10px] px-2 py-0.5 font-heading"
          >
            {{ skill }}
          </Badge>
          <span
            v-if="candidate.skills.length > 3"
            class="text-[10px] text-muted-foreground font-heading self-center px-1"
          >
            +{{ candidate.skills.length - 3 }}
          </span>
        </div>
      </div>

      <!-- Actions de contact et consultation -->
      <div class="pt-3 border-t border-border flex items-center gap-2">
        <RouterLink :to="`/profils/${candidate.id}`" class="flex-1">
          <Button
            variant="outline"
            size="sm"
            class="w-full h-9 gap-1.5 text-xs font-heading font-semibold cursor-pointer"
          >
            <Eye class="w-3.5 h-3.5" />
            <span>Fiche</span>
          </Button>
        </RouterLink>

        <Button
          variant="secondaryCta"
          size="sm"
          class="flex-1 h-9 gap-1.5 text-xs font-heading font-bold cursor-pointer"
          @click="emit('openContact', candidate)"
        >
          <MessageSquare class="w-3.5 h-3.5" />
          <span>Contacter</span>
        </Button>
      </div>
    </div>
  </article>
</template>
