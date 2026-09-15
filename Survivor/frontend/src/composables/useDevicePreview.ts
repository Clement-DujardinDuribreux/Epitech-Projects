import { ref } from 'vue'

export type DeviceFormat = 'mobile' | 'tablet' | 'desktop'

export const MOBILE_BREAKPOINT_PX = 640
export const TABLET_BREAKPOINT_PX = 1024

function detectDeviceFormat(): DeviceFormat {
  if (typeof window !== 'undefined') {
    if (window.innerWidth < MOBILE_BREAKPOINT_PX) {
      return 'mobile'
    }
    if (window.innerWidth < TABLET_BREAKPOINT_PX) {
      return 'tablet'
    }
  }
  return 'desktop'
}

const currentFormat = ref<DeviceFormat>(detectDeviceFormat())
let hasUserOverridden = false

if (typeof window !== 'undefined') {
  window.addEventListener('resize', () => {
    if (!hasUserOverridden) {
      currentFormat.value = detectDeviceFormat()
    }
  })
}

export function useDevicePreview() {
  function setFormat(format: DeviceFormat): void {
    hasUserOverridden = true
    currentFormat.value = format
  }

  function resetToAuto(): void {
    hasUserOverridden = false
    currentFormat.value = detectDeviceFormat()
  }

  return {
    currentFormat,
    setFormat,
    resetToAuto,
  }
}
