export interface IBrandingConfig {
  appName: string
  appShortName: string
  appTagline: string
  // ministryName: string
  badgeName: string
  badgeDescription: string
  contactCtaText: string
}

export const defaultBranding: IBrandingConfig = {
  appName: 'ProfilsActifs',
  appShortName: 'ProfilsActifs',
  appTagline: 'La plateforme de recrutement vidéo & certification des aptitudes',
  badgeName: 'Certification',
  badgeDescription: 'Certification des aptitudes transversales',
  contactCtaText: 'Contacter le candidat',
}
