import { ref, computed } from 'vue'
import { AuthHttpRepository } from '../core/infrastructure/repositories/AuthHttpRepository'
import type {
  IAuthRepository,
  IAuthUser,
  ILoginCredentials,
  IRegisterPayload,
} from '../core/interfaces/IAuthRepository'

const token = ref<string | null>(null)
const currentUser = ref<IAuthUser | null>(null)
const isInitialized = ref(false)
const isLoading = ref(false)
const error = ref<string | null>(null)

const authRepo: IAuthRepository = new AuthHttpRepository()

export function useAuth() {
  const isAuthenticated = computed(() => Boolean(token.value))
  const isJobSeeker = computed(() => currentUser.value?.role === 'job_seeker')
  const isRecruiter = computed(() => currentUser.value?.role === 'recruiter')
  const isAdmin = computed(() => currentUser.value?.role === 'admin')

  function clearSession(): void {
    token.value = null
    currentUser.value = null
    if (typeof localStorage !== 'undefined') {
      localStorage.removeItem('auth_token')
      localStorage.removeItem('access_token')
      localStorage.removeItem('auth_user')
      localStorage.removeItem('user')
    }
  }

  function persistSession(newToken: string, user: IAuthUser): void {
    token.value = newToken
    currentUser.value = user
    if (typeof localStorage !== 'undefined') {
      localStorage.setItem('auth_token', newToken)
      localStorage.setItem('access_token', newToken)
      localStorage.setItem('auth_user', JSON.stringify(user))
      localStorage.setItem('user', JSON.stringify(user))
    }
  }

  async function initAuth(): Promise<void> {
    if (isInitialized.value) return
    isInitialized.value = true

    if (typeof localStorage === 'undefined') return

    const savedToken = localStorage.getItem('auth_token') || localStorage.getItem('access_token')
    const savedUserStr = localStorage.getItem('auth_user') || localStorage.getItem('user')

    if (!savedToken) {
      clearSession()
      return
    }

    token.value = savedToken
    if (savedUserStr) {
      try {
        currentUser.value = JSON.parse(savedUserStr) as IAuthUser
      } catch {
        currentUser.value = null
      }
    }

    try {
      const freshUser = await authRepo.getMe()
      currentUser.value = freshUser
      localStorage.setItem('auth_user', JSON.stringify(freshUser))
    } catch {
      // Si le token est invalide ou expiré côté backend
      clearSession()
    }
  }

  async function login(credentials: ILoginCredentials): Promise<IAuthUser> {
    isLoading.value = true
    error.value = null
    try {
      const response = await authRepo.login(credentials)
      persistSession(response.access_token, response.user)
      return response.user
    } catch (err: unknown) {
      const msg = err instanceof Error ? err.message : 'Échec de la connexion'
      error.value = msg
      throw err
    } finally {
      isLoading.value = false
    }
  }

  async function register(payload: IRegisterPayload): Promise<IAuthUser> {
    isLoading.value = true
    error.value = null
    try {
      const response = await authRepo.register(payload)
      persistSession(response.access_token, response.user)
      return response.user
    } catch (err: unknown) {
      const msg = err instanceof Error ? err.message : "Échec de l'inscription"
      error.value = msg
      throw err
    } finally {
      isLoading.value = false
    }
  }

  function logout(): void {
    clearSession()
  }

  return {
    token,
    currentUser,
    isAuthenticated,
    isJobSeeker,
    isRecruiter,
    isAdmin,
    isLoading,
    error,
    login,
    register,
    logout,
    initAuth,
  }
}
