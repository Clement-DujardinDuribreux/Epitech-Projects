import { HttpClient } from '../http/HttpClient'
import type {
  IAuthRepository,
  IAuthResponse,
  IAuthUser,
  ILoginCredentials,
  IRegisterPayload,
} from '../../interfaces/IAuthRepository'

export class AuthHttpRepository implements IAuthRepository {
  private readonly httpClient: HttpClient

  constructor(httpClient: HttpClient = new HttpClient()) {
    this.httpClient = httpClient
  }

  public async login(credentials: ILoginCredentials): Promise<IAuthResponse> {
    return this.httpClient.post<IAuthResponse>('/auth/login', credentials)
  }

  public async register(payload: IRegisterPayload): Promise<IAuthResponse> {
    return this.httpClient.post<IAuthResponse>('/auth/register', payload)
  }

  public async getMe(): Promise<IAuthUser> {
    return this.httpClient.get<IAuthUser>('/auth/me')
  }

  public async deleteAccount(): Promise<void> {
    await this.httpClient.delete<void>('/auth/delete')
  }
}
