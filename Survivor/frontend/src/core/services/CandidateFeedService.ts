import type { ICandidateRepository, IPaginationParams, IPaginatedResult } from '../interfaces/ICandidateRepository'
import { CandidateProfileEntity } from '../domain/entities/CandidateProfile.entity'

export class CandidateFeedService {
  readonly repository: ICandidateRepository

  constructor(repository: ICandidateRepository) {
    this.repository = repository
  }

  public async fetchCandidates(params?: IPaginationParams): Promise<IPaginatedResult<CandidateProfileEntity>> {
    return await this.repository.getAll(params)
  }

  public async fetchCandidateById(id: string): Promise<CandidateProfileEntity | null> {
    return await this.repository.getById(id)
  }

  public async fetchCandidatesByIds(
    ids: string[],
    params?: Omit<IPaginationParams, 'ids'>,
  ): Promise<IPaginatedResult<CandidateProfileEntity>> {
    return await this.repository.getAll({ ...params, ids })
  }

  public async toggleCandidateSave(id: string): Promise<CandidateProfileEntity> {
    const candidate = await this.repository.getById(id)
    if (!candidate) throw new Error(`Candidat ${id} non trouvé`)
    candidate.toggleSave()
    return await this.repository.save(candidate)
  }

  public async contactCandidate(id: string): Promise<CandidateProfileEntity> {
    const candidate = await this.repository.getById(id)
    if (!candidate) throw new Error(`Candidat ${id} non trouvé`)
    candidate.recordSolicitation()
    return await this.repository.save(candidate)
  }
}
