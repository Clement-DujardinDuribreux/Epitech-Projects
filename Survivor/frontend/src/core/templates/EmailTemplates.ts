/**
 * Modèles d'e-mails transactionnels ProfilsActifs
 * Chaque modèle intègre obligatoirement l'avertissement légal en pied de page.
 */

export const LEGAL_EMAIL_DISCLAIMER_TEXT =
  'Démonstrateur technique, ne constitue pas un service public en exploitation.'

export const LEGAL_EMAIL_DISCLAIMER_HTML = `
  <div style="margin-top: 32px; padding-top: 16px; border-top: 1px solid #272738; text-align: center; font-size: 11px; color: #94a3b8; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;">
    <p style="margin: 0 0 4px 0; font-weight: 600; color: #f5c842;">
      Avertissement Légal
    </p>
    <p style="margin: 0;">
      « Démonstrateur technique, ne constitue pas un service public en exploitation. »
    </p>
    <p style="margin: 8px 0 0 0; font-size: 10px; color: #64748b;">
      © 2026 ProfilsActifs · Plateforme de mise en relation professionnelle
    </p>
  </div>
`

export interface IEmailMessage {
  subject: string
  text: string
  html: string
}

export class EmailTemplates {
  /**
   * E-mail de notification de contact recruteur vers candidat
   */
  public static recruiterContactCandidate(params: {
    candidateName: string
    recruiterCompany: string
    message: string
  }): IEmailMessage {
    const subject = `Nouveau message d'un recruteur sur ProfilsActifs`
    const text = `Bonjour ${params.candidateName},\n\nUn recruteur (${params.recruiterCompany}) a consulté votre profil vidéo et vous a envoyé le message suivant :\n\n"${params.message}"\n\nConnectez-vous sur votre espace ProfilsActifs pour lui répondre.\n\n---\n${LEGAL_EMAIL_DISCLAIMER_TEXT}`

    const html = `
      <div style="background-color: #0b0b12; color: #f8fafc; padding: 32px; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; max-width: 600px; margin: 0 auto; border-radius: 12px;">
        <h2 style="color: #ffffff; margin-top: 0;">Profils<span style="color: #f5c842;">Actifs</span></h2>
        <p style="font-size: 15px; line-height: 1.5;">Bonjour <strong>${params.candidateName}</strong>,</p>
        <p style="font-size: 14px; line-height: 1.5; color: #cbd5e1;">
          L'entreprise <strong>${params.recruiterCompany}</strong> a apprécié votre présentation vidéo et souhaite échanger avec vous.
        </p>
        <div style="background-color: #161526; border-left: 3px solid #863bff; padding: 14px; margin: 20px 0; border-radius: 6px; font-style: italic; color: #e2e8f0;">
          "${params.message}"
        </div>
        ${LEGAL_EMAIL_DISCLAIMER_HTML}
      </div>
    `

    return { subject, text, html }
  }

  /**
   * E-mail de bienvenue à l'inscription
   */
  public static welcomeUser(params: { userName: string; role: 'candidat' | 'recruteur' }): IEmailMessage {
    const roleLabel = params.role === 'candidat' ? 'candidat' : 'recruteur'
    const subject = `Bienvenue sur ProfilsActifs`
    const text = `Bonjour ${params.userName},\n\nBienvenue sur ProfilsActifs en tant que ${roleLabel}.\n\n---\n${LEGAL_EMAIL_DISCLAIMER_TEXT}`

    const html = `
      <div style="background-color: #0b0b12; color: #f8fafc; padding: 32px; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; max-width: 600px; margin: 0 auto; border-radius: 12px;">
        <h2 style="color: #ffffff; margin-top: 0;">Profils<span style="color: #f5c842;">Actifs</span></h2>
        <p style="font-size: 15px; line-height: 1.5;">Bonjour <strong>${params.userName}</strong>,</p>
        <p style="font-size: 14px; line-height: 1.5; color: #cbd5e1;">
          Votre compte ${roleLabel} a été créé avec succès sur ProfilsActifs.
        </p>
        ${LEGAL_EMAIL_DISCLAIMER_HTML}
      </div>
    `

    return { subject, text, html }
  }

  /**
   * E-mail de validation de certification d'aptitudes
   */
  public static certificationAchieved(params: { candidateName: string; score: number }): IEmailMessage {
    const subject = `Félicitations pour votre certification sur ProfilsActifs`
    const text = `Bonjour ${params.candidateName},\n\nVous avez complété avec succès votre questionnaire de certification avec un score de ${params.score}%.\n\n---\n${LEGAL_EMAIL_DISCLAIMER_TEXT}`

    const html = `
      <div style="background-color: #0b0b12; color: #f8fafc; padding: 32px; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; max-width: 600px; margin: 0 auto; border-radius: 12px;">
        <h2 style="color: #ffffff; margin-top: 0;">Profils<span style="color: #f5c842;">Actifs</span></h2>
        <p style="font-size: 15px; line-height: 1.5;">Bonjour <strong>${params.candidateName}</strong>,</p>
        <p style="font-size: 14px; line-height: 1.5; color: #cbd5e1;">
          Félicitations ! Vous avez validé votre questionnaire de certification des aptitudes avec un score de <strong>${params.score}%</strong>. Votre badge est maintenant visible par les recruteurs.
        </p>
        ${LEGAL_EMAIL_DISCLAIMER_HTML}
      </div>
    `

    return { subject, text, html }
  }
}

