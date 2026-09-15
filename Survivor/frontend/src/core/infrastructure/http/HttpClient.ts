import { HttpError } from './HttpError'

export interface IHttpRequestOptions {
  headers?: Record<string, string>
  params?: Record<string, string | number | boolean | undefined | null>
}

export class HttpClient {
  private readonly baseUrl: string

  constructor(baseUrl?: string) {
    const envUrl = typeof import.meta !== 'undefined' && import.meta.env ? import.meta.env.VITE_API_BASE_URL : undefined
    const fallback = typeof window !== 'undefined' ? '/api' : 'http://localhost:8000'
    this.baseUrl = (baseUrl || envUrl || fallback).replace(/\/$/, '')
  }

  private getAuthToken(): string | null {
    if (typeof localStorage === 'undefined') {
      return null
    }
    return localStorage.getItem('auth_token') || localStorage.getItem('access_token')
  }

  private buildHeaders(customHeaders?: Record<string, string>): Headers {
    const headers = new Headers()
    headers.set('Accept', 'application/json')
    headers.set('Content-Type', 'application/json')

    const token = this.getAuthToken()
    if (token) {
      headers.set('Authorization', `Bearer ${token}`)
    }

    if (customHeaders) {
      Object.entries(customHeaders).forEach(([key, value]) => {
        headers.set(key, value)
      })
    }

    return headers
  }

  private buildUrl(endpoint: string, params?: Record<string, string | number | boolean | undefined | null>): string {
    const cleanEndpoint = endpoint.startsWith('/') ? endpoint : `/${endpoint}`
    const fullUrl = `${this.baseUrl}${cleanEndpoint}`

    if (!params) {
      return fullUrl
    }

    const searchParams = new URLSearchParams()
    Object.entries(params).forEach(([key, value]) => {
      if (value !== undefined && value !== null && value !== '') {
        searchParams.set(key, String(value))
      }
    })

    const queryString = searchParams.toString()
    return queryString ? `${fullUrl}?${queryString}` : fullUrl
  }

  private async request<T>(method: string, endpoint: string, body?: unknown, options?: IHttpRequestOptions): Promise<T> {
    const url = this.buildUrl(endpoint, options?.params)
    const headers = this.buildHeaders(options?.headers)

    const requestInit: RequestInit = {
      method,
      headers,
    }

    if (body !== undefined && body !== null) {
      requestInit.body = JSON.stringify(body)
    }

    const response = await fetch(url, requestInit)

    if (!response.ok) {
      const errorData = await response.json().catch(() => null)
      const message = errorData && typeof errorData === 'object' && 'detail' in errorData
        ? String((errorData as { detail: unknown }).detail)
        : `Erreur HTTP ${response.status}: ${response.statusText}`
      throw new HttpError(response.status, response.statusText, errorData, message)
    }

    if (response.status === 204) {
      return null as T
    }

    return (await response.json()) as T
  }

  public async get<T>(endpoint: string, options?: IHttpRequestOptions): Promise<T> {
    return this.request<T>('GET', endpoint, undefined, options)
  }

  public async post<T>(endpoint: string, body?: unknown, options?: IHttpRequestOptions): Promise<T> {
    return this.request<T>('POST', endpoint, body, options)
  }

  public async put<T>(endpoint: string, body?: unknown, options?: IHttpRequestOptions): Promise<T> {
    return this.request<T>('PUT', endpoint, body, options)
  }

  public async patch<T>(endpoint: string, body?: unknown, options?: IHttpRequestOptions): Promise<T> {
    return this.request<T>('PATCH', endpoint, body, options)
  }

  public async delete<T>(endpoint: string, options?: IHttpRequestOptions): Promise<T> {
    return this.request<T>('DELETE', endpoint, undefined, options)
  }

  public async uploadFile<T>(endpoint: string, formData: FormData, options?: IHttpRequestOptions): Promise<T> {
    const url = this.buildUrl(endpoint, options?.params)
    const headers = new Headers()
    headers.set('Accept', 'application/json')

    const token = this.getAuthToken()
    if (token) {
      headers.set('Authorization', `Bearer ${token}`)
    }

    if (options?.headers) {
      Object.entries(options.headers).forEach(([key, value]) => {
        if (key.toLowerCase() !== 'content-type') {
          headers.set(key, value)
        }
      })
    }

    const response = await fetch(url, {
      method: 'POST',
      headers,
      body: formData,
    })

    if (!response.ok) {
      const errorData = await response.json().catch(() => null)
      const message = errorData && typeof errorData === 'object' && 'detail' in errorData
        ? String((errorData as { detail: unknown }).detail)
        : `Erreur HTTP ${response.status}: ${response.statusText}`
      throw new HttpError(response.status, response.statusText, errorData, message)
    }

    return (await response.json()) as T
  }
}
