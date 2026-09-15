import type { CertificationAnswer } from './CertificationQuestion.entity'

export type QuestionDifficulty = 'Facile' | 'Moyen' | 'Difficile'

export interface IAdminCertificationQuestionProps {
  id: number
  categorie: string
  type: string
  question: string
  reponse: CertificationAnswer
  ponderation: number
  difficulty: QuestionDifficulty
  successRate: number
  isActive: boolean
}

export class AdminCertificationQuestionEntity {
  private readonly _props: IAdminCertificationQuestionProps

  constructor(props: IAdminCertificationQuestionProps) {
    this.validate(props)
    this._props = { ...props }
  }

  private validate(props: IAdminCertificationQuestionProps): void {
    if (!props.question || props.question.trim().length === 0) {
      throw new Error('Le texte de la question est requis')
    }
    if (!props.categorie || props.categorie.trim().length === 0) {
      throw new Error('La catégorie de la question est requise')
    }
    if (props.ponderation <= 0) {
      throw new Error('La pondération doit être strictement positive')
    }
  }

  public get id(): number {
    return this._props.id
  }

  public get categorie(): string {
    return this._props.categorie
  }

  public get type(): string {
    return this._props.type
  }

  public get question(): string {
    return this._props.question
  }

  public get reponse(): CertificationAnswer {
    return this._props.reponse
  }

  public get ponderation(): number {
    return this._props.ponderation
  }

  public get difficulty(): QuestionDifficulty {
    return this._props.difficulty
  }

  public get successRate(): number {
    return this._props.successRate
  }

  public get isActive(): boolean {
    return this._props.isActive
  }

  public update(fields: Partial<Omit<IAdminCertificationQuestionProps, 'id'>>): void {
    if (fields.question !== undefined && fields.question.trim().length === 0) {
      throw new Error('Le texte de la question ne peut pas être vide')
    }
    if (fields.ponderation !== undefined && fields.ponderation <= 0) {
      throw new Error('La pondération doit être strictement positive')
    }
    Object.assign(this._props, fields)
  }

  public toggleActive(): void {
    this._props.isActive = !this._props.isActive
  }

  public toJSON(): IAdminCertificationQuestionProps {
    return { ...this._props }
  }
}
