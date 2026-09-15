<script setup lang="ts">
import { type HTMLAttributes, computed } from 'vue'
import { AvatarImage, type AvatarImageProps } from 'radix-vue'
import { cn } from '@/lib/utils'

const props = defineProps<AvatarImageProps & { class?: HTMLAttributes['class'] }>()

const delegatedProps = computed(() => {
  const { class: _, ...delegated } = props
  return delegated
})
</script>

<template>
  <AvatarImage
    v-bind="delegatedProps"
    :class="cn('h-full w-full object-cover', props.class)"
  />
</template>
