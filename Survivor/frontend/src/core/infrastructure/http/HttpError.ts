export class HttpError extends Error {
  public readonly status: number
  public readonly statusText: string
  public readonly responseData: unknown

  constructor(status: number, statusText: string, responseData: unknown = null, message?: string) {
    const defaultMessage = `Erreur HTTP ${status}: ${statusText}`
    super(message || defaultMessage)
    this.name = 'HttpError'
    this.status = status
    this.statusText = statusText
    this.responseData = responseData
    Object.setPrototypeOf(this, HttpError.prototype)
  }

  public get isNotFound(): boolean {
    return this.status === 404
  }

  public get isUnauthorized(): boolean {
    return this.status === 401
  }

  public get isForbidden(): boolean {
    return this.status === 403
  }
}
