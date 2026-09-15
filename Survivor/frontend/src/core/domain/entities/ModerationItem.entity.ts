export type ModerationStatus = 'pending' | 'flagged' | 'approved' | 'rejected'
export type ReportSource = 'auto_ai' | 'recruiter' | 'user'

export interface IModerationItemProps {
  id: string
  candidateId: string
  candidateName: string
  candidateHeadline: string
  candidateAvatarUrl: string
  videoUrl: string
  posterUrl: string
  durationSeconds: number
  status: ModerationStatus
  submittedAt: string
  reportedAt?: string
  reportReason?: string
  reportSource?: ReportSource
  rejectionReason?: string
  rejectionComment?: string
  reviewedAt?: string
}

export class ModerationItemEntity {
  public readonly props: Readonly<IModerationItemProps>

  constructor(props: IModerationItemProps) {
    this.validate(props)
    this.props = { ...props }
  }

  private validate(props: IModerationItemProps): void {
    if (!props.id) {
      throw new Error('Identifiant de modération requis')
    }
    if (!props.candidateId) {
      throw new Error('Identifiant candidat requis')
    }
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

  public get videoUrl(): string {
    return this.props.videoUrl
  }

  public get posterUrl(): string {
    return this.props.posterUrl
  }

  public get durationSeconds(): number {
    return this.props.durationSeconds
  }

  public get status(): ModerationStatus {
    return this.props.status
  }

  public get submittedAt(): string {
    return this.props.submittedAt
  }

  public get reportedAt(): string | undefined {
    return this.props.reportedAt
  }

  public get reportReason(): string | undefined {
    return this.props.reportReason
  }

  public get reportSource(): ReportSource | undefined {
    return this.props.reportSource
  }

  public get rejectionReason(): string | undefined {
    return this.props.rejectionReason
  }

  public get rejectionComment(): string | undefined {
    return this.props.rejectionComment
  }

  public get reviewedAt(): string | undefined {
    return this.props.reviewedAt
  }

  public approve(): void {
    const mutableProps = this.props as IModerationItemProps
    mutableProps.status = 'approved'
    mutableProps.reviewedAt = new Date().toISOString()
  }

  public reject(reason: string, comment?: string): void {
    if (!reason || reason.trim().length === 0) {
      throw new Error('Le motif de rejet est obligatoire')
    }
    const mutableProps = this.props as IModerationItemProps
    mutableProps.status = 'rejected'
    mutableProps.rejectionReason = reason
    mutableProps.rejectionComment = comment
    mutableProps.reviewedAt = new Date().toISOString()
  }

  public toJSON(): IModerationItemProps {
    return { ...this.props }
  }
}
