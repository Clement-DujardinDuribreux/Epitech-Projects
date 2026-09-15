export type LegalSectionId =
  | 'cgu'
  | 'rgpd'
  | 'rgaa'
  | 'filtrage'
  | 'souverainete'

export interface ILegalDocumentProps {
  id: LegalSectionId
  title: string
  shortTitle: string
  icon: string
  lastUpdated: string
  summary: string
  contentMarkdown: string
  badgeText?: string
}

export class LegalDocumentEntity {
  readonly props: ILegalDocumentProps

  constructor(props: ILegalDocumentProps) {
    this.props = props
  }

  public get id(): LegalSectionId {
    return this.props.id
  }

  public get title(): string {
    return this.props.title
  }

  public get shortTitle(): string {
    return this.props.shortTitle
  }

  public get icon(): string {
    return this.props.icon
  }

  public get lastUpdated(): string {
    return this.props.lastUpdated
  }

  public get summary(): string {
    return this.props.summary
  }

  public get contentMarkdown(): string {
    return this.props.contentMarkdown
  }

  public get badgeText(): string | undefined {
    return this.props.badgeText
  }

  public toJSON(): ILegalDocumentProps {
    return { ...this.props }
  }
}
