<script setup lang="ts">
// import { RouterLink } from 'vue-router'
import { Button } from '@/components/ui/button'
import { ServerCrash, RotateCcw } from 'lucide-vue-next'

function handleReload() {
  window.location.reload()
}
</script>

<template>
  <div class="flex-1 flex flex-col justify-between min-h-[calc(100vh-80px)] p-6">
    <div class="flex-1 flex flex-col items-center justify-center text-center max-w-md mx-auto py-16">
      <div class="w-16 h-16 rounded-2xl bg-destructive/10 border border-destructive/20 flex items-center justify-center mb-6">
        <ServerCrash class="w-8 h-8 text-destructive" />
      </div>

      <h1 class="text-4xl font-extrabold font-heading tracking-tight mb-3">500</h1>
      <h2 class="text-xl font-bold font-heading mb-2">Erreur interne du serveur</h2>
      <p class="text-sm text-muted-foreground font-body leading-relaxed mb-8">
        Une anomalie temporaire est survenue. Nos équipes ont été alertées pour rétablir le service dans les plus brefs délais.
      </p>

      <div class="flex gap-3">
        <Button variant="outline" class="gap-2 font-heading font-semibold cursor-pointer" @click="handleReload">
          <RotateCcw class="w-4 h-4" />
          <span>Actualiser</span>
        </Button>
        <RouterLink to="/">
          <Button class="gap-2 font-heading font-semibold cursor-pointer">
            <span>Accueil</span>
          </Button>
        </RouterLink> -->
      </div>
    </div>
  </div>
</template>

