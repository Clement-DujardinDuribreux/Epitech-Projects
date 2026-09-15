import type { ModerationItemEntity, ModerationStatus } from '../domain/entities/ModerationItem.entity'
import type { IModerationRepository } from '../interfaces/IModerationRepository'
import { ModerationMockRepository } from '../infrastructure/repositories/ModerationMockRepository'

export type ModerationFilter = 'all' | 'to_moderate' | ModerationStatus

export class ModerationService {
  private readonly _repository: IModerationRepository

  constructor(repository: IModerationRepository = new ModerationMockRepository()) {
    this._repository = repository
  }

  public async getItems(filter: ModerationFilter = 'all'): Promise<ModerationItemEntity[]> {
    const items = await this._repository.getAll()

    if (filter === 'all') {
      return items
    }
    if (filter === 'to_moderate') {
      return items.filter((item) => item.status === 'flagged' || item.status === 'pending')
    }
    return items.filter((item) => item.status === filter)
  }

  public async approveVideo(id: string): Promise<ModerationItemEntity> {
    const item = await this._repository.getById(id)
    if (!item) {
      throw new Error(`Élément de modération #${id} introuvable`)
    }
    item.approve()
    await this._repository.save(item)
    return item
  }

  public async rejectVideo(id: string, reason: string, comment?: string): Promise<ModerationItemEntity> {
    const item = await this._repository.getById(id)
    if (!item) {
      throw new Error(`Élément de modération #${id} introuvable`)
    }
    item.reject(reason, comment)
    await this._repository.save(item)
    return item
  }

  public async getCounts(): Promise<{
    pending: number
    flagged: number
    approved: number
    rejected: number
    totalToModerate: number
  }> {
    return this._repository.getCounts()
  }
}
