<script setup lang="ts">
import { ref } from 'vue'
import { Monitor, Smartphone, Tablet, ChevronUp, ChevronDown, Sparkles } from 'lucide-vue-next'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'
import type { DeviceFormat } from '@/composables/useDevicePreview'

defineProps<{
  activeFormat: DeviceFormat
}>()

const emit = defineEmits<{
  (e: 'update:format', format: DeviceFormat): void
}>()

const isExpanded = ref<boolean>(false)
</script>

<template>
  <aside
    class="fixed bottom-4 right-4 z-50 hidden sm:flex flex-col items-end gap-2 font-sans select-none"
    aria-label="Barre d'outils de prévisualisation responsive"
  >
    <div
      v-if="isExpanded"
      class="bg-[#101018]/95 border border-white/20 text-white rounded-2xl p-4 shadow-2xl backdrop-blur-xl flex flex-col gap-3 min-w-[280px] animate-in slide-in-from-bottom-3 duration-200"
    >
      <div class="flex items-center justify-between border-b border-white/10 pb-2">
        <div class="flex items-center gap-1.5 font-bold text-xs font-heading">
          <Sparkles class="w-4 h-4 text-[#00d4ff]" />
          <span>Outils de Prévisualisation</span>
        </div>
        <Badge variant="tech" class="text-[10px] py-0 px-2">Dev Tools</Badge>
      </div>

      <div class="space-y-1.5">
        <span class="text-[11px] text-white/60 font-semibold uppercase tracking-wider font-heading">Format d'écran :</span>
        <div class="grid grid-cols-3 gap-1 bg-white/5 p-1 rounded-xl border border-white/10">
          <button
            class="flex items-center justify-center gap-1.5 py-1.5 px-2 rounded-lg text-xs font-semibold font-heading transition-all cursor-pointer"
            :class="activeFormat === 'mobile' ? 'bg-[#000091] text-white shadow' : 'text-white/70 hover:text-white'"
            @click="emit('update:format', 'mobile')"
            title="Mobile 390px (iPhone)"
          >
            <Smartphone class="w-3.5 h-3.5" />
            <span>390px</span>
          </button>
          <button
            class="flex items-center justify-center gap-1.5 py-1.5 px-2 rounded-lg text-xs font-semibold font-heading transition-all cursor-pointer"
            :class="activeFormat === 'tablet' ? 'bg-[#000091] text-white shadow' : 'text-white/70 hover:text-white'"
            @click="emit('update:format', 'tablet')"
            title="Tablette 768px (iPad)"
          >
            <Tablet class="w-3.5 h-3.5" />
            <span>768px</span>
          </button>
          <button
            class="flex items-center justify-center gap-1.5 py-1.5 px-2 rounded-lg text-xs font-semibold font-heading transition-all cursor-pointer"
            :class="activeFormat === 'desktop' ? 'bg-[#000091] text-white shadow' : 'text-white/70 hover:text-white'"
            @click="emit('update:format', 'desktop')"
            title="Desktop 100%"
          >
            <Monitor class="w-3.5 h-3.5" />
            <span>Desktop</span>
          </button>
        </div>
      </div>

      <div class="space-y-1">
        <span class="text-[11px] text-white/60 font-semibold uppercase tracking-wider font-heading">Thème Actif :</span>
        <div class="flex items-center gap-2 p-2 rounded-xl border border-primary/40 bg-primary/20 text-white text-xs font-heading">
          <span class="text-sm">✨</span>
          <span class="font-bold">Dark Neon & Or Sémantique</span>
        </div>
      </div>

      <div class="pt-2 border-t border-white/10 flex items-center justify-between text-xs font-heading">
        <!-- <RouterLink to="/conformite" class="flex items-center gap-1 text-[#00d4ff] hover:underline font-semibold">
          <Shield class="w-3.5 h-3.5" />
          <span>Pack Légal & RGPD</span>
        </RouterLink> -->
        <RouterLink to="/recruteur/dashboard" class="flex items-center gap-1 text-[#00d4ff] hover:underline font-semibold">
          <span>Catalogue</span>
        </RouterLink>
        <RouterLink to="/profils/lea-d" class="text-white/70 hover:text-white hover:underline">
          Profil Candidat →
        </RouterLink>
      </div>
    </div>

    <Button
      size="sm"
      variant="ghost"
      class="h-8 w-8 p-0 sm:h-9 sm:w-auto sm:px-3.5 rounded-full bg-[#101018]/90 border border-white/20 text-white hover:bg-[#1c1c28] shadow-2xl backdrop-blur-md gap-2 font-bold font-heading text-xs cursor-pointer flex items-center justify-center shrink-0"
      @click="isExpanded = !isExpanded"
      aria-label="Ouvrir le panneau Dev Tools"
      title="Outils de prévisualisation responsive"
    >
      <Sparkles class="w-3.5 h-3.5 text-[#00d4ff]" />
      <span class="hidden sm:inline">Format d'écran Dev</span>
      <ChevronDown v-if="isExpanded" class="w-3.5 h-3.5 hidden sm:inline" />
      <ChevronUp v-else class="w-3.5 h-3.5 hidden sm:inline" />
    </Button>
  </aside>
</template>
