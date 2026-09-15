import {
  RecruiterViewsStatsVO,
  type IRecruiterViewsStatsProps,
} from '../../domain/value-objects/RecruiterViewsStats.vo'
import type { IRecruiterViewsRepository } from '../../interfaces/IRecruiterViewsRepository'

const INITIAL_VIEWS_STATS: IRecruiterViewsStatsProps = {
  totalViews: 1420,
  weeklyViews: 84,
  weeklyChangePercent: 18,
  monthlyViews: 312,
  monthlyChangePercent: 24,
  dailyViewsLast7Days: [
    { label: 'Lun 01/09', date: '2026-09-01', count: 12 },
    { label: 'Mar 02/09', date: '2026-09-02', count: 15 },
    { label: 'Mer 03/09', date: '2026-09-03', count: 18 },
    { label: 'Jeu 04/09', date: '2026-09-04', count: 10 },
    { label: 'Ven 05/09', date: '2026-09-05', count: 14 },
    { label: 'Sam 06/09', date: '2026-09-06', count: 7 },
    { label: 'Dim 07/09', date: '2026-09-07', count: 8 },
  ],
  weeklyViewsLast4Weeks: [
    { label: 'Semaine 33', week: '11-17 août', count: 65 },
    { label: 'Semaine 34', week: '18-24 août', count: 72 },
    { label: 'Semaine 35', week: '25-31 août', count: 79 },
    { label: 'Semaine 36', week: '01-07 sept', count: 84 },
  ],
  recentVisits: [
    {
      id: 'visit-1',
      recruiterName: 'Claire Dubois',
      company: 'TechNova Solutions',
      recruiterRole: 'Tech Recruiter',
      visitedAt: 'Il y a 45 minutes',
      viewedVideo: true,
    },
    {
      id: 'visit-2',
      recruiterName: 'Marc Lemaire',
      company: 'InnoVibe Studio',
      recruiterRole: 'CTO',
      visitedAt: 'Il y a 3 heures',
      viewedVideo: true,
    },
    {
      id: 'visit-3',
      recruiterName: 'Alexandre Roux',
      company: 'DataFlow Systems',
      recruiterRole: 'Responsable Pôle Web',
      visitedAt: 'Il y a 6 heures',
      viewedVideo: false,
    },
    {
      id: 'visit-4',
      recruiterName: 'Sophie Bertrand',
      company: 'NextGen Digital',
      recruiterRole: 'Talent Acquisition',
      visitedAt: 'Hier à 16:40',
      viewedVideo: true,
    },
    {
      id: 'visit-5',
      recruiterName: 'Julien Mercier',
      company: 'CloudSphere SAS',
      recruiterRole: 'Directeur de Projet',
      visitedAt: '04 sept. 2026',
      viewedVideo: true,
    },
    {
      id: 'visit-6',
      recruiterName: 'Camille Leroy',
      company: 'ScaleUp Partners',
      recruiterRole: 'Head of People',
      visitedAt: '03 sept. 2026',
      viewedVideo: false,
    },
  ],
}

export class RecruiterViewsMockRepository implements IRecruiterViewsRepository {
  private _stats: RecruiterViewsStatsVO

  constructor() {
    this._stats = new RecruiterViewsStatsVO(INITIAL_VIEWS_STATS)
  }

  public async getStats(): Promise<RecruiterViewsStatsVO> {
    return this._stats
  }
}
