import { AdminKpiStatsVO } from '../../domain/value-objects/AdminKpiStats.vo'
import type { IAdminStatsRepository } from '../../interfaces/IAdminStatsRepository'
import type { ICandidateRepository } from '../../interfaces/ICandidateRepository'
import type { IModerationRepository } from '../../interfaces/IModerationRepository'
import { CandidateHttpRepository } from './CandidateHttpRepository'
import { ModerationMockRepository } from './ModerationMockRepository'

export class AdminStatsMockRepository implements IAdminStatsRepository {
  private readonly _candidateRepo: ICandidateRepository
  private readonly _moderationRepo: IModerationRepository

  constructor(
    candidateRepo: ICandidateRepository = new CandidateHttpRepository(),
    moderationRepo: IModerationRepository = new ModerationMockRepository(),
  ) {
    this._candidateRepo = candidateRepo
    this._moderationRepo = moderationRepo
  }

  public async getKpis(): Promise<AdminKpiStatsVO> {
    const candidatesResult = await this._candidateRepo.getAll({ limit: 100 })
    const candidates = candidatesResult.items
    const moderationCounts = await this._moderationRepo.getCounts()

    const totalPublishedProfiles = candidates.length
    const certifiedCount = candidates.filter((c) => c.isJebCertified).length
    const totalSolicitationsTransmitted = candidates.reduce(
      (sum, c) => sum + (c.stats.solicitationsCount ?? 0),
      0,
    )

    const profilesWithVideo = candidates.filter((c) => !!c.media.videoUrl).length
    const successRate = totalPublishedProfiles > 0
      ? Math.round((certifiedCount / totalPublishedProfiles) * 100)
      : 80

    const sectorDistribution: Record<string, number> = {}
    for (const c of candidates) {
      const mainSector = c.skills[0] || 'Autre'
      sectorDistribution[mainSector] = (sectorDistribution[mainSector] || 0) + 1
    }

    return new AdminKpiStatsVO({
      totalPublishedProfiles,
      totalCertificationsIssued: certifiedCount,
      totalSolicitationsTransmitted,
      totalVideosToModerate: moderationCounts.totalToModerate,
      flaggedVideosCount: moderationCounts.flagged,
      pendingVideosCount: moderationCounts.pending,
      certificationSuccessRate: successRate,
      averageQuizScore: 84,
      profilesWithVideoCount: profilesWithVideo,
      sectorDistribution,
    })
  }
}
