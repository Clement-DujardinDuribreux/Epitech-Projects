<script setup lang="ts">
import { ShieldCheck } from 'lucide-vue-next'
import { Badge } from '@/components/ui/badge'

withDefaults(
  defineProps<{
    label?: string
    size?: 'sm' | 'base' | 'lg'
  }>(),
  {
    label: 'Certifié',
    size: 'base',
  },
)
</script>

<template>
  <Badge
    variant="jeb"
    class="flex items-center gap-1.5 uppercase"
    :class="{
      'px-2 py-0.5 text-[10px]': size === 'sm',
      'px-3 py-1 text-xs': size === 'base',
      'px-4 py-1.5 text-sm': size === 'lg',
    }"
  >
    <ShieldCheck class="w-3.5 h-3.5 text-[#f5c842]" />
    <span class="font-extrabold font-heading">{{ label }}</span>
  </Badge>
</template>

