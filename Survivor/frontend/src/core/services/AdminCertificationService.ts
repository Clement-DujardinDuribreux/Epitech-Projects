import {
  AdminCertificationQuestionEntity,
  type IAdminCertificationQuestionProps,
} from '../domain/entities/AdminCertificationQuestion.entity'
import type { IAdminCertificationRepository } from '../interfaces/IAdminCertificationRepository'
import { AdminCertificationMockRepository } from '../infrastructure/repositories/AdminCertificationMockRepository'

export class AdminCertificationService {
  private readonly _repository: IAdminCertificationRepository

  constructor(repository: IAdminCertificationRepository = new AdminCertificationMockRepository()) {
    this._repository = repository
  }

  public async getQuestions(
    categoryFilter?: string,
    searchQuery?: string,
  ): Promise<AdminCertificationQuestionEntity[]> {
    const questions = await this._repository.getAll()

    return questions.filter((q) => {
      const matchCategory = !categoryFilter || categoryFilter === 'Toutes' || q.categorie === categoryFilter
      const matchSearch =
        !searchQuery ||
        q.question.toLowerCase().includes(searchQuery.toLowerCase()) ||
        q.categorie.toLowerCase().includes(searchQuery.toLowerCase())
      return matchCategory && matchSearch
    })
  }

  public async getCategories(): Promise<string[]> {
    const questions = await this._repository.getAll()
    const set = new Set<string>()
    for (const q of questions) {
      set.add(q.categorie)
    }
    return Array.from(set)
  }

  public async createQuestion(
    props: Omit<IAdminCertificationQuestionProps, 'id'>,
  ): Promise<AdminCertificationQuestionEntity> {
    const all = await this._repository.getAll()
    const nextId = all.length > 0 ? Math.max(...all.map((q) => q.id)) + 1 : 1
    const entity = new AdminCertificationQuestionEntity({
      id: nextId,
      ...props,
    })
    return this._repository.save(entity)
  }

  public async updateQuestion(
    id: number,
    fields: Partial<Omit<IAdminCertificationQuestionProps, 'id'>>,
  ): Promise<AdminCertificationQuestionEntity> {
    const existing = await this._repository.getById(id)
    if (!existing) {
      throw new Error(`Question #${id} introuvable`)
    }
    existing.update(fields)
    return this._repository.save(existing)
  }

  public async deleteQuestion(id: number): Promise<void> {
    return this._repository.delete(id)
  }

  public async resetToDefaults(): Promise<AdminCertificationQuestionEntity[]> {
    return this._repository.resetToDefaults()
  }
}
