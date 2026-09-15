import type { AdminCertificationQuestionEntity } from '../domain/entities/AdminCertificationQuestion.entity'

export interface IAdminCertificationRepository {
  getAll(): Promise<AdminCertificationQuestionEntity[]>
  getById(id: number): Promise<AdminCertificationQuestionEntity | null>
  save(question: AdminCertificationQuestionEntity): Promise<AdminCertificationQuestionEntity>
  delete(id: number): Promise<void>
  resetToDefaults(): Promise<AdminCertificationQuestionEntity[]>
}
