import { createRouter, createWebHistory, type RouteRecordRaw } from 'vue-router'
import { useAuth } from '@/composables/useAuth'

const routes: Array<RouteRecordRaw> = [
  // Accès public & Authentification
  {
    path: '/',
    name: 'Home',
    component: () => import('@/views/HomeView.vue'),
    meta: {
      title: 'Accueil — ProfilsActifs',
    },
  },
  {
    path: '/feed',
    redirect: '/recruteur/dashboard',
  },
  {
    path: '/recruteur/contacts',
    redirect: '/recruteur/dashboard?tab=suivi',
  },
  {
    path: '/profils/:id',
    name: 'ProfileDetail',
    component: () => import('@/views/ProfileDetailView.vue'),
    meta: {
      title: 'Profil Candidat — ProfilsActifs',
    },
  },
  {
    path: '/login',
    name: 'Login',
    component: () => import('@/views/LoginView.vue'),
    meta: {
      title: 'Connexion — ProfilsActifs',
      guestOnly: true,
    },
  },
  {
    path: '/register',
    name: 'Register',
    component: () => import('@/views/RegisterView.vue'),
    meta: {
      title: 'Inscription — ProfilsActifs',
      guestOnly: true,
    },
  },
  {
    path: '/conformite',
    name: 'LegalCompliance',
    component: () => import('@/views/LegalComplianceView.vue'),
    meta: {
      title: 'Conformité & Pack Légal — ProfilsActifs',
    },
  },
  {
    path: '/cgu',
    name: 'CGU',
    component: () => import('@/views/CGUView.vue'),
    meta: {
      title: 'Conditions Générales d’Utilisation — ProfilsActifs',
    },
  },

  // Espace Demandeur d'emploi (réservé aux candidats)
  {
    path: '/dashboard',
    name: 'Dashboard',
    component: () => import('@/views/dashboard/DashboardView.vue'),
    meta: {
      title: 'Tableau de bord — ProfilsActifs',
      requiresAuth: true,
      role: 'job_seeker',
    },
  },
  {
    path: '/dashboard/profile',
    name: 'ProfileEdit',
    component: () => import('@/views/dashboard/ProfileEditView.vue'),
    meta: {
      title: 'Édition du profil — ProfilsActifs',
      requiresAuth: true,
      role: 'job_seeker',
    },
  },
  {
    path: '/certification',
    name: 'Certification',
    component: () => import('@/views/CertificationView.vue'),
    meta: {
      title: 'Questionnaire de Certification — ProfilsActifs',
      requiresAuth: true,
      role: 'job_seeker',
    },
  },

  // Espace Recruteur & Catalogue
  {
    path: '/recruteur/dashboard',
    name: 'RecruteurDashboard',
    component: () => import('@/views/recruteur/RecruteurDashboardView.vue'),
    meta: {
      title: 'Catalogue Candidats — ProfilsActifs',
      requiresAuth: false,
    },
  },
  {
    path: '/recruteur/contact/:candidatId',
    name: 'RecruteurContact',
    component: () => import('@/views/recruteur/RecruteurContactView.vue'),
    meta: {
      title: 'Contacter le candidat — ProfilsActifs',
      requiresAuth: true,
      role: 'recruiter',
    },
  },

  // Espace Administration (réservé aux administrateurs)
  {
    path: '/admin',
    name: 'AdminDashboard',
    component: () => import('@/views/admin/AdminDashboardView.vue'),
    meta: {
      title: 'Administration — ProfilsActifs',
      requiresAuth: true,
      role: 'admin',
    },
  },
  {
    path: '/admin/moderation',
    name: 'AdminModeration',
    component: () => import('@/views/admin/AdminModerationView.vue'),
    meta: {
      title: 'Modération — ProfilsActifs',
      requiresAuth: true,
      role: 'admin',
    },
  },
  {
    path: '/admin/certification',
    name: 'AdminCertification',
    component: () => import('@/views/admin/AdminCertificationView.vue'),
    meta: {
      title: 'Gestion des Certifications — ProfilsActifs',
      requiresAuth: true,
      role: 'admin',
    },
  },

  // Erreurs
  {
    path: '/500',
    name: 'ServerError',
    component: () => import('@/views/errors/ServerErrorView.vue'),
    meta: {
      title: 'Erreur 500 — ProfilsActifs',
    },
  },
  {
    path: '/:pathMatch(.*)*',
    name: 'NotFound',
    component: () => import('@/views/errors/NotFoundView.vue'),
    meta: {
      title: 'Page introuvable (404) — ProfilsActifs',
    },
  },
]

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes,
})

router.beforeEach(async (to) => {
  const auth = useAuth()
  await auth.initAuth()

  const isGuestOnly = Boolean(to.meta?.guestOnly)
  const requiresAuth = Boolean(to.meta?.requiresAuth)
  const requiredRole = to.meta?.role as string | undefined

  // Redirection des utilisateurs déjà connectés accédant à /login ou /register
  if (isGuestOnly && auth.isAuthenticated.value) {
    if (auth.isRecruiter.value) return '/recruteur/dashboard'
    if (auth.isAdmin.value) return '/admin'
    return '/dashboard'
  }

  // Vérification de l'authentification
  if (requiresAuth && !auth.isAuthenticated.value) {
    return {
      path: '/login',
      query: { redirect: to.fullPath },
    }
  }

  // Vérification des droits selon le rôle
  if (requiresAuth && requiredRole) {
    const userRole = auth.currentUser.value?.role
    if (userRole !== requiredRole) {
      if (auth.isAdmin.value) return '/admin'
      if (auth.isRecruiter.value) return '/recruteur/dashboard'
      if (auth.isJobSeeker.value) return '/dashboard'
      return '/recruteur/dashboard'
    }
  }

  return true
})

router.afterEach((to) => {
  const defaultTitle = 'ProfilsActifs — Plateforme de recrutement vidéo & certification'
  document.title = (to.meta?.title as string) || defaultTitle
})

export default router
