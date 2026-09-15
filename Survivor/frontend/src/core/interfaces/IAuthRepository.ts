export type UserRole = 'job_seeker' | 'recruiter' | 'admin'

export interface IAuthUser {
  id: string
  email: string
  firstname: string
  name: string
  role: UserRole
  localisation?: string | null
  birthdate?: string | null
  certificated?: boolean | number | null
}

export interface ILoginCredentials {
  email: string
  password: string
}

export interface IRegisterPayload {
  email: string
  password: string
  firstname: string
  name: string
  role: 'job_seeker' | 'recruiter'
  localisation?: string
  birthdate?: string
}

export interface IAuthResponse {
  access_token: string
  user: IAuthUser
}

export interface IAuthRepository {
  login(credentials: ILoginCredentials): Promise<IAuthResponse>
  register(payload: IRegisterPayload): Promise<IAuthResponse>
  getMe(): Promise<IAuthUser>
  deleteAccount(): Promise<void>
}
