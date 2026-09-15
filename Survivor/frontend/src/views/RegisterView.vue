<script setup lang="ts">
import { ref } from 'vue'
import { useRouter } from 'vue-router'
import { Button } from '@/components/ui/button'
import { useAuth } from '@/composables/useAuth'

const router = useRouter()
const auth = useAuth()

const selectedRole = ref<'job_seeker' | 'recruiter'>('job_seeker')
const email = ref('')
const password = ref('')
const confirmPassword = ref('')
const firstName = ref('')
const lastName = ref('')
const birthDate = ref('')
const localisation = ref('')
const errorMessage = ref('')
const isLoading = ref(false)

function isValidEmail(value: string): boolean {
  return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(value)
}

function isValidPassword(value: string): boolean {
  return /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d).{8,}$/.test(value)
}

async function handleSubmit(): Promise<void> {
  errorMessage.value = ''

  if (!isValidEmail(email.value)) {
    errorMessage.value = 'Veuillez saisir une adresse email valide.'
    return
  }
  if (!isValidPassword(password.value)) {
    errorMessage.value = 'Le mot de passe doit contenir au moins 8 caractères, 1 majuscule, 1 minuscule et 1 chiffre.'
    return
  }
  if (password.value !== confirmPassword.value) {
    errorMessage.value = 'Les mots de passe ne correspondent pas.'
    return
  }
  if (!firstName.value.trim() || !lastName.value.trim()) {
    errorMessage.value = 'Le nom et le prénom sont obligatoires.'
    return
  }
  if (!birthDate.value) {
    errorMessage.value = 'La date de naissance est obligatoire.'
    return
  }

  isLoading.value = true
  try {
    const user = await auth.register({
      email: email.value.trim(),
      password: password.value,
      firstname: firstName.value.trim(),
      name: lastName.value.trim(),
      role: selectedRole.value,
      birthdate: birthDate.value,
      localisation: localisation.value.trim() || undefined,
    })

    if (user.role === 'recruiter') {
      await router.push('/recruteur/dashboard')
    } else {
      await router.push('/dashboard')
    }
  } catch (err: unknown) {
    if (err instanceof Error) {
      errorMessage.value = err.message
    } else {
      errorMessage.value = "Une erreur est survenue lors de l'inscription."
    }
  } finally {
    isLoading.value = false
  }
}
</script>

<template>
  <div class="container mx-auto py-10 max-w-md px-4">
    <h1 class="text-3xl font-bold font-heading mb-2 text-white">Inscription</h1>
    <p class="text-white/60 mb-6 text-sm font-body">Le CV montre le parcours. La vidéo révèle le talent.</p>

    <form class="space-y-4" @submit.prevent="handleSubmit">
      <!-- Sélecteur de rôle -->
      <div class="flex gap-2 p-1 bg-[#121218] rounded-lg border border-white/10">
        <Button
          type="button"
          :variant="selectedRole === 'job_seeker' ? 'default' : 'ghost'"
          class="flex-1 text-xs"
          :class="{ 'bg-[#1a6fff] text-white': selectedRole === 'job_seeker' }"
          @click="selectedRole = 'job_seeker'"
        >
          Candidat
        </Button>
        <Button
          type="button"
          :variant="selectedRole === 'recruiter' ? 'default' : 'ghost'"
          class="flex-1 text-xs"
          :class="{ 'bg-[#1a6fff] text-white': selectedRole === 'recruiter' }"
          @click="selectedRole = 'recruiter'"
        >
          Recruteur
        </Button>
      </div>

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
          autocomplete="new-password"
          class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white placeholder:text-white/30 focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
          placeholder="Min. 8 caractères, 1 Maj, 1 Min, 1 Chiffre"
          required
        />
      </div>

      <div class="space-y-1">
        <label for="confirm-password" class="text-xs font-medium text-white/70 font-body">Confirmer le mot de passe</label>
        <input
          id="confirm-password"
          v-model="confirmPassword"
          type="password"
          autocomplete="new-password"
          class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white placeholder:text-white/30 focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
          placeholder="Confirmez votre mot de passe"
          required
        />
      </div>

      <div class="grid grid-cols-2 gap-3">
        <div class="space-y-1">
          <label for="first-name" class="text-xs font-medium text-white/70 font-body">Prénom</label>
          <input
            id="first-name"
            v-model="firstName"
            type="text"
            autocomplete="given-name"
            class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white placeholder:text-white/30 focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
            placeholder="Jean"
            required
          />
        </div>

        <div class="space-y-1">
          <label for="last-name" class="text-xs font-medium text-white/70 font-body">Nom</label>
          <input
            id="last-name"
            v-model="lastName"
            type="text"
            autocomplete="family-name"
            class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white placeholder:text-white/30 focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
            placeholder="Dupont"
            required
          />
        </div>
      </div>

      <div class="space-y-1">
        <label for="birth-date" class="text-xs font-medium text-white/70 font-body">Date de naissance</label>
        <input
          id="birth-date"
          v-model="birthDate"
          type="date"
          autocomplete="bday"
          class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
          required
        />
      </div>

      <div class="space-y-1">
        <label for="localisation" class="text-xs font-medium text-white/70 font-body">Localisation (Optionnel)</label>
        <input
          id="localisation"
          v-model="localisation"
          type="text"
          class="w-full rounded-md border border-white/10 bg-[#121218] px-3 py-2 text-sm text-white placeholder:text-white/30 focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#1a6fff] font-body"
          placeholder="Ex: Paris, Lyon, Lille..."
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
        {{ isLoading ? 'Création en cours...' : 'Créer mon compte' }}
      </Button>
    </form>

    <p class="text-xs text-white/50 mt-6 text-center font-body">
      Déjà un compte ?
      <router-link to="/login" class="text-[#00d4ff] underline underline-offset-4 hover:text-[#1a6fff]">
        Se connecter
      </router-link>
    </p>
  </div>
</template>
