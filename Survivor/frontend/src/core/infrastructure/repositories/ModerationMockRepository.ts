import { ModerationItemEntity, type IModerationItemProps } from '../../domain/entities/ModerationItem.entity'
import type { IModerationRepository } from '../../interfaces/IModerationRepository'

const STORAGE_KEY = 'profilsactifs_admin_moderation_items_v1'

const DEFAULT_MODERATION_ITEMS: IModerationItemProps[] = [
  {
    id: 'mod-001',
    candidateId: 'maxime-moreau',
    candidateName: 'Maxime Moreau',
    candidateHeadline: 'Développeur Web Junior · Nantes, 44',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e?auto=format&fit=crop&w=200&q=80',
    videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 42,
    status: 'flagged',
    submittedAt: '2026-09-05T14:30:00Z',
    reportedAt: '2026-09-06T09:15:00Z',
    reportReason: 'Bruit de fond perturbateur et propos familiers inadaptés à un cadre de recrutement professionnel.',
    reportSource: 'recruiter',
  },
  {
    id: 'mod-002',
    candidateId: 'sarah-benali',
    candidateName: 'Sarah Benali',
    candidateHeadline: 'Chargée de Relation Client · Lyon, 69',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=200&q=80',
    videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerEscapes.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 36,
    status: 'flagged',
    submittedAt: '2026-09-06T11:20:00Z',
    reportedAt: '2026-09-06T11:22:00Z',
    reportReason: 'Détection IA : niveau sonore trop faible et luminosité insuffisante non conforme au standard de visibilité.',
    reportSource: 'auto_ai',
  },
  {
    id: 'mod-003',
    candidateId: 'david-lefevre',
    candidateName: 'David Lefèvre',
    candidateHeadline: 'Technicien Réseaux & Télécoms · Bordeaux, 33',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?auto=format&fit=crop&w=200&q=80',
    videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerFun.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 50,
    status: 'flagged',
    submittedAt: '2026-09-07T08:10:00Z',
    reportedAt: '2026-09-07T08:11:00Z',
    reportReason: 'Divulgation orale directe d\'adresse personnelle et numéro de sécurité sociale (Alerte RGPD/confidentialité).',
    reportSource: 'auto_ai',
  },
  {
    id: 'mod-004',
    candidateId: 'camille-rousseau',
    candidateName: 'Camille Rousseau',
    candidateHeadline: 'Infirmière DE · Lille, 59',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1559839734-2b71ea197ec2?auto=format&fit=crop&w=200&q=80',
    videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerJoyBlazes.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1559839734-2b71ea197ec2?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 44,
    status: 'pending',
    submittedAt: '2026-09-07T10:00:00Z',
  },
  {
    id: 'mod-005',
    candidateId: 'thomas-girard',
    candidateName: 'Thomas Girard',
    candidateHeadline: 'Électricien Bâtiment · Marseille, 13',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1506794778202-cad84cf45f1d?auto=format&fit=crop&w=200&q=80',
    videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerMeltdowns.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1506794778202-cad84cf45f1d?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 40,
    status: 'pending',
    submittedAt: '2026-09-07T12:45:00Z',
  },
  {
    id: 'mod-006',
    candidateId: 'amina-traore',
    candidateName: 'Amina Traoré',
    candidateHeadline: 'Développeuse Full-Stack · Paris, 75',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=200&q=80',
    videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 45,
    status: 'approved',
    submittedAt: '2026-09-01T09:00:00Z',
    reviewedAt: '2026-09-01T10:15:00Z',
  },
  {
    id: 'mod-007',
    candidateId: 'alexandre-roux',
    candidateName: 'Alexandre Roux',
    candidateHeadline: 'Assistant Commercial · Toulouse, 31',
    candidateAvatarUrl: 'https://images.unsplash.com/photo-1492562080023-ab3db95bfbce?auto=format&fit=crop&w=200&q=80',
    videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerEscapes.mp4',
    posterUrl: 'https://images.unsplash.com/photo-1492562080023-ab3db95bfbce?auto=format&fit=crop&w=800&q=80',
    durationSeconds: 78,
    status: 'rejected',
    submittedAt: '2026-09-03T14:10:00Z',
    reviewedAt: '2026-09-03T16:00:00Z',
    rejectionReason: 'Durée vidéo excessive (> 60 secondes)',
    rejectionComment: 'Votre vidéo fait 78s. Le format d\'insertion impose une durée maximale stricte de 60 secondes.',
  },
]

export class ModerationMockRepository implements IModerationRepository {
  private loadData(): IModerationItemProps[] {
    try {
      const stored = localStorage.getItem(STORAGE_KEY)
      if (stored) {
        return JSON.parse(stored) as IModerationItemProps[]
      }
    } catch (err) {
      console.warn('[ModerationMockRepository] loadData failed:', err)
    }
    this.saveData(DEFAULT_MODERATION_ITEMS)
    return [...DEFAULT_MODERATION_ITEMS]
  }

  private saveData(items: IModerationItemProps[]): void {
    try {
      localStorage.setItem(STORAGE_KEY, JSON.stringify(items))
    } catch (err) {
      console.warn('[ModerationMockRepository] saveData failed:', err)
    }
  }

  public async getAll(): Promise<ModerationItemEntity[]> {
    const raw = this.loadData()
    return raw.map((item) => new ModerationItemEntity(item))
  }

  public async getById(id: string): Promise<ModerationItemEntity | null> {
    const raw = this.loadData()
    const found = raw.find((item) => item.id === id)
    return found ? new ModerationItemEntity(found) : null
  }

  public async save(item: ModerationItemEntity): Promise<ModerationItemEntity> {
    const raw = this.loadData()
    const index = raw.findIndex((entry) => entry.id === item.id)
    if (index >= 0) {
      raw[index] = item.toJSON()
    } else {
      raw.unshift(item.toJSON())
    }
    this.saveData(raw)
    return item
  }

  public async getCounts(): Promise<{
    pending: number
    flagged: number
    approved: number
    rejected: number
    totalToModerate: number
  }> {
    const raw = this.loadData()
    const pending = raw.filter((i) => i.status === 'pending').length
    const flagged = raw.filter((i) => i.status === 'flagged').length
    const approved = raw.filter((i) => i.status === 'approved').length
    const rejected = raw.filter((i) => i.status === 'rejected').length
    return {
      pending,
      flagged,
      approved,
      rejected,
      totalToModerate: pending + flagged,
    }
  }
}
