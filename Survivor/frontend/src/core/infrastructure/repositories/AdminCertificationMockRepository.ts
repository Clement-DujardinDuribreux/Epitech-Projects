import {
  AdminCertificationQuestionEntity,
  type IAdminCertificationQuestionProps,
} from '../../domain/entities/AdminCertificationQuestion.entity'
import type { IAdminCertificationRepository } from '../../interfaces/IAdminCertificationRepository'

const STORAGE_KEY = 'profilsactifs_admin_certification_questions_v1'

const DEFAULT_20_QUESTIONS: IAdminCertificationQuestionProps[] = [
  // 5 questions Travail
  {
    id: 1,
    categorie: 'Travail',
    type: 'Vrai/Faux',
    question: 'Le CDI est le contrat de travail standard en France.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 92,
    isActive: true,
  },
  {
    id: 2,
    categorie: 'Travail',
    type: 'Vrai/Faux',
    question: "La période d'essai peut être renouvelée indéfiniment.",
    reponse: 'Faux',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 81,
    isActive: true,
  },
  {
    id: 3,
    categorie: 'Travail',
    type: 'Vrai/Faux',
    question: 'Le salaire minimum en France est appelé le SMIC.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 96,
    isActive: true,
  },
  {
    id: 5,
    categorie: 'Travail',
    type: 'Vrai/Faux',
    question: 'Le télétravail est un droit absolu inscrit dans le code du travail français pour tous les salariés.',
    reponse: 'Faux',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 74,
    isActive: true,
  },
  {
    id: 6,
    categorie: 'Travail',
    type: 'Vrai/Faux',
    question: 'La durée légale du travail en France est de 35 heures par semaine.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 94,
    isActive: true,
  },

  // 5 questions Culture générale business
  {
    id: 26,
    categorie: 'Culture générale',
    type: 'Vrai/Faux',
    question: 'Apple a été fondée par Steve Jobs, Steve Wozniak et Ronald Wayne en 1976.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 79,
    isActive: true,
  },
  {
    id: 27,
    categorie: 'Culture générale',
    type: 'Vrai/Faux',
    question: 'LinkedIn a été racheté par Microsoft en 2016.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 71,
    isActive: true,
  },
  {
    id: 30,
    categorie: 'Culture générale',
    type: 'Vrai/Faux',
    question: 'Le PIB mesure la richesse produite par un pays sur une année.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 88,
    isActive: true,
  },
  {
    id: 32,
    categorie: 'Culture générale',
    type: 'Vrai/Faux',
    question: 'Le terme "licorne" désigne une startup valorisée à plus d\'un milliard de dollars.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 86,
    isActive: true,
  },
  {
    id: 34,
    categorie: 'Culture générale',
    type: 'Vrai/Faux',
    question: 'Le "Kaizen", méthode d\'amélioration continue, est un concept d\'origine japonaise.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Difficile',
    successRate: 64,
    isActive: true,
  },

  // 5 questions Comportement en entreprise
  {
    id: 53,
    categorie: 'Comportement en entreprise',
    type: 'Oui/Non',
    question: 'Est-il important de saluer ses collègues en arrivant au bureau ?',
    reponse: 'Oui',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 98,
    isActive: true,
  },
  {
    id: 55,
    categorie: 'Comportement en entreprise',
    type: 'Oui/Non',
    question: 'Un feedback constructif doit-il se concentrer sur des faits plutôt que sur la personne ?',
    reponse: 'Oui',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 91,
    isActive: true,
  },
  {
    id: 57,
    categorie: 'Comportement en entreprise',
    type: 'Oui/Non',
    question: "L'écoute active est-elle une compétence essentielle en entreprise ?",
    reponse: 'Oui',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 95,
    isActive: true,
  },
  {
    id: 60,
    categorie: 'Comportement en entreprise',
    type: 'Oui/Non',
    question: "Partager des informations confidentielles de l'entreprise avec des concurrents est-il autorisé ?",
    reponse: 'Non',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 99,
    isActive: true,
  },
  {
    id: 68,
    categorie: 'Comportement en entreprise',
    type: 'Oui/Non',
    question: 'Reconnaître ses erreurs publiquement peut-il renforcer la confiance de son équipe ?',
    reponse: 'Oui',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 82,
    isActive: true,
  },

  // 5 questions Gestion de projets et d'équipe
  {
    id: 76,
    categorie: "Gestion de projets et d'équipe",
    type: 'Vrai/Faux',
    question: 'La méthode Agile privilégie des cycles de développement courts et itératifs.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 89,
    isActive: true,
  },
  {
    id: 77,
    categorie: "Gestion de projets et d'équipe",
    type: 'Vrai/Faux',
    question: 'Le diagramme de Gantt est un outil utilisé pour planifier les tâches d\'un projet dans le temps.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Facile',
    successRate: 87,
    isActive: true,
  },
  {
    id: 79,
    categorie: "Gestion de projets et d'équipe",
    type: 'Vrai/Faux',
    question: 'Un "sprint" en méthode Scrum dure généralement plusieurs mois.',
    reponse: 'Faux',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 76,
    isActive: true,
  },
  {
    id: 80,
    categorie: "Gestion de projets et d'équipe",
    type: 'Vrai/Faux',
    question: 'Le triangle d\'or de la gestion de projet représente le coût, le délai et le périmètre/qualité.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 72,
    isActive: true,
  },
  {
    id: 83,
    categorie: "Gestion de projets et d'équipe",
    type: 'Vrai/Faux',
    question: 'Le "Product Owner" dans Scrum est responsable de la vision produit et du backlog.',
    reponse: 'Vrai',
    ponderation: 1,
    difficulty: 'Moyen',
    successRate: 83,
    isActive: true,
  },
]

export class AdminCertificationMockRepository implements IAdminCertificationRepository {
  private loadData(): IAdminCertificationQuestionProps[] {
    try {
      const stored = localStorage.getItem(STORAGE_KEY)
      if (stored) {
        return JSON.parse(stored) as IAdminCertificationQuestionProps[]
      }
    } catch (err) {
      console.warn('[AdminCertificationMockRepository] loadData failed:', err)
    }
    this.saveData(DEFAULT_20_QUESTIONS)
    return [...DEFAULT_20_QUESTIONS]
  }

  private saveData(items: IAdminCertificationQuestionProps[]): void {
    try {
      localStorage.setItem(STORAGE_KEY, JSON.stringify(items))
    } catch (err) {
      console.warn('[AdminCertificationMockRepository] saveData failed:', err)
    }
  }

  public async getAll(): Promise<AdminCertificationQuestionEntity[]> {
    const raw = this.loadData()
    return raw.map((item) => new AdminCertificationQuestionEntity(item))
  }

  public async getById(id: number): Promise<AdminCertificationQuestionEntity | null> {
    const raw = this.loadData()
    const found = raw.find((item) => item.id === id)
    return found ? new AdminCertificationQuestionEntity(found) : null
  }

  public async save(question: AdminCertificationQuestionEntity): Promise<AdminCertificationQuestionEntity> {
    const raw = this.loadData()
    const index = raw.findIndex((item) => item.id === question.id)
    if (index >= 0) {
      raw[index] = question.toJSON()
    } else {
      raw.push(question.toJSON())
    }
    this.saveData(raw)
    return question
  }

  public async delete(id: number): Promise<void> {
    const raw = this.loadData()
    const filtered = raw.filter((item) => item.id !== id)
    this.saveData(filtered)
  }

  public async resetToDefaults(): Promise<AdminCertificationQuestionEntity[]> {
    this.saveData(DEFAULT_20_QUESTIONS)
    return DEFAULT_20_QUESTIONS.map((item) => new AdminCertificationQuestionEntity(item))
  }
}
