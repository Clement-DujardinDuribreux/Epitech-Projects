export type RecruiterContactStatus =
  | 'Prise de contact envoyée'
  | 'En attente de réponse'
  | 'Entretien planifié'

export interface IRecruiterContactProps {
  id: string
  candidateId: string
  candidateName: string
  candidateHeadline: string
  candidateAvatarUrl: string
  location?: string
  date: string
  message: string
  status: RecruiterContactStatus
  isJebCertified?: boolean
  updatedAt?: string
}

export class RecruiterContactEntity {
  readonly props: IRecruiterContactProps
  private _status: RecruiterContactStatus
  private _updatedAt?: string

  constructor(props: IRecruiterContactProps) {
    this.validate(props)
    this.props = { ...props }
    this._status = props.status
    this._updatedAt = props.updatedAt
  }

  private validate(props: IRecruiterContactProps): void {
    if (!props.id) throw new Error('ID du contact requis')
    if (!props.candidateId) throw new Error('ID candidat requis')
    if (!props.candidateName) throw new Error('Nom du candidat requis')
    if (!props.message) throw new Error('Message requis')
    if (!props.status) throw new Error('Statut requis')
  }

  public get id(): string {
    return this.props.id
  }

  public get candidateId(): string {
    return this.props.candidateId
  }

  public get candidateName(): string {
    return this.props.candidateName
  }

  public get candidateHeadline(): string {
    return this.props.candidateHeadline
  }

  public get candidateAvatarUrl(): string {
    return this.props.candidateAvatarUrl
  }

  public get location(): string | undefined {
    return this.props.location
  }

  public get date(): string {
    return this.props.date
  }

  public get message(): string {
    return this.props.message
  }

  public get status(): RecruiterContactStatus {
    return this._status
  }

  public get isJebCertified(): boolean {
    return !!this.props.isJebCertified
  }

  public get updatedAt(): string | undefined {
    return this._updatedAt
  }

  public updateStatus(newStatus: RecruiterContactStatus): void {
    this._status = newStatus
    this._updatedAt = new Date().toISOString()
  }

  public toJSON(): IRecruiterContactProps {
    return {
      ...this.props,
      status: this._status,
      updatedAt: this._updatedAt,
    }
  }
}
