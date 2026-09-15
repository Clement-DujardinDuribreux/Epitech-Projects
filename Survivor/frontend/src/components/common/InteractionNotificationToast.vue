<script setup lang="ts">
import { ref, onMounted, onBeforeUnmount } from 'vue'
import { Bell, X, Mail } from 'lucide-vue-next'
import type { IInteractionNotification } from '@/core/services/InteractionNotificationService'

const activeNotification = ref<IInteractionNotification | null>(null)
let timer: ReturnType<typeof setTimeout> | null = null

function showNotification(notif: IInteractionNotification) {
  activeNotification.value = notif
  if (timer) clearTimeout(timer)
  timer = setTimeout(() => {
    activeNotification.value = null
  }, 6000)
}

function handleEvent(event: Event) {
  const customEvent = event as CustomEvent<IInteractionNotification>
  if (customEvent.detail) {
    showNotification(customEvent.detail)
  }
}

onMounted(() => {
  window.addEventListener('recruiter-interaction', handleEvent)
})

onBeforeUnmount(() => {
  window.removeEventListener('recruiter-interaction', handleEvent)
  if (timer) clearTimeout(timer)
})

function dismiss() {
  activeNotification.value = null
  if (timer) clearTimeout(timer)
}
</script>

<template>
  <Transition
    enter-active-class="transform ease-out duration-300 transition"
    enter-from-class="translate-y-2 opacity-0 sm:translate-y-0 sm:translate-x-2"
    enter-to-class="translate-y-0 opacity-100 sm:translate-x-0"
    leave-active-class="transition ease-in duration-100"
    leave-from-class="opacity-100"
    leave-to-class="opacity-0"
  >
    <aside
      v-if="activeNotification"
      class="fixed bottom-5 right-5 z-50 max-w-sm w-full bg-[#12121e] border border-[#000091]/40 rounded-xl shadow-2xl p-4 flex items-start gap-3 text-white backdrop-blur-lg"
      role="alert"
      aria-live="assertive"
    >
      <div class="w-9 h-9 rounded-lg bg-[#000091]/20 border border-[#000091]/30 flex items-center justify-center shrink-0 text-[#7999ff]">
        <Bell class="w-4 h-4 animate-bounce" />
      </div>

      <div class="flex-1 min-w-0 pr-1">
        <div class="flex items-center gap-1.5 mb-1">
          <span class="text-xs font-bold font-heading text-white">Nouvelle interaction recruteur</span>
          <span class="inline-flex items-center gap-1 text-[10px] text-emerald-400 bg-emerald-500/10 px-1.5 py-0.5 rounded border border-emerald-500/20">
            <Mail class="w-3 h-3" /> E-mail transmis
          </span>
        </div>
        <p class="text-xs text-white/80 font-body leading-snug">
          L'entreprise <strong class="text-white">{{ activeNotification.recruiterCompany }}</strong> s'intéresse au profil de <strong class="text-white">{{ activeNotification.candidateName }}</strong>.
        </p>
        <p v-if="activeNotification.message" class="text-[11px] text-white/50 italic mt-1 truncate font-body">
          « {{ activeNotification.message }} »
        </p>
      </div>

      <button
        type="button"
        class="text-white/50 hover:text-white transition-colors p-1 rounded-md cursor-pointer"
        @click="dismiss"
        aria-label="Fermer la notification"
      >
        <X class="w-4 h-4" />
      </button>
    </aside>
  </Transition>
</template>
