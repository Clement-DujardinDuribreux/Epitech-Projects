import type { ModerationItemEntity } from '../domain/entities/ModerationItem.entity'

export interface IModerationRepository {
  getAll(): Promise<ModerationItemEntity[]>
  getById(id: string): Promise<ModerationItemEntity | null>
  save(item: ModerationItemEntity): Promise<ModerationItemEntity>
  getCounts(): Promise<{ pending: number; flagged: number; approved: number; rejected: number; totalToModerate: number }>
}
