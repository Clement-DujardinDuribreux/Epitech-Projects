<script setup lang="ts">
// import { RouterLink } from 'vue-router'
// import { Button } from '@/components/ui/button'
import { FileQuestion /*, ArrowLeft */ } from 'lucide-vue-next'
</script>

<template>
  <div class="flex-1 flex flex-col justify-between min-h-[calc(100vh-80px)] p-6">
    <div class="flex-1 flex flex-col items-center justify-center text-center max-w-md mx-auto py-16">
      <div class="w-16 h-16 rounded-2xl bg-amber-500/10 border border-amber-500/20 flex items-center justify-center mb-6">
        <FileQuestion class="w-8 h-8 text-amber-400" />
      </div>

      <h1 class="text-4xl font-extrabold font-heading tracking-tight mb-3">404</h1>
      <h2 class="text-xl font-bold font-heading mb-2">Page introuvable</h2>
      <p class="text-sm text-muted-foreground font-body leading-relaxed mb-8">
        La page ou le profil que vous recherchez n'existe pas ou n'est plus disponible.
      </p>

      <RouterLink to="/">
        <Button class="gap-2 font-heading font-semibold cursor-pointer">
          <ArrowLeft class="w-4 h-4" />
          <span>Retourner à l'accueil</span>
        </Button>
      </RouterLink> -->
    </div>
  </div>
</template>

