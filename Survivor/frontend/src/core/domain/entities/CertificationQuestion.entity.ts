export type CertificationAnswer = 'Vrai' | 'Faux' | 'Oui' | 'Non'

export type CertificationCategory =
  | 'Rigueur professionnelle'
  | 'Esprit d\'équipe & Communication'
  | 'Adaptabilité & Résolution de problèmes'
  | 'Sens du service & Engagement'

export interface CertificationQuestion {
  id: number
  categorie: CertificationCategory | string
  type: 'Vrai/Faux' | 'Oui/Non' | string
  question: string
  reponse: CertificationAnswer
  justification: string
}

export function getOptions(q: CertificationQuestion): [CertificationAnswer, CertificationAnswer] {
  return q.type === 'Oui/Non' ? ['Oui', 'Non'] : ['Vrai', 'Faux']
}

export function isCorrectAnswer(q: CertificationQuestion, answer: CertificationAnswer): boolean {
  return answer === q.reponse
}
