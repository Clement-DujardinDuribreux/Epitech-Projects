import type { AdminKpiStatsVO } from '../domain/value-objects/AdminKpiStats.vo'
import type { IAdminStatsRepository } from '../interfaces/IAdminStatsRepository'
import type { IModerationRepository } from '../interfaces/IModerationRepository'
import type { ModerationItemEntity } from '../domain/entities/ModerationItem.entity'
import { AdminStatsMockRepository } from '../infrastructure/repositories/AdminStatsMockRepository'
import { ModerationMockRepository } from '../infrastructure/repositories/ModerationMockRepository'

export interface IAdminActivityLog {
  id: string
  type: 'video_moderated' | 'certification_issued' | 'profile_created' | 'solicitation_sent'
  title: string
  description: string
  timestamp: string
}

export class AdminDashboardService {
  private readonly _statsRepo: IAdminStatsRepository
  private readonly _moderationRepo: IModerationRepository

  constructor(
    statsRepo: IAdminStatsRepository = new AdminStatsMockRepository(),
    moderationRepo: IModerationRepository = new ModerationMockRepository(),
  ) {
    this._statsRepo = statsRepo
    this._moderationRepo = moderationRepo
  }

  public async getDashboardData(): Promise<{
    kpis: AdminKpiStatsVO
    urgentModerationItems: ModerationItemEntity[]
    recentActivities: IAdminActivityLog[]
  }> {
    const [kpis, allModeration] = await Promise.all([
      this._statsRepo.getKpis(),
      this._moderationRepo.getAll(),
    ])

    const urgentModerationItems = allModeration
      .filter((item) => item.status === 'flagged' || item.status === 'pending')
      .slice(0, 4)

    // Pas de source de données réelle pour le journal d'audit : liste vide plutôt que
    // des événements fictifs présentés comme "Temps Réel".
    const recentActivities: IAdminActivityLog[] = []

    return {
      kpis,
      urgentModerationItems,
      recentActivities,
    }
  }
}
