<script setup lang="ts">
import { Home, Search, ShieldCheck, User } from 'lucide-vue-next'
</script>

<template>
  <nav
    class="w-full bg-[#0e0e10]/95 backdrop-blur-md border-t border-white/15 px-6 py-2 flex items-center justify-around text-white/80 text-[10px] font-heading"
    aria-label="Navigation mobile principale"
  >
    <RouterLink
      to="/"
      class="flex flex-col items-center gap-1 hover:text-white transition-colors"
      v-slot="{ isActive }"
    >
      <Home class="w-5 h-5" :class="{ 'text-[#90b8ff]': isActive }" />
      <span :class="{ 'text-[#90b8ff] font-bold': isActive }">Accueil</span>
    </RouterLink>

    <RouterLink
      to="/recruteur/dashboard"
      class="flex flex-col items-center gap-1 hover:text-white transition-colors"
      v-slot="{ isActive }"
    >
      <Search class="w-5 h-5" :class="{ 'text-[#90b8ff]': isActive }" />
      <span :class="{ 'text-[#90b8ff] font-bold': isActive }">Catalogue</span>
    </RouterLink>

    <RouterLink
      to="/certification"
      class="flex flex-col items-center gap-1 hover:text-white transition-colors"
      v-slot="{ isActive }"
    >
      <ShieldCheck class="w-5 h-5" :class="{ 'text-[#90b8ff]': isActive }" />
      <span :class="{ 'text-[#90b8ff] font-bold': isActive }">Certifié</span>
    </RouterLink>

    <RouterLink
      to="/dashboard"
      class="flex flex-col items-center gap-1 hover:text-white transition-colors"
      v-slot="{ isActive }"
    >
      <User class="w-5 h-5" :class="{ 'text-[#90b8ff]': isActive }" />
      <span :class="{ 'text-[#90b8ff] font-bold': isActive }">Mon Espace</span>
    </RouterLink>
  </nav>
</template>
