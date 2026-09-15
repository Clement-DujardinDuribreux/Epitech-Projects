import { ref } from 'vue'
import { ThemeService } from '@/core/services/ThemeService'
import type { IThemePreset } from '@/core/config/theme-presets.config'

const themeService = new ThemeService()

export function useThemeManager() {
  const currentThemeId = ref<string>(themeService.getCurrentThemeId())
  const themes = ref<IThemePreset[]>(themeService.getAvailableThemes())

  function setTheme(id: string): void {
    themeService.applyTheme(id)
    currentThemeId.value = id
  }

  return {
    currentThemeId,
    themes,
    setTheme,
  }
}
