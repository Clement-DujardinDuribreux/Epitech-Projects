<script lang="ts" setup>
import { ref } from 'vue'
import { Menu, X, LogIn, UserPlus } from 'lucide-vue-next'
import BrandLogo from '@/components/common/BrandLogo.vue'
import { Button } from '@/components/ui/button'

const isMobileMenuOpen = ref(false)

const navLinks = [
  { href: '/feed', label: 'Feed Vidéo' },
  { href: '/recruteur/dashboard', label: 'Catalogue Recruteur' },
  { href: '/profils/lea-d', label: 'Profils' },
  { href: '/recruteur/dashboard', label: 'Catalogue Recruteur' },
  { href: '/certification', label: 'Certification' },
  { href: '/recruteur/dashboard', label: 'Espace Recruteur' },
  { href: '/conformite', label: 'Pack Légal' },
]

function toggleMobileMenu() {
  isMobileMenuOpen.value = !isMobileMenuOpen.value
}
</script>

<template>
  <header
    class="w-[96%] sm:w-[94%] md:w-[85%] lg:max-w-screen-xl top-2 sm:top-3.5 mx-auto sticky border border-border z-40 rounded-2xl bg-card/95 backdrop-blur-md shadow-lg shrink-0 select-none"
  >
    <div class="flex justify-between items-center px-2.5 sm:px-6 py-1.5 sm:py-2.5">
      <div class="flex items-center shrink-0">
        <BrandLogo size="sm" :show-tagline="false" />
      </div>

      <!-- Liens Navigation Desktop / Tablette -->
      <nav class="hidden md:flex items-center gap-5 lg:gap-6">
        <RouterLink
          v-for="link in navLinks"
          :key="link.href"
          :to="link.href"
          class="text-xs lg:text-sm font-semibold font-heading text-muted-foreground hover:text-foreground transition-colors"
          active-class="text-[#000091] dark:text-[#90b8ff] font-bold"
        >
          {{ link.label }}
        </RouterLink>
      </nav>

      <!-- Actions Desktop -->
      <div class="hidden sm:flex items-center gap-2 shrink-0">
        <RouterLink to="/login">
          <Button variant="ghost" size="sm" class="font-semibold font-heading text-xs cursor-pointer">
            Connexion
          </Button>
        </RouterLink>
        <RouterLink to="/register">
          <Button variant="default" size="sm" class="font-semibold font-heading text-xs cursor-pointer">
            S'inscrire
          </Button>
        </RouterLink>
      </div>

      <!-- Actions Mobile (< 640px) -->
      <div class="flex sm:hidden items-center gap-1.5 shrink-0">
        <RouterLink to="/register">
          <Button variant="default" size="sm" class="h-8 px-2.5 text-xs font-semibold font-heading cursor-pointer">
            S'inscrire
          </Button>
        </RouterLink>
        <Button
          variant="outline"
          size="sm"
          class="h-8 w-8 p-0 rounded-lg text-foreground cursor-pointer"
          @click="toggleMobileMenu"
          aria-label="Ouvrir le menu de navigation"
        >
          <X v-if="isMobileMenuOpen" class="w-4 h-4" />
          <Menu v-else class="w-4 h-4" />
        </Button>
      </div>
    </div>

    <!-- Menu déroulant mobile animé -->
    <div
      v-if="isMobileMenuOpen"
      class="md:hidden border-t border-border px-3.5 py-3 space-y-2 bg-card rounded-b-2xl animate-in slide-in-from-top-2 duration-150 shadow-xl"
    >
      <nav class="flex flex-col space-y-1">
        <RouterLink
          v-for="link in navLinks"
          :key="link.href"
          :to="link.href"
          class="px-3 py-2 rounded-xl text-xs sm:text-sm font-semibold font-heading text-muted-foreground hover:text-foreground hover:bg-muted/40 transition-colors"
          active-class="bg-[#000091]/10 text-[#000091] dark:text-[#90b8ff] font-bold"
          @click="isMobileMenuOpen = false"
        >
          {{ link.label }}
        </RouterLink>
      </nav>

      <div class="pt-2 border-t border-border flex items-center gap-2">
        <RouterLink to="/login" class="flex-1" @click="isMobileMenuOpen = false">
          <Button variant="outline" size="sm" class="w-full h-8 text-xs font-semibold font-heading">
            <LogIn class="w-3.5 h-3.5 mr-1" />
            <span>Connexion</span>
          </Button>
        </RouterLink>
        <RouterLink to="/register" class="flex-1" @click="isMobileMenuOpen = false">
          <Button variant="default" size="sm" class="w-full h-8 text-xs font-semibold font-heading">
            <UserPlus class="w-3.5 h-3.5 mr-1" />
            <span>S'inscrire</span>
          </Button>
        </RouterLink>
      </div>
    </div>
  </header>
</template>
