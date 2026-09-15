<script setup lang="ts">
import { Badge } from '@/components/ui/badge'

defineProps<{
  skills: string[]
}>()
</script>

<template>
  <section class="px-5 py-3 space-y-2.5">
    <h2 class="text-base font-bold text-white tracking-tight font-['Bricolage_Grotesque',sans-serif]">
      Compétences
    </h2>
    <div class="flex flex-wrap gap-2">
      <Badge
        v-for="skill in skills"
        :key="skill"
        variant="pill"
        class="border border-white/10 bg-[#1a1a1c] text-white/90 font-medium text-xs px-3.5 py-1.5 rounded-full hover:bg-white/10"
      >
        {{ skill }}
      </Badge>
    </div>
  </section>
</template>
