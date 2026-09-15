<script setup lang="ts">
import { onMounted } from 'vue'
import { ArrowLeft } from 'lucide-vue-next'
import { RouterLink } from 'vue-router'
import { useCandidateProfileEditor } from '@/composables/useCandidateProfileEditor'
import CandidateProfileEditor from '@/components/dashboard/CandidateProfileEditor.vue'
import CandidateVideoSection from '@/components/dashboard/CandidateVideoSection.vue'

const { videoLink, profileInitialData, load, handleSaveProfile, handleSaveVideo } = useCandidateProfileEditor()

onMounted(() => {
  load()
})
</script>

<template>
  <div class="flex-1 bg-[#09090f] text-white min-h-screen py-8">
    <div class="w-full max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 space-y-6">
      <div class="flex items-center gap-4">
        <RouterLink
          to="/dashboard"
          class="inline-flex items-center gap-1.5 text-xs text-white/60 hover:text-white transition-colors bg-white/5 px-3 py-1.5 rounded-lg border border-white/10"
        >
          <ArrowLeft class="w-3.5 h-3.5" />
          Retour au tableau de bord
        </RouterLink>
      </div>

      <div>
        <h1 class="text-2xl font-bold font-heading">Édition du profil et pitch vidéo</h1>
        <p class="text-xs text-white/50 font-body mt-1">
          Mettez à jour vos informations professionnelles, coordonnées et votre vidéo de présentation.
        </p>
      </div>

      <div class="grid grid-cols-1 lg:grid-cols-2 gap-6">
        <CandidateProfileEditor :initial-data="profileInitialData" @save="handleSaveProfile" />
        <CandidateVideoSection :initial-url="videoLink" @save="handleSaveVideo" />
      </div>
    </div>
  </div>
</template>
