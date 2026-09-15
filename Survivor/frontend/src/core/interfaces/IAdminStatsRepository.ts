import type { AdminKpiStatsVO } from '../domain/value-objects/AdminKpiStats.vo'

export interface IAdminStatsRepository {
  getKpis(): Promise<AdminKpiStatsVO>
}
