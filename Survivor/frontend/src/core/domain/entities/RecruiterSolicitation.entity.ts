export type SolicitationStatus = 'unread' | 'read' | 'archived'

export interface IRecruiterSolicitationProps {
  id: string
  recruiterName: string
  company: string
  recruiterRole?: string
  avatarUrl?: string
  date: string
  subject: string
  message: string
  status: SolicitationStatus
  reply?: string
  repliedAt?: string
  createdAt: string
}

export class RecruiterSolicitationEntity {
  public readonly props: Readonly<IRecruiterSolicitationProps>
  private _status: SolicitationStatus
  private _reply?: string
  private _repliedAt?: string

  constructor(props: IRecruiterSolicitationProps) {
    this.validate(props)
    this.props = { ...props }
    this._status = props.status
    this._reply = props.reply
    this._repliedAt = props.repliedAt
  }

  private validate(props: IRecruiterSolicitationProps): void {
    if (!props.id) throw new Error('ID de la sollicitation requis')
    if (!props.recruiterName) throw new Error('Nom du recruteur requis')
    if (!props.company) throw new Error('Entreprise requise')
    if (!props.message) throw new Error('Message requis')
  }

  public get id(): string {
    return this.props.id
  }

  public get status(): SolicitationStatus {
    return this._status
  }

  public get replyContent(): string | undefined {
    return this._reply
  }

  public get repliedAt(): string | undefined {
    return this._repliedAt
  }

  public get isUnread(): boolean {
    return this._status === 'unread'
  }

  public get isArchived(): boolean {
    return this._status === 'archived'
  }

  public markAsRead(): void {
    if (this._status === 'unread') {
      this._status = 'read'
    }
  }

  public markAsUnread(): void {
    this._status = 'unread'
  }

  public archive(): void {
    this._status = 'archived'
  }

  public unarchive(): void {
    this._status = 'read'
  }

  public reply(content: string): void {
    const trimmed = content.trim()
    if (!trimmed) throw new Error('Le message de réponse ne peut être vide')
    this._reply = trimmed
    this._repliedAt = new Date().toISOString()
    this._status = 'read'
  }

  public toJSON(): IRecruiterSolicitationProps {
    return {
      ...this.props,
      status: this._status,
      reply: this._reply,
      repliedAt: this._repliedAt,
    }
  }
}
