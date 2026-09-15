<script setup lang="ts">
import { ref } from 'vue'
import { Menu, X, LogIn, UserPlus } from 'lucide-vue-next'
import AppSidebar from '@/components/layout/AppSidebar.vue'
import NotificationBell from '@/components/layout/NotificationBell.vue'
import AppFooter from '@/components/footer.vue'
import BrandLogo from '@/components/common/BrandLogo.vue'
import InteractionNotificationToast from '@/components/common/InteractionNotificationToast.vue'
import { Button } from '@/components/ui/button'

const isMobileMenuOpen = ref(false)

const mobileNavLinks = [
  { href: '/recruteur/dashboard', label: 'Catalogue Recruteur' },
  { href: '/certification', label: 'Certification' },
  { href: '/dashboard', label: 'Mon Espace' },
  { href: '/admin', label: 'Administration' },
]

function toggleMobileMenu() {
  isMobileMenuOpen.value = !isMobileMenuOpen.value
}
</script>

<template>
  <div class="h-full w-full flex flex-col md:flex-row bg-background text-foreground overflow-hidden font-body">
    <!-- Sidebar Latérale Desktop Rétractable -->
    <AppSidebar class="hidden md:flex" />

    <!-- En-tête Mobile (< 768px) -->
    <header class="flex md:hidden sticky top-0 z-40 w-full bg-card/95 backdrop-blur-md border-b border-border px-3 py-2 items-center justify-between shrink-0">
      <BrandLogo size="sm" :show-tagline="false" />
      <div class="flex items-center gap-1.5">
        <NotificationBell align-right />
        <RouterLink to="/register">
          <Button variant="default" size="sm" class="h-8 px-2.5 text-xs font-semibold font-heading">
            S'inscrire
          </Button>
        </RouterLink>
        <Button
          variant="outline"
          size="sm"
          class="h-8 w-8 p-0 rounded-lg text-foreground cursor-pointer"
          @click="toggleMobileMenu"
          aria-label="Menu principal"
        >
          <X v-if="isMobileMenuOpen" class="w-4 h-4" />
          <Menu v-else class="w-4 h-4" />
        </Button>
      </div>
    </header>

    <!-- Menu Tiroir Déroulant Mobile -->
    <div
      v-if="isMobileMenuOpen"
      class="md:hidden fixed top-[49px] inset-x-0 bottom-0 z-50 bg-background/95 backdrop-blur-xl border-b border-border p-4 space-y-4 animate-in slide-in-from-top-2 duration-150 overflow-y-auto"
    >
      <nav class="flex flex-col space-y-1.5">
        <RouterLink
          v-for="link in mobileNavLinks"
          :key="link.href"
          :to="link.href"
          class="px-4 py-3 rounded-xl text-sm font-semibold font-heading text-muted-foreground hover:text-foreground hover:bg-muted/50 transition-colors"
          active-class="bg-[#000091]/10 text-[#000091] dark:text-[#90b8ff] font-bold"
          @click="isMobileMenuOpen = false"
        >
          {{ link.label }}
        </RouterLink>
      </nav>

      <div class="pt-4 border-t border-border flex flex-col gap-2">
        <RouterLink to="/login" @click="isMobileMenuOpen = false">
          <Button variant="outline" size="sm" class="w-full h-10 text-xs font-semibold font-heading">
            <LogIn class="w-4 h-4 mr-1.5" />
            <span>Se connecter</span>
          </Button>
        </RouterLink>
        <RouterLink to="/register" @click="isMobileMenuOpen = false">
          <Button variant="default" size="sm" class="w-full h-10 text-xs font-semibold font-heading">
            <UserPlus class="w-4 h-4 mr-1.5" />
            <span>Créer un compte</span>
          </Button>
        </RouterLink>
      </div>

      <!-- Disclaimer légal dans le tiroir mobile -->
      <div class="pt-3 border-t border-border/60 text-[10px] text-amber-300/80 leading-snug">
        <p>Démonstrateur technique, ne constitue pas un service public en exploitation.</p>
      </div>
    </div>

    <!-- Zone de Contenu Principale Plein Écran -->
    <main class="flex-1 min-w-0 flex flex-col relative overflow-y-auto">
      <div class="flex-1 flex flex-col">
        <RouterView />
      </div>
      <AppFooter class="shrink-0" />
      <InteractionNotificationToast />
    </main>
  </div>
</template>
