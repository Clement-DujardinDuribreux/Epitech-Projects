import type { CertificationQuestion } from '../domain/entities/CertificationQuestion.entity'

export interface ICertificationRepository {
  getAllQuestions(): Promise<CertificationQuestion[]>
  getQuestionById(id: number): Promise<CertificationQuestion | null>
  getQuestionsByCategory(category: string): Promise<CertificationQuestion[]>
  getCategories(): Promise<string[]>
}
