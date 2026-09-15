import { CandidateStatsVO, type ICandidateStatsProps } from '../value-objects/CandidateStats.vo'
import { VideoSourceVO, type IVideoSourceProps } from '../value-objects/VideoSource.vo'

export interface ICandidateProfileProps {
  id: string
  fullName: string
  headline: string
  location: string
  age?: number
  bio: string
  skills: string[]
  isJebCertified: boolean
  certificationDate?: string
  media: IVideoSourceProps
  stats: ICandidateStatsProps
}

export class CandidateProfileEntity {
  readonly props: ICandidateProfileProps
  private _stats: CandidateStatsVO
  private _media: VideoSourceVO

  constructor(props: ICandidateProfileProps) {
    this.validate(props)
    this.props = props
    this._stats = new CandidateStatsVO(props.stats)
    this._media = new VideoSourceVO(props.media)
  }

  private validate(props: ICandidateProfileProps): void {
    if (!props.id) throw new Error('ID candidat requis')
    if (!props.fullName) throw new Error('Nom candidat requis')
  }

  public get id(): string {
    return this.props.id
  }

  public get fullName(): string {
    return this.props.fullName
  }

  public get headline(): string {
    return this.props.headline
  }

  public get location(): string {
    return this.props.location
  }

  public get age(): number | undefined {
    return this.props.age
  }

  public get bio(): string {
    return this.props.bio
  }

  public get skills(): string[] {
    return [...this.props.skills]
  }

  public get isJebCertified(): boolean {
    return this.props.isJebCertified
  }

  public get certificationDate(): string | undefined {
    return this.props.certificationDate
  }

  public get media(): VideoSourceVO {
    return this._media
  }

  public get stats(): CandidateStatsVO {
    return this._stats
  }

  public toggleSave(): void {
    this._stats = this._stats.toggleSave()
  }

  public recordSolicitation(): void {
    this._stats = this._stats.incrementSolicitations()
  }

  public toJSON(): ICandidateProfileProps {
    return {
      ...this.props,
      stats: this._stats.toJSON(),
      media: this._media.toJSON(),
    }
  }
}
