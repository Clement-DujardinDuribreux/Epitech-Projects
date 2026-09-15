import type { ICandidateRepository, IPaginationParams, IPaginatedResult } from '../../interfaces/ICandidateRepository'
import { CandidateProfileEntity, type ICandidateProfileProps } from '../../domain/entities/CandidateProfile.entity'
import type { IVideoSourceProps } from '../../domain/value-objects/VideoSource.vo'
import { CandidateMockRepository } from './CandidateMockRepository'
import { HttpClient } from '../http/HttpClient'
import { HttpError } from '../http/HttpError'

const DEFAULT_VIDEO_URL = 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4'

const PROFESSIONAL_PORTRAITS = [
  'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2',
  'https://images.unsplash.com/photo-1534528741775-53994a69daeb',
  'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d',
  'https://images.unsplash.com/photo-1580489944761-15a19d654956',
  'https://images.unsplash.com/photo-1500648767791-00dcc994a43e',
  'https://images.unsplash.com/photo-1506794778202-cad84cf45f1d',
  'https://images.unsplash.com/photo-1492562080023-ab3db95bfbce',
  'https://images.unsplash.com/photo-1559839734-2b71ea197ec2',
]

function getDeterministicPortrait(seed: string): string {
  let hash = 0
  for (let i = 0; i < seed.length; i++) {
    hash = (hash << 5) - hash + seed.charCodeAt(i)
    hash |= 0
  }
  const index = Math.abs(hash) % PROFESSIONAL_PORTRAITS.length
  return PROFESSIONAL_PORTRAITS[index]
}

export interface IBackendRawUser {
  id?: string
  firstname?: string
  name?: string
  email?: string
  localisation?: string
  certificated?: number | boolean
  birthdate?: string
  competences?: string[]
  competence?: string
  sectors?: string[]
  sector?: string
  medias?: string[]
  link?: string
  videoUrl?: string
  posterUrl?: string
  avatar_url?: string
  bio?: string
  headline?: string
}

interface IFeedUsersResponse {
  data?: IBackendRawUser[]
  total?: number
}

export class CandidateHttpRepository implements ICandidateRepository {
  private readonly httpClient: HttpClient
  private readonly fallbackRepo: ICandidateRepository

  constructor(
    httpClient: HttpClient = new HttpClient(),
    fallbackRepo: ICandidateRepository = new CandidateMockRepository()
  ) {
    this.httpClient = httpClient
    this.fallbackRepo = fallbackRepo
  }

  private resolveFullName(raw: IBackendRawUser): string {
    const first = raw.firstname || ''
    const last = raw.name || ''
    const full = `${first} ${last}`.trim()
    return full.length > 0 ? full : 'Candidat Anonyme'
  }

  private resolveSkills(raw: IBackendRawUser, sector: string): string[] {
    if (Array.isArray(raw.competences) && raw.competences.length > 0) {
      return raw.competences
    }
    if (raw.competence) {
      return [raw.competence]
    }
    if (sector.includes('Tech') || sector.includes('Info')) {
      return ['Vue.js', 'TypeScript', 'Tailwind CSS', 'Git']
    }
    if (sector.includes('Commerce') || sector.includes('Vente')) {
      return ['Relation client', 'Négociation', 'Merchandising', 'Gestion de caisse']
    }
    if (sector.includes('Design')) {
      return ['UI/UX Design', 'Figma', 'Design System', 'Accessibilité RGAA']
    }
    return ['Travail en équipe', 'Polyvalence', 'Communication professionnelle', 'Rigueur']
  }

  private resolveMedia(raw: IBackendRawUser, rawId: string): IVideoSourceProps {
    const video = (Array.isArray(raw.medias) && raw.medias[0]) || raw.link || raw.videoUrl || DEFAULT_VIDEO_URL
    const basePortrait = getDeterministicPortrait(rawId || raw.firstname || 'candidat')
    const poster = raw.posterUrl || `${basePortrait}?auto=format&fit=crop&w=800&q=80`
    const avatar = raw.avatar_url || `${basePortrait}?auto=format&fit=crop&w=200&q=80`

    return {
      videoUrl: video,
      posterUrl: poster,
      avatarUrl: avatar,
      durationSeconds: 45,
      subtitles: [],
    }
  }

  private mapBackendToEntity(raw: IBackendRawUser, index: number): CandidateProfileEntity {
    const rawId = raw.id || `candidate-${index + 1}`
    const sector = (Array.isArray(raw.sectors) && raw.sectors[0]) || raw.sector || 'Tech & Numérique'
    const loc = raw.localisation || 'France'
    const skills = this.resolveSkills(raw, sector)

    const props: ICandidateProfileProps = {
      id: rawId,
      fullName: this.resolveFullName(raw),
      headline: raw.headline || `${sector} · ${loc}`,
      location: loc,
      bio: raw.bio || "Demandeur d'emploi certifié et disponible sur ProfilsActifs.",
      skills,
      isJebCertified: Boolean(raw.certificated),
      media: this.resolveMedia(raw, rawId),
      stats: {
        recruiterViewsCount: 140,
        solicitationsCount: 4,
        isSaved: false,
      },
    }
    return new CandidateProfileEntity(props)
  }

  public async getAll(params?: IPaginationParams): Promise<IPaginatedResult<CandidateProfileEntity>> {
    const limit = params?.limit || 20
    const page = params?.page || 1

    try {
      const queryParams: Record<string, string | number | boolean | undefined> = {
        nb_users: limit,
        limit,
        page,
      }

      if (params?.query) queryParams.localisation = params.query
      if (params?.skill) queryParams.competences = params.skill
      if (params?.jebOnly) queryParams.certified = true

      const response = await this.httpClient.get<IFeedUsersResponse | IBackendRawUser[]>('/feed/users', {
        params: queryParams,
      })

      const rawList: IBackendRawUser[] = Array.isArray(response) ? response : (response.data || [])
      const total = (!Array.isArray(response) && typeof response.total === 'number') ? response.total : rawList.length

      let items = rawList.map((item, idx) => this.mapBackendToEntity(item, idx))
      if (params?.ids && params.ids.length > 0) {
        items = items.filter((item) => params.ids!.includes(item.id))
      }
      const totalPages = Math.max(1, Math.ceil(total / limit))

      return {
        items,
        total: params?.ids ? items.length : total,
        page,
        limit,
        totalPages,
        hasMore: page < totalPages,
      }
    } catch {
      return await this.fallbackRepo.getAll(params)
    }
  }

  public async getById(id: string): Promise<CandidateProfileEntity | null> {
    try {
      // Enregistrement de la consultation de profil (recruteur / visiteur)
      try {
        await this.httpClient.post<void>(`/feed/user/${encodeURIComponent(id)}/view`)
      } catch (viewErr) {
        console.warn('[CandidateHttpRepository] Erreur enregistrement vue profil:', viewErr)
      }

      const data = await this.httpClient.get<IBackendRawUser>(`/feed/user/${encodeURIComponent(id)}`)
      if (!data) {
        return await this.fallbackRepo.getById(id)
      }
      return this.mapBackendToEntity({ ...data, id }, 0)
    } catch (error: unknown) {
      if (error instanceof HttpError && error.isNotFound) {
        return await this.fallbackRepo.getById(id)
      }
      return await this.fallbackRepo.getById(id)
    }
  }

  public async save(candidate: CandidateProfileEntity): Promise<CandidateProfileEntity> {
    try {
      const parts = candidate.fullName.trim().split(' ')
      const firstname = parts[0] || ''
      const name = parts.slice(1).join(' ') || ''

      await this.httpClient.put('/job_seeker/profile', {
        firstname,
        name,
        localisation: candidate.location,
      })

      return candidate
    } catch {
      return await this.fallbackRepo.save(candidate)
    }
  }
}
