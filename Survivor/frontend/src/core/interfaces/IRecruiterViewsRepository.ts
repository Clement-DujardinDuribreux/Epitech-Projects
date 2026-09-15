import type { RecruiterViewsStatsVO } from '../domain/value-objects/RecruiterViewsStats.vo'

export interface IRecruiterViewsRepository {
  getStats(): Promise<RecruiterViewsStatsVO>
}
