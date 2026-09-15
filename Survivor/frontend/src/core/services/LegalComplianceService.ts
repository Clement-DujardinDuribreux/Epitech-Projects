import type { ILegalRepository } from '../interfaces/ILegalRepository'
import { LegalDocumentEntity, type LegalSectionId } from '../domain/entities/LegalDocument.entity'

export class LegalComplianceService {
  readonly repository: ILegalRepository

  constructor(repository: ILegalRepository) {
    this.repository = repository
  }

  public async fetchAllDocuments(): Promise<LegalDocumentEntity[]> {
    return await this.repository.getAllDocuments()
  }

  public async fetchDocument(id: LegalSectionId): Promise<LegalDocumentEntity | null> {
    return await this.repository.getDocumentById(id)
  }
}
