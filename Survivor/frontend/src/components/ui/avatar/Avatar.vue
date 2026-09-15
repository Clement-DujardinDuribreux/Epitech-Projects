<script setup lang="ts">
import type { HTMLAttributes } from 'vue'
import { AvatarRoot } from 'radix-vue'
import { type AvatarVariants, avatarVariants } from './index'
import { cn } from '@/lib/utils'

interface Props {
  size?: AvatarVariants['size']
  shape?: AvatarVariants['shape']
  class?: HTMLAttributes['class']
}

const props = withDefaults(defineProps<Props>(), {
  size: 'base',
  shape: 'circle',
})
</script>

<template>
  <AvatarRoot :class="cn(avatarVariants({ size, shape }), props.class)">
    <slot />
  </AvatarRoot>
</template>
