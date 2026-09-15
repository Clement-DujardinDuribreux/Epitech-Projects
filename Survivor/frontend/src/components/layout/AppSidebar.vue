<script setup lang="ts">
import { computed, type Component } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import {
  Home,
  Compass,
  User,
  Video,
  Award,
  Bookmark,
  Clock,
  LayoutDashboard,
  ShieldAlert,
  Scale,
  PanelLeftClose,
  PanelLeftOpen,
  LogIn,
  UserPlus,
  LogOut,
} from 'lucide-vue-next'
import { useSidebarState } from '@/composables/useSidebarState'
import { useAuth } from '@/composables/useAuth'
import BrandLogo from '@/components/common/BrandLogo.vue'
import NotificationBell from '@/components/layout/NotificationBell.vue'
import { Button } from '@/components/ui/button'

const route = useRoute()
const router = useRouter()
const auth = useAuth()
const { isCollapsed, toggleSidebar } = useSidebarState()

interface NavItem {
  href?: string
  label: string
  icon: Component
  badge?: string
  action?: () => void
}

function handleLogout() {
  auth.logout()
  router.push('/login')
}

function isActive(href: string): boolean {
  if (href.includes('?')) {
    const [path, queryStr] = href.split('?')
    const params = new URLSearchParams(queryStr)
    const tab = params.get('tab')
    return route.path === path && route.query.tab === tab
  }
  if (href === '/') {
    return route.path === '/'
  }
  if (href === '/recruteur/dashboard') {
    return route.path === '/recruteur/dashboard' && (!route.query.tab || route.query.tab === 'catalogue')
  }
  return route.path === href || route.path.startsWith(href + '/')
}

const navItems = computed<NavItem[]>(() => {
  if (!auth.isAuthenticated.value) {
    return [
      { href: '/', label: 'Accueil', icon: Home },
      { href: '/recruteur/dashboard', label: 'Catalogue Candidats', icon: Compass },
      { href: '/login', label: 'Se connecter', icon: LogIn },
      { href: '/register', label: "S'inscrire", icon: UserPlus },
      { href: '/conformite', label: 'Conformité', icon: Scale },
    ]
  }

  if (auth.isJobSeeker.value) {
    return [
      { href: '/dashboard', label: 'Mon Espace & Interactions', icon: User },
      { href: '/dashboard/profile', label: 'Mon Profil & Pitch Vidéo', icon: Video },
      { href: '/certification', label: 'Certification JEB', icon: Award },
      { href: '/recruteur/dashboard', label: 'Catalogue Candidats', icon: Compass },
      { href: '/conformite', label: 'Conformité', icon: Scale },
      { label: 'Déconnexion', icon: LogOut, action: handleLogout },
    ]
  }

  if (auth.isRecruiter.value) {
    return [
      { href: '/recruteur/dashboard', label: 'Catalogue Candidats', icon: Compass },
      { href: '/recruteur/dashboard?tab=favoris', label: 'Mes Favoris', icon: Bookmark },
      { href: '/recruteur/dashboard?tab=suivi', label: 'Suivi des contacts', icon: Clock },
      { href: '/conformite', label: 'Conformité', icon: Scale },
      { label: 'Déconnexion', icon: LogOut, action: handleLogout },
    ]
  }

  if (auth.isAdmin.value) {
    return [
      { href: '/admin', label: 'Tableau de bord KPIs', icon: LayoutDashboard },
      { href: '/admin/moderation', label: 'Modération des vidéos', icon: ShieldAlert },
      { href: '/admin/certification', label: 'Gestion des Certifications', icon: Award },
      { href: '/recruteur/dashboard', label: 'Catalogue Candidats', icon: Compass },
      { href: '/conformite', label: 'Conformité', icon: Scale },
      { label: 'Déconnexion', icon: LogOut, action: handleLogout },
    ]
  }

  return [
    { href: '/', label: 'Accueil', icon: Home },
    { href: '/recruteur/dashboard', label: 'Catalogue Candidats', icon: Compass },
    { href: '/conformite', label: 'Conformité', icon: Scale },
    { label: 'Déconnexion', icon: LogOut, action: handleLogout },
  ]
})
</script>

<template>
  <aside
    class="h-screen sticky top-0 bg-card border-r border-border flex flex-col justify-between select-none transition-all duration-300 z-30 font-body shrink-0"
    :class="isCollapsed ? 'w-20 px-2.5 py-4' : 'w-72 px-3.5 py-4'"
    aria-label="Navigation principale"
  >
    <!-- Haut de la Sidebar : Logo & Bouton Toggle Collapse -->
    <div class="space-y-4">
      <div class="flex items-center justify-between gap-1.5" :class="isCollapsed ? 'flex-col justify-center gap-3' : ''">
        <!-- Logo en mode ouvert ou réduit -->
        <BrandLogo v-if="!isCollapsed" size="sm" :show-tagline="false" class="shrink-0" />
        <RouterLink
          v-else
          to="/"
          class="flex items-center justify-center p-2 rounded-xl bg-[var(--theme-bg-card,#0e0e16)] border border-white/10 hover:border-[#863bff]/50 shadow-sm transition-colors shrink-0"
          title="ProfilsActifs — Accueil"
        >
          <span class="text-white font-bold">P</span><span class="text-[#f5c842] font-bold">A</span>
        </RouterLink>

        <div class="flex items-center gap-1 shrink-0" :class="isCollapsed ? 'flex-col' : ''">
          <NotificationBell :collapsed="isCollapsed" />

          <!-- Bouton pour ouvrir / fermer la barre latérale -->
          <Button
            variant="ghost"
            size="icon"
            class="h-8 w-8 rounded-lg text-muted-foreground hover:text-foreground hover:bg-muted/60 cursor-pointer shrink-0"
            :title="isCollapsed ? 'Déplier le menu latéral' : 'Réduire le menu latéral'"
            @click="toggleSidebar"
            :aria-label="isCollapsed ? 'Déplier le menu' : 'Réduire le menu'"
          >
            <PanelLeftOpen v-if="isCollapsed" class="w-4 h-4" />
            <PanelLeftClose v-else class="w-4 h-4" />
          </Button>
        </div>
      </div>

      <!-- Navigation principale -->
      <nav class="space-y-1.5 pt-2" role="navigation" aria-label="Menu principal">
        <template v-for="item in navItems" :key="item.label">
          <!-- Action Button (ex: Déconnexion) -->
          <button
            v-if="item.action"
            type="button"
            class="flex items-center w-full rounded-xl font-heading text-xs sm:text-sm font-semibold transition-all group relative cursor-pointer"
            :class="[
              isCollapsed
                ? 'justify-center p-3 text-muted-foreground hover:text-destructive hover:bg-destructive/10'
                : 'gap-3.5 px-3.5 py-2.5 text-muted-foreground hover:text-destructive hover:bg-destructive/10',
            ]"
            :title="isCollapsed ? item.label : undefined"
            @click="item.action"
          >
            <component
              :is="item.icon"
              class="w-5 h-5 shrink-0 transition-transform group-hover:scale-110 text-muted-foreground group-hover:text-destructive"
            />

            <span v-if="!isCollapsed" class="truncate flex-1 text-left">
              {{ item.label }}
            </span>

            <!-- Tooltip flottant au survol en mode réduit -->
            <div
              v-if="isCollapsed"
              class="absolute left-full ml-3 px-2.5 py-1.5 bg-[#101018] text-white text-xs font-heading font-medium rounded-lg opacity-0 pointer-events-none group-hover:opacity-100 group-hover:pointer-events-auto transition-opacity shadow-xl z-50 whitespace-nowrap border border-white/15"
            >
              {{ item.label }}
            </div>
          </button>

          <!-- RouterLink classique -->
          <RouterLink
            v-else-if="item.href"
            :to="item.href"
            class="flex items-center rounded-xl font-heading text-xs sm:text-sm font-semibold transition-all group relative"
            :class="[
              isCollapsed
                ? 'justify-center p-3 text-muted-foreground hover:text-foreground hover:bg-muted/50'
                : 'gap-3.5 px-3.5 py-2.5 text-muted-foreground hover:text-foreground hover:bg-muted/50',
              isActive(item.href)
                ? 'bg-[#000091]/10 text-[#000091] dark:text-[#90b8ff] font-bold border-l-3 border-[#000091] shadow-sm'
                : '',
            ]"
            :title="isCollapsed ? item.label : undefined"
          >
            <component
              :is="item.icon"
              class="w-5 h-5 shrink-0 transition-transform group-hover:scale-110"
              :class="isActive(item.href) ? 'text-[#000091] dark:text-[#90b8ff]' : 'text-muted-foreground'"
            />

            <span v-if="!isCollapsed" class="truncate flex-1 text-left">
              {{ item.label }}
            </span>

            <span
              v-if="!isCollapsed && item.badge"
              class="px-1.5 py-0.5 rounded text-[9px] font-bold uppercase tracking-wider bg-[#000091]/15 text-[#000091] dark:text-[#90b8ff] border border-[#000091]/20 shrink-0"
            >
              {{ item.badge }}
            </span>

            <!-- Tooltip flottant au survol en mode réduit -->
            <div
              v-if="isCollapsed"
              class="absolute left-full ml-3 px-2.5 py-1.5 bg-[#101018] text-white text-xs font-heading font-medium rounded-lg opacity-0 pointer-events-none group-hover:opacity-100 group-hover:pointer-events-auto transition-opacity shadow-xl z-50 whitespace-nowrap border border-white/15"
            >
              {{ item.label }}
            </div>
          </RouterLink>
        </template>
      </nav>
    </div>

    <!-- Bas de la Sidebar : Authentification & Mentions légales -->
    <div class="space-y-4 pt-4 border-t border-border">
      <!-- Zone d'authentification pour les invités -->
      <div v-if="!auth.isAuthenticated.value">
        <div v-if="!isCollapsed" class="space-y-2">
          <RouterLink to="/login" class="block w-full">
            <Button
              variant="default"
              size="sm"
              class="w-full h-9 gap-2 font-bold font-heading text-xs shadow-sm cursor-pointer"
            >
              <LogIn class="w-4 h-4" />
              <span>Se connecter</span>
            </Button>
          </RouterLink>

          <RouterLink to="/register" class="block w-full">
            <Button
              variant="outline"
              size="sm"
              class="w-full h-9 gap-2 font-semibold font-heading text-xs text-muted-foreground hover:text-foreground cursor-pointer"
            >
              <UserPlus class="w-4 h-4" />
              <span>Créer un compte</span>
            </Button>
          </RouterLink>
        </div>

        <!-- Mode icônes réduites pour invité -->
        <div v-else class="flex flex-col items-center gap-2">
          <RouterLink to="/login">
            <Button
              variant="default"
              size="icon"
              class="w-10 h-10 rounded-xl shadow-sm cursor-pointer"
              title="Se connecter"
            >
              <LogIn class="w-4 h-4" />
            </Button>
          </RouterLink>
        </div>
      </div>

      <!-- Zone utilisateur connecté -->
      <div v-else>
        <div v-if="!isCollapsed" class="p-2.5 rounded-xl bg-muted/40 border border-border/50 flex items-center justify-between gap-2">
          <div class="truncate text-left">
            <p class="text-xs font-heading font-semibold text-foreground truncate">
              {{ auth.currentUser.value?.email || 'Utilisateur' }}
            </p>
            <p class="text-[10px] text-muted-foreground uppercase font-mono">
              {{ auth.currentUser.value?.role || 'Connecté' }}
            </p>
          </div>
          <Button
            variant="ghost"
            size="icon"
            class="h-8 w-8 text-muted-foreground hover:text-destructive hover:bg-destructive/10 shrink-0 cursor-pointer"
            title="Se déconnecter"
            @click="handleLogout"
          >
            <LogOut class="w-4 h-4" />
          </Button>
        </div>

        <div v-else class="flex justify-center">
          <Button
            variant="ghost"
            size="icon"
            class="w-10 h-10 rounded-xl text-muted-foreground hover:text-destructive hover:bg-destructive/10 cursor-pointer"
            title="Se déconnecter"
            @click="handleLogout"
          >
            <LogOut class="w-4 h-4" />
          </Button>
        </div>
      </div>

      <!-- Pied de page & avertissement légal (en mode étendu uniquement) -->
      <div v-if="!isCollapsed" class="text-[10px] text-muted-foreground/75 space-y-1.5 font-body leading-tight pt-1">
        <p class="font-bold font-heading text-foreground/80">ProfilsActifs</p>
        <p class="text-[9px] text-amber-400/80 leading-snug">
          Démonstrateur technique, ne constitue pas un service public en exploitation.
        </p>
      </div>
    </div>
  </aside>
</template>
