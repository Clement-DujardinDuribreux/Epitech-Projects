export interface ICandidateStatsProps {
  recruiterViewsCount: number
  solicitationsCount: number
  isSaved: boolean
}

export class CandidateStatsVO {
  readonly props: ICandidateStatsProps

  constructor(props: ICandidateStatsProps) {
    this.props = props
  }

  public get recruiterViewsCount(): number {
    return this.props.recruiterViewsCount
  }

  public get solicitationsCount(): number {
    return this.props.solicitationsCount
  }

  public get isSaved(): boolean {
    return this.props.isSaved
  }

  public toggleSave(): CandidateStatsVO {
    return new CandidateStatsVO({
      ...this.props,
      isSaved: !this.props.isSaved,
    })
  }

  public incrementSolicitations(): CandidateStatsVO {
    return new CandidateStatsVO({
      ...this.props,
      solicitationsCount: this.props.solicitationsCount + 1,
    })
  }

  public format(value: number): string {
    if (value >= 1_000_000) return `${(value / 1_000_000).toFixed(1)}M`
    if (value >= 10_000) return `${(value / 1_000).toFixed(1)}k`
    if (value >= 1_000) return `${(value / 1_000).toFixed(1)}K`
    return value.toString()
  }

  public toJSON(): ICandidateStatsProps {
    return { ...this.props }
  }
}
