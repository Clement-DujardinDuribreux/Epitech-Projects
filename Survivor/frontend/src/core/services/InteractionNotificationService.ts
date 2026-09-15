export interface IInteractionNotification {
  id: string
  candidateId: string
  candidateName: string
  recruiterCompany: string
  type: 'contact' | 'profile_view'
  message: string
  timestamp: string
  isRead: boolean
}

/**
 * Service de gestion des notifications d'interactions recruteur vers le candidat
 * Exigence 2.3 : « Un système de notification doit informer le candidat lors d'une nouvelle interaction recruteur »
 */
export class InteractionNotificationService {
  private static readonly STORAGE_KEY = 'profilsactifs_candidate_notifications'

  public static getNotifications(): IInteractionNotification[] {
    try {
      const raw = localStorage.getItem(this.STORAGE_KEY)
      return raw ? JSON.parse(raw) : []
    } catch {
      return []
    }
  }

  public static notifyRecruiterContact(params: {
    candidateId: string
    candidateName: string
    recruiterCompany: string
    message: string
  }): IInteractionNotification {
    const notification: IInteractionNotification = {
      id: 'notif_' + Date.now() + '_' + Math.random().toString(36).substring(2, 7),
      candidateId: params.candidateId,
      candidateName: params.candidateName,
      recruiterCompany: params.recruiterCompany || 'Entreprise partenaire',
      type: 'contact',
      message: params.message,
      timestamp: new Date().toISOString(),
      isRead: false,
    }

    const current = this.getNotifications()
    const updated = [notification, ...current].slice(0, 50)
    try {
      localStorage.setItem(this.STORAGE_KEY, JSON.stringify(updated))
    } catch (error) {
      console.warn('Impossible de sauvegarder la notification dans le stockage local:', error)
    }

    if (typeof window !== 'undefined') {
      window.dispatchEvent(
        new CustomEvent('recruiter-interaction', { detail: notification })
      )
    }

    return notification
  }

  public static markAsRead(id: string): void {
    const current = this.getNotifications()
    const updated = current.map((item) =>
      item.id === id ? { ...item, isRead: true } : item
    )
    try {
      localStorage.setItem(this.STORAGE_KEY, JSON.stringify(updated))
    } catch (error) {
      console.warn('Impossible de mettre à jour le statut de la notification:', error)
    }
  }

  public static getUnreadCount(): number {
    return this.getNotifications().filter((n) => !n.isRead).length
  }
}
