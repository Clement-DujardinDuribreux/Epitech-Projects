import { ref, computed } from 'vue'
import { JobSeekerHttpRepository } from '@/core/infrastructure/repositories/JobSeekerHttpRepository'
import type { IJobSeekerProfileResponse } from '@/core/interfaces/IJobSeekerRepository'
import type { ICandidateProfileFormData } from '@/components/dashboard/CandidateProfileEditor.vue'

export function useCandidateProfileEditor(jobSeekerRepo: JobSeekerHttpRepository = new JobSeekerHttpRepository()) {
  const candidateProfile = ref<IJobSeekerProfileResponse | null>(null)
  const competencesList = ref<string[]>([])
  const videoLink = ref<string>('')
  const videoValid = ref<boolean>(true)
  const videoReason = ref<string | null>(null)

  const profileInitialData = computed<Partial<ICandidateProfileFormData>>(() => {
    const full = `${candidateProfile.value?.firstname || ''} ${candidateProfile.value?.name || ''}`.trim()
    return {
      fullName: full || undefined,
      headline: candidateProfile.value?.headline || undefined,
      location: candidateProfile.value?.localisation || undefined,
      email: candidateProfile.value?.email || undefined,
      phone: candidateProfile.value?.phone || undefined,
      bio: candidateProfile.value?.bio || undefined,
      avatarUrl: candidateProfile.value?.avatar_url || undefined,
      skills: competencesList.value.length > 0 ? [...competencesList.value] : undefined,
    }
  })

  async function load(): Promise<void> {
    const [profRes, compsRes, vidRes] = await Promise.allSettled([
      jobSeekerRepo.getProfile(),
      jobSeekerRepo.getCompetences(),
      jobSeekerRepo.getVideo(),
    ])
    if (profRes.status === 'fulfilled') candidateProfile.value = profRes.value
    if (compsRes.status === 'fulfilled') competencesList.value = compsRes.value
    if (vidRes.status === 'fulfilled') {
      videoLink.value = vidRes.value.link
      videoValid.value = Boolean(vidRes.value.valid)
      videoReason.value = vidRes.value.reason ?? null
    }
  }

  async function handleSaveProfile(data: ICandidateProfileFormData): Promise<void> {
    try {
      let avatarUrl = data.avatarUrl
      if (data.avatarFile) {
        const uploaded = await jobSeekerRepo.uploadAvatar(data.avatarFile)
        avatarUrl = uploaded.avatar_url
      }

      const parts = data.fullName.trim().split(' ')
      const firstname = parts[0] || ''
      const name = parts.slice(1).join(' ') || ''

      await jobSeekerRepo.updateProfile({
        firstname,
        name,
        localisation: data.location,
        headline: data.headline,
        bio: data.bio,
        phone: data.phone,
        avatar_url: avatarUrl,
      })

      const currentSkills = new Set(competencesList.value)
      const newSkills = new Set(data.skills)
      const toAdd = data.skills.filter((s) => !currentSkills.has(s))
      const toRemove = competencesList.value.filter((s) => !newSkills.has(s))
      if (toAdd.length > 0 || toRemove.length > 0) {
        await jobSeekerRepo.updateCompetences({ add: toAdd, remove: toRemove })
      }

      // Recharge depuis le backend pour garantir que l'affichage reflète bien
      // ce qui a été persisté (plutôt qu'un patch local optimiste divergent).
      await load()
    } catch {
      // Gestion silencieuse d'erreur
    }
  }

  async function handleSaveVideo(link: string): Promise<void> {
    try {
      const updated = await jobSeekerRepo.updateVideo({ link })
      videoLink.value = updated.link
    } catch {
      // Gestion silencieuse d'erreur
    }
  }

  return {
    candidateProfile,
    competencesList,
    videoLink,
    videoValid,
    videoReason,
    profileInitialData,
    load,
    handleSaveProfile,
    handleSaveVideo,
  }
}
