<script setup lang="ts">
import { Eye, Mail } from 'lucide-vue-next'
import { CandidateProfileEntity } from '@/core/domain/entities/CandidateProfile.entity'
import { Card } from '@/components/ui/card'

defineProps<{
  candidate: CandidateProfileEntity
}>()
</script>

<template>
  <div class="grid grid-cols-2 gap-3 px-5 py-4" aria-label="Statistiques professionnelles du candidat">
    <Card class="bg-[#1a1a1c] border-white/10 p-3.5 flex flex-col items-center justify-center text-center shadow-md">
      <Eye class="w-5 h-5 text-white/70 mb-1" />
      <span class="text-lg md:text-xl font-bold text-white font-['Bricolage_Grotesque',sans-serif]">
        {{ candidate.stats.format(candidate.stats.recruiterViewsCount) }}
      </span>
      <span class="text-[11px] text-white/50 font-medium">Vues recruteurs</span>
    </Card>

    <Card class="bg-[#1a1a1c] border-white/10 p-3.5 flex flex-col items-center justify-center text-center shadow-md">
      <Mail class="w-5 h-5 text-blue-400 mb-1" />
      <span class="text-lg md:text-xl font-bold text-white font-['Bricolage_Grotesque',sans-serif]">
        {{ candidate.stats.format(candidate.stats.solicitationsCount) }}
      </span>
      <span class="text-[11px] text-white/50 font-medium">Contacts</span>
    </Card>
  </div>
</template>
