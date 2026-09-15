export interface IDailyViewData {
  label: string
  date: string
  count: number
}

export interface IWeeklyViewData {
  label: string
  week: string
  count: number
}

export interface IRecentRecruiterVisit {
  id: string
  recruiterName: string
  company: string
  recruiterRole?: string
  visitedAt: string
  viewedVideo: boolean
}

export interface IRecruiterViewsStatsProps {
  totalViews: number
  weeklyViews: number
  weeklyChangePercent: number
  monthlyViews: number
  monthlyChangePercent: number
  dailyViewsLast7Days: IDailyViewData[]
  weeklyViewsLast4Weeks: IWeeklyViewData[]
  recentVisits: IRecentRecruiterVisit[]
}

export class RecruiterViewsStatsVO {
  readonly props: IRecruiterViewsStatsProps

  constructor(props: IRecruiterViewsStatsProps) {
    this.props = { ...props }
  }

  public get totalViews(): number {
    return this.props.totalViews
  }

  public get weeklyViews(): number {
    return this.props.weeklyViews
  }

  public get weeklyChangePercent(): number {
    return this.props.weeklyChangePercent
  }

  public get monthlyViews(): number {
    return this.props.monthlyViews
  }

  public get monthlyChangePercent(): number {
    return this.props.monthlyChangePercent
  }

  public get dailyViewsLast7Days(): IDailyViewData[] {
    return [...this.props.dailyViewsLast7Days]
  }

  public get weeklyViewsLast4Weeks(): IWeeklyViewData[] {
    return [...this.props.weeklyViewsLast4Weeks]
  }

  public get recentVisits(): IRecentRecruiterVisit[] {
    return [...this.props.recentVisits]
  }

  public formatNumber(value: number): string {
    return new Intl.NumberFormat('fr-FR').format(value)
  }

  public formatPercent(value: number): string {
    const sign = value > 0 ? '+' : ''
    return `${sign}${value}%`
  }

  public toJSON(): IRecruiterViewsStatsProps {
    return { ...this.props }
  }
}
