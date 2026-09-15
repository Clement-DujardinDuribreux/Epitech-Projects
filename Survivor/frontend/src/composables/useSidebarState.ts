import { ref, watch } from 'vue'

const STORAGE_KEY = 'profilsactifs_sidebar_collapsed'
const isCollapsed = ref<boolean>(false)

// Initialisation depuis localStorage si disponible
if (typeof window !== 'undefined') {
  const saved = localStorage.getItem(STORAGE_KEY)
  if (saved !== null) {
    isCollapsed.value = saved === 'true'
  }
}

watch(isCollapsed, (val) => {
  if (typeof window !== 'undefined') {
    localStorage.setItem(STORAGE_KEY, String(val))
  }
})

export function useSidebarState() {
  function toggleSidebar(): void {
    isCollapsed.value = !isCollapsed.value
  }

  function setCollapsed(val: boolean): void {
    isCollapsed.value = val
  }

  return {
    isCollapsed,
    toggleSidebar,
    setCollapsed,
  }
}
