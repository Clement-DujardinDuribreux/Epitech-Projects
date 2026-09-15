import { HttpClient } from '../http/HttpClient'
import type {
  IJobSeekerAvatarResponse,
  IJobSeekerCompetencesResponse,
  IJobSeekerCompetencesUpdate,
  IJobSeekerInteractionsResponse,
  IJobSeekerProfileResponse,
  IJobSeekerProfileUpdate,
  IJobSeekerRepository,
  IJobSeekerSectorsResponse,
  IJobSeekerSectorsUpdate,
  IJobSeekerSolicitationRaw,
  IJobSeekerVideoResponse,
  IJobSeekerVideoUpdate,
} from '../../interfaces/IJobSeekerRepository'

export class JobSeekerHttpRepository implements IJobSeekerRepository {
  private readonly httpClient: HttpClient

  constructor(httpClient: HttpClient = new HttpClient()) {
    this.httpClient = httpClient
  }

  public async getProfile(): Promise<IJobSeekerProfileResponse> {
    return this.httpClient.get<IJobSeekerProfileResponse>('/job_seeker/profile')
  }

  public async updateProfile(payload: IJobSeekerProfileUpdate): Promise<IJobSeekerProfileResponse> {
    return this.httpClient.put<IJobSeekerProfileResponse>('/job_seeker/profile', payload)
  }

  public async getCompetences(): Promise<string[]> {
    const response = await this.httpClient.get<IJobSeekerCompetencesResponse>('/job_seeker/competences')
    return response.competences || []
  }

  public async updateCompetences(payload: IJobSeekerCompetencesUpdate): Promise<string[]> {
    const response = await this.httpClient.put<IJobSeekerCompetencesResponse>('/job_seeker/competences', {
      add: payload.add || [],
      remove: payload.remove || [],
    })
    return response.competences || []
  }

  public async getSectors(): Promise<string[]> {
    const response = await this.httpClient.get<IJobSeekerSectorsResponse>('/job_seeker/sectors')
    return response.sectors || []
  }

  public async updateSectors(payload: IJobSeekerSectorsUpdate): Promise<string[]> {
    const response = await this.httpClient.put<IJobSeekerSectorsResponse>('/job_seeker/sectors', {
      add: payload.add || [],
      remove: payload.remove || [],
    })
    return response.sectors || []
  }

  public async getVideo(): Promise<IJobSeekerVideoResponse> {
    return this.httpClient.get<IJobSeekerVideoResponse>('/job_seeker/video')
  }

  public async updateVideo(payload: IJobSeekerVideoUpdate): Promise<IJobSeekerVideoResponse> {
    return this.httpClient.put<IJobSeekerVideoResponse>('/job_seeker/video', payload)
  }

  public async uploadVideo(file: File): Promise<IJobSeekerVideoResponse> {
    const formData = new FormData()
    formData.append('file', file)
    return this.httpClient.uploadFile<IJobSeekerVideoResponse>('/job_seeker/video/upload', formData)
  }

  public async uploadAvatar(file: File): Promise<IJobSeekerAvatarResponse> {
    const formData = new FormData()
    formData.append('file', file)
    return this.httpClient.uploadFile<IJobSeekerAvatarResponse>('/job_seeker/avatar/upload', formData)
  }

  public async getInteractions(): Promise<IJobSeekerInteractionsResponse> {
    return this.httpClient.get<IJobSeekerInteractionsResponse>('/job_seeker/interactions')
  }

  public async markSolicitationRead(id: string, read: boolean): Promise<IJobSeekerSolicitationRaw> {
    const suffix = read ? 'read' : 'unread'
    return this.httpClient.patch<IJobSeekerSolicitationRaw>(
      `/job_seeker/solicitations/${encodeURIComponent(id)}/${suffix}`
    )
  }

  public async archiveSolicitation(id: string, archived: boolean): Promise<IJobSeekerSolicitationRaw> {
    const suffix = archived ? 'archive' : 'unarchive'
    return this.httpClient.patch<IJobSeekerSolicitationRaw>(
      `/job_seeker/solicitations/${encodeURIComponent(id)}/${suffix}`
    )
  }

  public async replyToSolicitation(id: string, message: string): Promise<IJobSeekerSolicitationRaw> {
    return this.httpClient.post<IJobSeekerSolicitationRaw>(
      `/job_seeker/solicitations/${encodeURIComponent(id)}/reply`,
      { message }
    )
  }
}
