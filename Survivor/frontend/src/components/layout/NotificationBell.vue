<script setup lang="ts">
import { ref, onMounted, onBeforeUnmount } from 'vue'
import { Bell } from 'lucide-vue-next'
import { useNotifications } from '@/composables/useNotifications'
import { Button } from '@/components/ui/button'

defineProps<{ collapsed?: boolean; alignRight?: boolean }>()

const { notifications, unreadCount, unreadPersonCount, markAllAsRead } = useNotifications()
const isOpen = ref(false)
const rootEl = ref<HTMLElement | null>(null)

function toggle(): void {
  isOpen.value = !isOpen.value
}

function onClickOutside(event: MouseEvent): void {
  if (rootEl.value && !rootEl.value.contains(event.target as Node)) {
    isOpen.value = false
  }
}

onMounted(() => document.addEventListener('click', onClickOutside))
onBeforeUnmount(() => document.removeEventListener('click', onClickOutside))
</script>

<template>
  <div ref="rootEl" class="relative">
    <Button
      variant="ghost"
      size="icon"
      class="h-8 w-8 rounded-lg text-muted-foreground hover:text-foreground hover:bg-muted/60 cursor-pointer relative"
      aria-label="Notifications"
      @click="toggle"
    >
      <Bell class="w-4 h-4" />
      <span
        v-if="unreadCount > 0"
        class="absolute -top-1 -right-1 min-w-4 h-4 px-1 rounded-full bg-red-500 text-white text-[9px] font-bold flex items-center justify-center"
      >
        {{ unreadCount }}
      </span>
    </Button>

    <div
      v-if="isOpen"
      class="absolute z-50 w-80 max-w-[calc(100vw-2rem)] max-h-96 overflow-y-auto rounded-xl bg-[#101018] border border-white/15 shadow-2xl py-2"
      :class="collapsed ? 'left-full ml-2 top-0' : alignRight ? 'right-0 top-full mt-2' : 'left-0 top-full mt-2'"
    >
      <div class="px-3.5 py-2 flex items-center justify-between border-b border-white/10">
        <p class="text-xs font-heading font-bold text-white">
          {{
            unreadCount > 0
              ? `${unreadCount} notification${unreadCount > 1 ? 's' : ''} de ${unreadPersonCount} personne${unreadPersonCount > 1 ? 's' : ''}`
              : 'Notifications'
          }}
        </p>
        <button
          v-if="unreadCount > 0"
          type="button"
          class="text-[10px] text-[#90b8ff] hover:underline font-body"
          @click="markAllAsRead"
        >
          Tout marquer comme lu
        </button>
      </div>

      <p v-if="notifications.length === 0" class="px-3.5 py-6 text-xs text-white/50 text-center font-body">
        Aucune notification
      </p>

      <div
        v-for="n in notifications"
        :key="n.id"
        class="flex items-start gap-2.5 px-3.5 py-2.5 hover:bg-white/5"
        :class="!n.read ? 'bg-[#000091]/10' : ''"
      >
        <img :src="n.personAvatarUrl" :alt="n.personName" class="w-8 h-8 rounded-full shrink-0 object-cover" />
        <div class="min-w-0">
          <p class="text-xs text-white font-body leading-snug">
            <span class="font-bold">{{ n.personName }}</span> {{ n.message }}
          </p>
          <p class="text-[10px] text-white/40 mt-0.5">{{ new Date(n.date).toLocaleDateString('fr-FR') }}</p>
        </div>
        <span v-if="!n.read" class="w-2 h-2 rounded-full bg-[#000091] shrink-0 mt-1 ml-auto" />
      </div>
    </div>
  </div>
</template>
