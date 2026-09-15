import type { CandidateProfileEntity } from '../domain/entities/CandidateProfile.entity'

export interface IPaginationParams {
  page?: number
  limit?: number
  query?: string
  skill?: string
  jebOnly?: boolean
  ids?: string[]
}

export interface IPaginatedResult<T> {
  items: T[]
  total: number
  page: number
  limit: number
  totalPages: number
  hasMore: boolean
}

export interface ICandidateRepository {
  getAll(params?: IPaginationParams): Promise<IPaginatedResult<CandidateProfileEntity>>
  getById(id: string): Promise<CandidateProfileEntity | null>
  save(candidate: CandidateProfileEntity): Promise<CandidateProfileEntity>
}
