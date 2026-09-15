import type { CertificationQuestion } from '../../domain/entities/CertificationQuestion.entity'
import type { ICertificationRepository } from '../../interfaces/ICertificationRepository'
import questionsData from '../../../components/certification/questions.json'

export const CERTIFICATION_OFFICIAL_CATEGORIES = [
  'Travail',
  'Culture',
  'Comportement',
  'Gestion de projets',
] as const

export const CERTIFICATION_20_QUESTIONS: CertificationQuestion[] =
  questionsData.questions as CertificationQuestion[]

export class CertificationMockRepository implements ICertificationRepository {
  private readonly questions: CertificationQuestion[]

  constructor(customQuestions: CertificationQuestion[] = CERTIFICATION_20_QUESTIONS) {
    this.questions = [...customQuestions]
  }

  public async getAllQuestions(): Promise<CertificationQuestion[]> {
    return [...this.questions]
  }

  public async getQuestionById(id: number): Promise<CertificationQuestion | null> {
    const found = this.questions.find((q) => q.id === id)
    return found ? { ...found } : null
  }

  public async getQuestionsByCategory(category: string): Promise<CertificationQuestion[]> {
    return this.questions
      .filter((q) => q.categorie === category)
      .map((q) => ({ ...q }))
  }

  public async getCategories(): Promise<string[]> {
    return [...CERTIFICATION_OFFICIAL_CATEGORIES]
  }
}
