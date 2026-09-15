export interface IAdminKpiStatsProps {
  totalPublishedProfiles: number
  totalCertificationsIssued: number
  totalSolicitationsTransmitted: number
  totalVideosToModerate: number
  flaggedVideosCount: number
  pendingVideosCount: number
  certificationSuccessRate: number
  averageQuizScore: number
  profilesWithVideoCount: number
  sectorDistribution: Record<string, number>
}

export class AdminKpiStatsVO {
  private readonly _props: IAdminKpiStatsProps

  constructor(props: IAdminKpiStatsProps) {
    this.validate(props)
    this._props = {
      ...props,
      sectorDistribution: { ...props.sectorDistribution },
    }
  }

  private validate(props: IAdminKpiStatsProps): void {
    if (props.totalPublishedProfiles < 0) {
      throw new Error('Le nombre de profils publiés ne peut pas être négatif')
    }
    if (props.totalCertificationsIssued < 0) {
      throw new Error('Le nombre de certifications ne peut pas être négatif')
    }
    if (props.totalSolicitationsTransmitted < 0) {
      throw new Error('Le nombre de sollicitations ne peut pas être négatif')
    }
    if (props.totalVideosToModerate < 0) {
      throw new Error('Le nombre de vidéos à modérer ne peut pas être négatif')
    }
  }

  public get totalPublishedProfiles(): number {
    return this._props.totalPublishedProfiles
  }

  public get totalCertificationsIssued(): number {
    return this._props.totalCertificationsIssued
  }

  public get totalSolicitationsTransmitted(): number {
    return this._props.totalSolicitationsTransmitted
  }

  public get totalVideosToModerate(): number {
    return this._props.totalVideosToModerate
  }

  public get flaggedVideosCount(): number {
    return this._props.flaggedVideosCount
  }

  public get pendingVideosCount(): number {
    return this._props.pendingVideosCount
  }

  public get certificationSuccessRate(): number {
    return this._props.certificationSuccessRate
  }

  public get averageQuizScore(): number {
    return this._props.averageQuizScore
  }

  public get profilesWithVideoCount(): number {
    return this._props.profilesWithVideoCount
  }

  public get sectorDistribution(): Record<string, number> {
    return { ...this._props.sectorDistribution }
  }

  public toJSON(): IAdminKpiStatsProps {
    return {
      ...this._props,
      sectorDistribution: { ...this._props.sectorDistribution },
    }
  }
}
