import { availableThemes, type IThemePreset } from '../config/theme-presets.config'

export class ThemeService {
  private currentThemeId: string = 'profilsactifs'

  public getAvailableThemes(): IThemePreset[] {
    return availableThemes
  }

  public getTheme(id: string): IThemePreset {
    return availableThemes.find((t) => t.id === id) ?? availableThemes[0]
  }

  public applyTheme(id: string): void {
    const target = this.getTheme(id)
    if (typeof document !== 'undefined') {
      const root = document.documentElement
      const body = document.body

      availableThemes.forEach((t) => {
        root.classList.remove(t.cssClass)
        body.classList.remove(t.cssClass)
      })

      root.classList.add(target.cssClass)
      body.classList.add(target.cssClass)
      root.classList.add('dark')
    }
    this.currentThemeId = id
  }

  public getCurrentThemeId(): string {
    return this.currentThemeId
  }
}
