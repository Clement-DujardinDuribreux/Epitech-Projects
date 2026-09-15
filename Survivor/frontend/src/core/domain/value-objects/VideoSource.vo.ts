export interface ISubtitleTrack {
  src: string
  label: string
  srcLang: string
  default?: boolean
}

export interface IVideoSourceProps {
  videoUrl?: string
  posterUrl: string
  avatarUrl: string
  durationSeconds?: number
  subtitles?: ISubtitleTrack[]
}

export class VideoSourceVO {
  readonly props: IVideoSourceProps

  constructor(props: IVideoSourceProps) {
    this.props = props
  }

  public get videoUrl(): string | undefined {
    return this.props.videoUrl
  }

  public get posterUrl(): string {
    return this.props.posterUrl
  }

  public get avatarUrl(): string {
    return this.props.avatarUrl
  }

  public get durationSeconds(): number | undefined {
    return this.props.durationSeconds
  }

  public get subtitles(): ISubtitleTrack[] {
    return this.props.subtitles ?? []
  }

  public get hasSubtitles(): boolean {
    return (this.props.subtitles?.length ?? 0) > 0
  }

  public toJSON(): IVideoSourceProps {
    return { ...this.props }
  }
}
