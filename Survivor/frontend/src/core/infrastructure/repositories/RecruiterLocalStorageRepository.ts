import {
  RecruiterContactEntity,
  type IRecruiterContactProps,
  type RecruiterContactStatus,
} from '../../domain/entities/RecruiterContact.entity'
import type { IRecruiterRepository } from '../../interfaces/IRecruiterRepository'

const FAVORITES_STORAGE_KEY = 'profilsactifs_recruiter_favorites'
const CONTACTS_STORAGE_KEY = 'profilsactifs_recruiter_contacts'

const INITIAL_FAVORITES: string[] = ['lea-d', 'amina-traore']

const INITIAL_CONTACTS: IRecruiterContactProps[] = [
  {
    id: 'contact-init-1',
    candidateId: 'lea-d',
    candidateName: 'Léa D.',
    candidateHeadline: 'Vendeuse prêt-à-porter · Lyon, 69',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=200&q=80',
    location: 'Lyon, 69',
    isJebCertified: true,
    status: 'Entretien planifié',
    date: '2026-09-05T10:30:00.000Z',
    message:
      'Bonjour Léa, votre pitch vidéo et vos compétences en vente nous ont beaucoup plu. Nous souhaiterions organiser un premier entretien en visioconférence ce jeudi à 14h30.',
    updatedAt: '2026-09-06T09:15:00.000Z',
  },
  {
    id: 'contact-init-2',
    candidateId: 'amina-traore',
    candidateName: 'Amina Traoré',
    candidateHeadline: 'Développeuse Full-Stack · Paris, 75',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=200&q=80',
    location: 'Paris, 75',
    isJebCertified: true,
    status: 'En attente de réponse',
    date: '2026-09-06T15:45:00.000Z',
    message:
      'Bonjour Amina, votre profil technique en Vue 3 et TypeScript ainsi que votre Badge de certification JEB correspondent parfaitement à nos ouvertures actuelles. Avez-vous un créneau pour échanger ?',
    updatedAt: '2026-09-06T15:45:00.000Z',
  },
  {
    id: 'contact-init-3',
    candidateId: 'lucas-martin',
    candidateName: 'Lucas Martin',
    candidateHeadline: 'UX/UI Designer & Accessibilité · Lille, 59',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?auto=format&fit=crop&w=200&q=80',
    location: 'Lille, 59',
    isJebCertified: true,
    status: 'Prise de contact envoyée',
    date: '2026-09-07T08:20:00.000Z',
    message:
      'Bonjour Lucas, nous préparons un projet majeur nécessitant une stricte conformité RGAA 4.1. Votre profil nous intéresse vivement pour piloter notre Design System.',
    updatedAt: '2026-09-07T08:20:00.000Z',
  },
]

export class RecruiterLocalStorageRepository implements IRecruiterRepository {
  private isBrowser(): boolean {
    return typeof window !== 'undefined' && typeof window.localStorage !== 'undefined'
  }

  public async getFavorites(): Promise<string[]> {
    if (!this.isBrowser()) return [...INITIAL_FAVORITES]
    try {
      const stored = window.localStorage.getItem(FAVORITES_STORAGE_KEY)
      if (!stored) {
        window.localStorage.setItem(FAVORITES_STORAGE_KEY, JSON.stringify(INITIAL_FAVORITES))
        return [...INITIAL_FAVORITES]
      }
      return JSON.parse(stored) as string[]
    } catch (err) {
      console.warn('[RecruiterLocalStorageRepository] getFavorites failed:', err)
      return [...INITIAL_FAVORITES]
    }
  }

  public async addFavorite(candidateId: string): Promise<void> {
    const favorites = await this.getFavorites()
    if (!favorites.includes(candidateId)) {
      favorites.push(candidateId)
      if (this.isBrowser()) {
        try {
          window.localStorage.setItem(FAVORITES_STORAGE_KEY, JSON.stringify(favorites))
        } catch (err) {
          console.warn('[RecruiterLocalStorageRepository] addFavorite failed:', err)
        }
      }
    }
  }

  public async removeFavorite(candidateId: string): Promise<void> {
    const favorites = await this.getFavorites()
    const filtered = favorites.filter((id) => id !== candidateId)
    if (this.isBrowser()) {
      try {
        window.localStorage.setItem(FAVORITES_STORAGE_KEY, JSON.stringify(filtered))
      } catch (err) {
        console.warn('[RecruiterLocalStorageRepository] removeFavorite failed:', err)
      }
    }
  }

  public async isFavorite(candidateId: string): Promise<boolean> {
    const favorites = await this.getFavorites()
    return favorites.includes(candidateId)
  }

  public async getContacts(): Promise<RecruiterContactEntity[]> {
    if (!this.isBrowser()) {
      return INITIAL_CONTACTS.map((c) => new RecruiterContactEntity(c))
    }
    try {
      const stored = window.localStorage.getItem(CONTACTS_STORAGE_KEY)
      if (!stored) {
        window.localStorage.setItem(CONTACTS_STORAGE_KEY, JSON.stringify(INITIAL_CONTACTS))
        return INITIAL_CONTACTS.map((c) => new RecruiterContactEntity(c))
      }
      const rawList = JSON.parse(stored) as IRecruiterContactProps[]
      return rawList.map((item) => new RecruiterContactEntity(item))
    } catch (err) {
      console.warn('[RecruiterLocalStorageRepository] getContacts failed:', err)
      return INITIAL_CONTACTS.map((c) => new RecruiterContactEntity(c))
    }
  }

  public async saveContact(contact: RecruiterContactEntity): Promise<RecruiterContactEntity> {
    const contacts = await this.getContacts()
    const existingIndex = contacts.findIndex((c) => c.id === contact.id)
    if (existingIndex >= 0) {
      contacts[existingIndex] = contact
    } else {
      // Newest first
      contacts.unshift(contact)
    }

    if (this.isBrowser()) {
      try {
        window.localStorage.setItem(
          CONTACTS_STORAGE_KEY,
          JSON.stringify(contacts.map((c) => c.toJSON())),
        )
      } catch (err) {
        console.warn('[RecruiterLocalStorageRepository] saveContact failed:', err)
      }
    }

    return contact
  }

  public async updateContactStatus(
    contactId: string,
    status: RecruiterContactStatus,
  ): Promise<RecruiterContactEntity> {
    const contacts = await this.getContacts()
    const contact = contacts.find((c) => c.id === contactId)
    if (!contact) {
      throw new Error(`Contact ${contactId} introuvable`)
    }
    contact.updateStatus(status)

    if (this.isBrowser()) {
      try {
        window.localStorage.setItem(
          CONTACTS_STORAGE_KEY,
          JSON.stringify(contacts.map((c) => c.toJSON())),
        )
      } catch (err) {
        console.warn('[RecruiterLocalStorageRepository] updateContactStatus failed:', err)
      }
    }

    return contact
  }

  public async deleteContact(contactId: string): Promise<void> {
    const contacts = await this.getContacts()
    const filtered = contacts.filter((c) => c.id !== contactId)
    if (this.isBrowser()) {
      try {
        window.localStorage.setItem(
          CONTACTS_STORAGE_KEY,
          JSON.stringify(filtered.map((c) => c.toJSON())),
        )
      } catch (err) {
        console.warn('[RecruiterLocalStorageRepository] deleteContact failed:', err)
      }
    }
  }
}
