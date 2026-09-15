<script setup lang="ts">
import { Compass, Users, UserPlus, LogIn } from 'lucide-vue-next'
</script>

<template>
  <div class="container mx-auto py-16 px-4 text-center max-w-4xl">
    <div class="inline-flex items-center gap-2 px-3.5 py-1.5 rounded-full bg-[#000091]/10 text-[#000091] dark:text-[#90b8ff] text-xs font-heading font-semibold uppercase tracking-wider mb-6">
      <span>Plateforme Officielle JEB</span>
    </div>

    <h1 class="text-4xl sm:text-5xl font-extrabold mb-4 font-heading tracking-tight text-foreground">
      Bienvenue sur <span class="text-[#000091] dark:text-[#90b8ff]">PROFILSACTIFS</span>
    </h1>

    <p class="text-lg sm:text-xl text-muted-foreground mb-10 max-w-2xl mx-auto font-body leading-relaxed">
      La plateforme de recrutement vidéo et certification de compétences. Valorisez vos aptitudes réelles ou recrutez des talents vérifiés sans biais.
    </p>

    <!-- Boutons d'action -->
    <div class="flex flex-wrap items-center justify-center gap-4">
      <RouterLink
        to="/recruteur/dashboard"
        class="inline-flex items-center gap-2.5 bg-[#000091] hover:bg-[#1212ff] text-white px-6 py-3 rounded-xl font-heading font-semibold text-base shadow-md transition-all transform hover:scale-[1.02] active:scale-[0.98] cursor-pointer"
        title="Découvrir les profils candidats"
      >
        <Compass class="w-5 h-5" />
        <span>Découvrir les profils candidats</span>
      </RouterLink>

      <RouterLink
        to="/recruteur/dashboard"
        class="inline-flex items-center gap-2 bg-secondary hover:bg-muted text-secondary-foreground border border-border px-5 py-3 rounded-xl font-heading font-medium text-sm transition-colors cursor-pointer"
        title="Explorer le catalogue de profils"
      >
        <Users class="w-4 h-4 text-muted-foreground" />
        <span>Explorer le catalogue de profils</span>
      </RouterLink>

      <RouterLink
        to="/register"
        class="inline-flex items-center gap-2 bg-card hover:bg-muted text-foreground border border-border px-5 py-3 rounded-xl font-heading font-medium text-sm transition-colors cursor-pointer"
      >
        <UserPlus class="w-4 h-4 text-muted-foreground" />
        <span>Créer un compte</span>
      </RouterLink>

      <RouterLink
        to="/login"
        class="inline-flex items-center gap-2 text-muted-foreground hover:text-foreground hover:bg-muted/50 px-4 py-3 rounded-xl font-heading font-medium text-sm transition-colors cursor-pointer"
      >
        <LogIn class="w-4 h-4 text-muted-foreground" />
        <span>Se connecter</span>
      </RouterLink>
    </div>
  </div>
</template>
