import type { RecruiterContactEntity, RecruiterContactStatus } from '../domain/entities/RecruiterContact.entity'

export interface IRecruiterRepository {
  getFavorites(): Promise<string[]>
  addFavorite(candidateId: string): Promise<void>
  removeFavorite(candidateId: string): Promise<void>
  isFavorite(candidateId: string): Promise<boolean>
  getContacts(): Promise<RecruiterContactEntity[]>
  saveContact(contact: RecruiterContactEntity): Promise<RecruiterContactEntity>
  updateContactStatus(contactId: string, status: RecruiterContactStatus): Promise<RecruiterContactEntity>
  deleteContact(contactId: string): Promise<void>
}
