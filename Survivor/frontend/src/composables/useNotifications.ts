import { ref, computed } from 'vue'

export interface INotification {
  id: string
  personName: string
  personAvatarUrl: string
  message: string
  date: string
  read: boolean
}

const notifications = ref<INotification[]>([
  {
    id: 'n1',
    personName: 'Camille Roy — TechCorp',
    personAvatarUrl: 'https://i.pravatar.cc/100?img=32',
    message: 'a consulté votre profil et souhaite échanger avec vous.',
    date: '2026-09-05T10:12:00Z',
    read: false,
  },
  {
    id: 'n2',
    personName: 'Camille Roy — TechCorp',
    personAvatarUrl: 'https://i.pravatar.cc/100?img=32',
    message: 'vous a envoyé un second message.',
    date: '2026-09-06T09:00:00Z',
    read: false,
  },
  {
    id: 'n3',
    personName: 'Julien Faure — Studio Nova',
    personAvatarUrl: 'https://i.pravatar.cc/100?img=15',
    message: "a validé votre certification et propose un entretien.",
    date: '2026-09-06T16:45:00Z',
    read: false,
  },
])

const unreadCount = computed(() => notifications.value.filter((n) => !n.read).length)

const unreadPersonCount = computed(
  () => new Set(notifications.value.filter((n) => !n.read).map((n) => n.personName)).size,
)

function markAllAsRead(): void {
  notifications.value = notifications.value.map((n) => ({ ...n, read: true }))
}

export function useNotifications() {
  return { notifications, unreadCount, unreadPersonCount, markAllAsRead }
}
