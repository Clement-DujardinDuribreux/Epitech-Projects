import type { LegalDocumentEntity, LegalSectionId } from '../domain/entities/LegalDocument.entity'

export interface ILegalRepository {
  getAllDocuments(): Promise<LegalDocumentEntity[]>
  getDocumentById(id: LegalSectionId): Promise<LegalDocumentEntity | null>
}
