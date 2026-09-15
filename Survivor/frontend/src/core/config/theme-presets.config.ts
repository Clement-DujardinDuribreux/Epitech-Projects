export interface IThemePreset {
  id: string
  name: string
  icon: string
  cssClass: string
  description: string
  accentColor: string
}

export const availableThemes: IThemePreset[] = [
  {
    id: 'profilsactifs',
    name: 'ProfilsActifs Dark Neon',
    icon: '✨',
    cssClass: 'theme-profilsactifs',
    description: 'Fond sombre #0b0b12, accents violet et or, typographies modernes et contrastées',
    accentColor: '#863bff',
  },
]
