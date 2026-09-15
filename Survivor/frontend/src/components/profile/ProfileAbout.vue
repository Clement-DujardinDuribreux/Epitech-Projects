<script setup lang="ts">
defineProps<{
  bio: string
}>()
</script>

<template>
  <section class="px-5 py-3 space-y-2">
    <h2 class="text-base font-bold text-white tracking-tight font-['Bricolage_Grotesque',sans-serif]">
      À propos
    </h2>
    <p class="text-sm text-white/75 leading-relaxed font-['Hanken_Grotesk',sans-serif]">
      {{ bio }}
    </p>
  </section>
</template>
