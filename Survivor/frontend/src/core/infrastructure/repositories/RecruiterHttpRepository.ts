import {
  RecruiterContactEntity,
  type RecruiterContactStatus,
} from '../../domain/entities/RecruiterContact.entity'
import type { IRecruiterRepository } from '../../interfaces/IRecruiterRepository'
import { HttpClient } from '../http/HttpClient'
import { RecruiterLocalStorageRepository } from './RecruiterLocalStorageRepository'

interface IFollowUserItem {
  id: string
  email?: string
  firstname?: string
  name?: string
  localisation?: string
  certificated?: boolean | number
}

interface IFollowResponse {
  data: IFollowUserItem[]
  total: number
}

interface IBackendSolicitation {
  id: string
  recruiter_id?: string
  job_seeker_id: string
  message: string
  status: string
  created_at?: string
  updated_at?: string
  candidate_firstname?: string
  candidate_name?: string
  candidate_email?: string
  candidate_location?: string
}

function mapBackendStatusToDomain(status: string): RecruiterContactStatus {
  if (status === 'interview') return 'Entretien planifié'
  if (status === 'contacted') return 'En attente de réponse'
  return 'Prise de contact envoyée'
}

function mapDomainStatusToBackend(status: RecruiterContactStatus): 'sent' | 'contacted' | 'interview' {
  if (status === 'Entretien planifié') return 'interview'
  if (status === 'En attente de réponse') return 'contacted'
  return 'sent'
}

function mapBackendToContactEntity(raw: IBackendSolicitation): RecruiterContactEntity {
  const firstName = raw.candidate_firstname || ''
  const lastName = raw.candidate_name || ''
  const fullName = `${firstName} ${lastName}`.trim() || 'Candidat ProfilsActifs'
  const location = raw.candidate_location || 'France'

  return new RecruiterContactEntity({
    id: raw.id,
    candidateId: raw.job_seeker_id,
    candidateName: fullName,
    candidateHeadline: `Candidat · ${location}`,
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=200&q=80',
    location,
    date: raw.created_at ? new Date(raw.created_at).toISOString() : new Date().toISOString(),
    message: raw.message,
    status: mapBackendStatusToDomain(raw.status),
    isJebCertified: true,
    updatedAt: raw.updated_at ? new Date(raw.updated_at).toISOString() : undefined,
  })
}

export class RecruiterHttpRepository implements IRecruiterRepository {
  private readonly httpClient: HttpClient
  private readonly contactsRepo: IRecruiterRepository

  constructor(
    httpClient: HttpClient = new HttpClient(),
    contactsRepo: IRecruiterRepository = new RecruiterLocalStorageRepository()
  ) {
    this.httpClient = httpClient
    this.contactsRepo = contactsRepo
  }

  public async getFavorites(): Promise<string[]> {
    try {
      const response = await this.httpClient.get<IFollowResponse>('/profile/follow')
      if (response && Array.isArray(response.data)) {
        return response.data.map((user) => user.id)
      }
      return []
    } catch {
      return await this.contactsRepo.getFavorites()
    }
  }

  public async addFavorite(candidateId: string): Promise<void> {
    try {
      await this.httpClient.post<unknown>(`/profile/follow/${encodeURIComponent(candidateId)}`)
    } catch {
      await this.contactsRepo.addFavorite(candidateId)
    }
  }

  public async removeFavorite(candidateId: string): Promise<void> {
    try {
      await this.httpClient.delete<void>(`/profile/unfollow/${encodeURIComponent(candidateId)}`)
    } catch {
      await this.contactsRepo.removeFavorite(candidateId)
    }
  }

  public async isFavorite(candidateId: string): Promise<boolean> {
    const favorites = await this.getFavorites()
    return favorites.includes(candidateId)
  }

  public async getContacts(): Promise<RecruiterContactEntity[]> {
    try {
      const response = await this.httpClient.get<IBackendSolicitation[]>('/recruiter/solicitations')
      if (Array.isArray(response)) {
        if (response.length === 0) {
          return await this.contactsRepo.getContacts()
        }
        return response.map(mapBackendToContactEntity)
      }
      return await this.contactsRepo.getContacts()
    } catch (err) {
      console.warn('[RecruiterHttpRepository] getContacts API error, fallback to local storage:', err)
      return await this.contactsRepo.getContacts()
    }
  }

  public async saveContact(contact: RecruiterContactEntity): Promise<RecruiterContactEntity> {
    try {
      const created = await this.httpClient.post<IBackendSolicitation>('/recruiter/solicitations', {
        candidate_id: contact.candidateId,
        message: contact.message,
      })

      if (created && created.id) {
        const backendStatus = mapDomainStatusToBackend(contact.status)
        if (backendStatus !== 'sent') {
          try {
            await this.httpClient.patch<unknown>(
              `/recruiter/solicitations/${encodeURIComponent(created.id)}/status`,
              { status: backendStatus }
            )
          } catch {
            // Ignore status patch error
          }
        }
        const savedEntity = mapBackendToContactEntity({
          ...created,
          status: backendStatus,
          candidate_firstname: contact.candidateName.split(' ')[0] || '',
          candidate_name: contact.candidateName.split(' ').slice(1).join(' ') || '',
          candidate_location: contact.location,
        })
        try {
          await this.contactsRepo.saveContact(savedEntity)
        } catch {
          // Ignore local mirror error
        }
        return savedEntity
      }
      return await this.contactsRepo.saveContact(contact)
    } catch (err) {
      console.warn('[RecruiterHttpRepository] saveContact API error, fallback to local storage:', err)
      return await this.contactsRepo.saveContact(contact)
    }
  }

  public async updateContactStatus(
    contactId: string,
    status: RecruiterContactStatus
  ): Promise<RecruiterContactEntity> {
    try {
      const backendStatus = mapDomainStatusToBackend(status)
      await this.httpClient.patch<unknown>(
        `/recruiter/solicitations/${encodeURIComponent(contactId)}/status`,
        { status: backendStatus }
      )
      return await this.contactsRepo.updateContactStatus(contactId, status)
    } catch (err) {
      console.warn('[RecruiterHttpRepository] updateContactStatus API error, fallback to local storage:', err)
      return await this.contactsRepo.updateContactStatus(contactId, status)
    }
  }

  public async deleteContact(contactId: string): Promise<void> {
    try {
      await this.httpClient.delete<void>(`/recruiter/solicitations/${encodeURIComponent(contactId)}`)
      await this.contactsRepo.deleteContact(contactId)
    } catch (err) {
      console.warn('[RecruiterHttpRepository] deleteContact API error, fallback to local storage:', err)
      await this.contactsRepo.deleteContact(contactId)
    }
  }
}
