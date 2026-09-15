export interface IJobSeekerProfileResponse {
  id: string
  email: string
  firstname: string
  name: string
  role: string
  localisation?: string | null
  certificated?: boolean | null
  birthdate?: string | null
  headline?: string | null
  bio?: string | null
  phone?: string | null
  avatar_url?: string | null
  created_at?: string | null
  last_update_at?: string | null
}

export interface IJobSeekerProfileUpdate {
  firstname?: string
  name?: string
  localisation?: string
  birthdate?: string
  headline?: string
  bio?: string
  phone?: string
  avatar_url?: string
}

export interface IJobSeekerAvatarResponse {
  avatar_url: string
}

export interface IJobSeekerCompetencesResponse {
  competences: string[]
}

export interface IJobSeekerCompetencesUpdate {
  add?: string[]
  remove?: string[]
}

export interface IJobSeekerSectorsResponse {
  sectors: string[]
}

export interface IJobSeekerSectorsUpdate {
  add?: string[]
  remove?: string[]
}

export interface IJobSeekerVideoResponse {
  link: string
  valid: boolean
  reason?: string | null
}

export interface IJobSeekerVideoUpdate {
  link: string
}

export interface IJobSeekerSolicitationRaw {
  id: string
  recruiter_id: string
  message: string
  status: string
  created_at: string
  is_read?: number | boolean
  is_archived?: number | boolean
  reply?: string | null
  replied_at?: string | null
  recruiter_firstname?: string | null
  recruiter_name?: string | null
  recruiter_email?: string | null
  recruiter_location?: string | null
}

export interface IJobSeekerInteractionsResponse {
  viewsCount: number
  solicitations: IJobSeekerSolicitationRaw[]
}

export interface IJobSeekerRepository {
  getProfile(): Promise<IJobSeekerProfileResponse>
  updateProfile(payload: IJobSeekerProfileUpdate): Promise<IJobSeekerProfileResponse>
  getCompetences(): Promise<string[]>
  updateCompetences(payload: IJobSeekerCompetencesUpdate): Promise<string[]>
  getSectors(): Promise<string[]>
  updateSectors(payload: IJobSeekerSectorsUpdate): Promise<string[]>
  getVideo(): Promise<IJobSeekerVideoResponse>
  updateVideo(payload: IJobSeekerVideoUpdate): Promise<IJobSeekerVideoResponse>
  uploadVideo(file: File): Promise<IJobSeekerVideoResponse>
  uploadAvatar(file: File): Promise<IJobSeekerAvatarResponse>
  getInteractions(): Promise<IJobSeekerInteractionsResponse>
  markSolicitationRead(id: string, read: boolean): Promise<IJobSeekerSolicitationRaw>
  archiveSolicitation(id: string, archived: boolean): Promise<IJobSeekerSolicitationRaw>
  replyToSolicitation(id: string, message: string): Promise<IJobSeekerSolicitationRaw>
}
