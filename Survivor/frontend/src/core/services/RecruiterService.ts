import {
  RecruiterContactEntity,
  type RecruiterContactStatus,
} from '../domain/entities/RecruiterContact.entity'
import type { IRecruiterRepository } from '../interfaces/IRecruiterRepository'
import { RecruiterLocalStorageRepository } from '../infrastructure/repositories/RecruiterLocalStorageRepository'

export interface ISendContactInput {
  candidateId: string
  candidateName: string
  candidateHeadline: string
  candidateAvatarUrl: string
  location?: string
  isJebCertified?: boolean
  message: string
  status?: RecruiterContactStatus
}

export interface IRecruiterTrackingStats {
  total: number
  envoyes: number
  enAttente: number
  entretiens: number
}

export class RecruiterService {
  readonly repository: IRecruiterRepository

  constructor(repository: IRecruiterRepository = new RecruiterLocalStorageRepository()) {
    this.repository = repository
  }

  public async getFavorites(): Promise<string[]> {
    return await this.repository.getFavorites()
  }

  public async toggleFavorite(candidateId: string): Promise<boolean> {
    const isFav = await this.repository.isFavorite(candidateId)
    if (isFav) {
      await this.repository.removeFavorite(candidateId)
      return false
    }
    await this.repository.addFavorite(candidateId)
    return true
  }

  public async isFavorite(candidateId: string): Promise<boolean> {
    return await this.repository.isFavorite(candidateId)
  }

  public async getContacts(): Promise<RecruiterContactEntity[]> {
    return await this.repository.getContacts()
  }

  public async sendContact(input: ISendContactInput): Promise<RecruiterContactEntity> {
    const newContact = new RecruiterContactEntity({
      id: `contact-${Date.now()}-${Math.random().toString(36).slice(2, 7)}`,
      candidateId: input.candidateId,
      candidateName: input.candidateName,
      candidateHeadline: input.candidateHeadline,
      candidateAvatarUrl: input.candidateAvatarUrl,
      location: input.location,
      isJebCertified: input.isJebCertified,
      date: new Date().toISOString(),
      message: input.message.trim(),
      status: input.status ?? 'Prise de contact envoyée',
    })

    return await this.repository.saveContact(newContact)
  }

  public async updateContactStatus(
    contactId: string,
    status: RecruiterContactStatus,
  ): Promise<RecruiterContactEntity> {
    return await this.repository.updateContactStatus(contactId, status)
  }

  public async deleteContact(contactId: string): Promise<void> {
    await this.repository.deleteContact(contactId)
  }

  public async getStats(): Promise<IRecruiterTrackingStats> {
    const contacts = await this.repository.getContacts()
    let envoyes = 0
    let enAttente = 0
    let entretiens = 0

    for (const c of contacts) {
      if (c.status === 'Prise de contact envoyée') envoyes++
      else if (c.status === 'En attente de réponse') enAttente++
      else if (c.status === 'Entretien planifié') entretiens++
    }

    return {
      total: contacts.length,
      envoyes,
      enAttente,
      entretiens,
    }
  }
}
