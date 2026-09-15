<script setup lang="ts">
import { ref } from 'vue'
import { useRouter, useRoute } from 'vue-router'
import { Button } from '@/components/ui/button'
import { useAuth } from '@/composables/useAuth'

const router = useRouter()
const route = useRoute()
const auth = useAuth()

const email = ref('')
const password = ref('')
const errorMessage = ref('')
const isLoading = ref(false)

function isValidEmail(value: string): boolean {
  return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(value)
}

function resolveRedirectPath(role: string): string {
  const redirectQuery = route.query.redirect as string | undefined
  if (redirectQuery && !redirectQuery.startsWith('/login') && !redirectQuery.startsWith('/register')) {
    return redirectQuery
  }

  if (role === 'recruiter') {
    return '/recruteur/dashboard'
  }
  if (role === 'admin') {
    return '/admin'
  }
  return '/dashboard'
}

async function handleSubmit(): Promise<void> {
  errorMessage.value = ''

  if (!isValidEmail(email.value)) {
    errorMessage.value = 'Veuillez saisir une adresse email valide.'
    return
  }
  if (password.value.length < 1) {
    errorMessage.value = 'Le mot de passe est obligatoire.'
    return
  }

  isLoading.value = true
  try {
    const user = await auth.login({
      email: email.value.trim(),
      password: password.value,
    })

    const target = resolveRedirectPath(user.role)
    await router.push(target)
  } catch (err: unknown) {
    if (err instanceof Error) {
      errorMessage.value = err.message
    } else {
      errorMessage.value = 'Identifiants invalides ou serveur indisponible.'
    }
  } finally {
    isLoading.value = false
  }
}
</script>

<template>
  <div class="container mx-auto py-10 max-w-md px-4">
    <h1 class="text-3xl font-bold font-heading mb-2 text-white">Connexion</h1>
    <p class="text-white/60 mb-2 text-sm font-body">Connexion multi-rôles (demandeur, recruteur, admin).</p>
    <p class="text-white/40 mb-6 text-xs italic font-body">Le CV montre le parcours. La vidéo révèle le talent.</p>

    <form class="space-y-4" @submit.prevent="handleSubmit">
      <div class="space-y-1">
        <label for="email" class="text-xs font-medium text-white/70 font-body">Email</label>
        <input
          id="email"
          v-model="email"
          type="email"
          autocomplete="email"
          class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white placeholder:text-white/30 focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
          placeholder="votre.email@exemple.fr"
          required
        />
      </div>

      <div class="space-y-1">
        <label for="password" class="text-xs font-medium text-white/70 font-body">Mot de passe</label>
        <input
          id="password"
          v-model="password"
          type="password"
          autocomplete="current-password"
          class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white placeholder:text-white/30 focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
          placeholder="••••••••"
          required
        />
      </div>

      <div
        v-if="errorMessage"
        class="p-3 rounded-lg bg-red-500/10 border border-red-500/20 text-red-400 text-xs font-body"
        role="alert"
      >
        {{ errorMessage }}
      </div>

      <Button type="submit" class="w-full bg-[#1a6fff] hover:bg-[#155bd6] text-white font-medium" :disabled="isLoading">
        {{ isLoading ? 'Connexion en cours...' : 'Se connecter' }}
      </Button>
    </form>

    <p class="text-xs text-white/50 mt-6 text-center font-body">
      Pas encore de compte ?
      <router-link to="/register" class="text-[#00d4ff] underline underline-offset-4 hover:text-[#1a6fff]">
        S'inscrire
      </router-link>
    </p>
  </div>
</template>
