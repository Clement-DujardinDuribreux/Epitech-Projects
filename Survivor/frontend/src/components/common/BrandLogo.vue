<script setup lang="ts">
withDefaults(
  defineProps<{
    size?: 'sm' | 'base' | 'lg'
    showTagline?: boolean
  }>(),
  {
    size: 'base',
    showTagline: true,
  },
)
</script>

<template>
  <RouterLink
    to="/"
    class="inline-flex items-center rounded-xl bg-[var(--theme-bg-card,#0e0e16)] border border-white/10 select-none group hover:border-[#863bff]/50 shadow-sm transition-colors"
    :class="{
      'p-1.5 gap-2': size === 'sm',
      'p-2 sm:p-2.5 gap-2.5 sm:gap-3': size === 'base',
      'p-3 gap-4': size === 'lg',
    }"
    title="ProfilsActifs — Plateforme de Recrutement"
    aria-label="ProfilsActifs — Retour à l'accueil"
  >
    <!-- Identité de la plateforme ProfilsActifs -->
    <div class="flex flex-col justify-center">
      <div
        class="font-black font-heading tracking-tight leading-none flex items-center whitespace-nowrap"
        :class="{
          'text-sm gap-1': size === 'sm',
          'text-lg sm:text-xl gap-1 sm:gap-1.5': size === 'base',
          'text-2xl gap-1.5': size === 'lg',
        }"
      >
        <span class="text-white">Profils</span><span class="text-[#f5c842]">Actifs</span>
      </div>
      <span
        v-if="showTagline"
        class="text-[8px] sm:text-[10px] text-white/70 tracking-wide font-heading uppercase font-semibold mt-0.5"
      >
        Recrutement Vidéo
      </span>
    </div>
  </RouterLink>
</template>
