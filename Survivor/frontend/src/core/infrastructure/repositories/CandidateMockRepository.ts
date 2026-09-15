import { CandidateProfileEntity, type ICandidateProfileProps } from '../../domain/entities/CandidateProfile.entity'
import type { ICandidateRepository, IPaginationParams, IPaginatedResult } from '../../interfaces/ICandidateRepository'

const MOCK_CANDIDATES: ICandidateProfileProps[] = [
  {
    id: 'amina-traore',
    fullName: 'Amina Traoré',
    headline: 'Développeuse Full-Stack · Paris, 75',
    location: 'Paris, 75',
    age: 25,
    bio: 'Développeuse passionnée par les architectures réactives, TypeScript et Vue 3. 3 ans d\'expérience dans la création d\'applications web performantes et accessibles.',
    skills: ['Tech & Numérique', 'React', 'TypeScript', 'Vue 3', 'Node.js'],
    isJebCertified: true,
    certificationDate: '2026-04-12',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 45,
      subtitles: [
        {
          src: '/subtitles/amina-presentation.vtt',
          label: 'Français (Auto-transcrit)',
          srcLang: 'fr',
          default: true,
        },
      ],
    },
    stats: {
      recruiterViewsCount: 18640,
      solicitationsCount: 154,
      isSaved: false,
    },
  },
  {
    id: 'lea-d',
    fullName: 'Léa D.',
    headline: 'Vendeuse prêt-à-porter · Lyon, 69',
    location: 'Lyon, 69',
    age: 24,
    bio: 'Passionnée par la mode et le conseil client depuis 4 ans. Excellente maîtrise des techniques d\'encaissement et de gestion des stocks.',
    skills: ['Commerce & Vente', 'Relation client', 'Caisse', 'Merchandising', 'Anglais B2'],
    isJebCertified: true,
    certificationDate: '2026-05-18',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerEscapes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 38,
      subtitles: [
        {
          src: '/subtitles/lea-presentation.vtt',
          label: 'Français (Auto-transcrit)',
          srcLang: 'fr',
          default: true,
        },
      ],
    },
    stats: {
      recruiterViewsCount: 18600,
      solicitationsCount: 47,
      isSaved: true,
    },
  },
  {
    id: 'lucas-martin',
    fullName: 'Lucas Martin',
    headline: 'UX/UI Designer & Accessibilité · Lille, 59',
    location: 'Lille, 59',
    age: 26,
    bio: 'Designer centré utilisateur spécialisé en Design Systems et conformité RGAA 4.1. Convaincu que l\'inclusivité numérique est la clé de produits durables et performants.',
    skills: ['Design & Création', 'Figma', 'Design System', 'Accessibilité RGAA', 'Prototypage'],
    isJebCertified: true,
    certificationDate: '2026-02-20',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerFun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 52,
      subtitles: [
        {
          src: '/subtitles/amina-presentation.vtt',
          label: 'Français (Auto-transcrit)',
          srcLang: 'fr',
          default: true,
        },
      ],
    },
    stats: {
      recruiterViewsCount: 12400,
      solicitationsCount: 89,
      isSaved: false,
    },
  },
  {
    id: 'sarah-benali',
    fullName: 'Sarah Benali',
    headline: 'Data Analyst & Python · Nantes, 44',
    location: 'Nantes, 44',
    age: 28,
    bio: 'Analyste de données rigoureuse. Transformation des données brutes en indicateurs décisionnels clairs et prédictifs pour les PME et grands comptes.',
    skills: ['Tech & Numérique', 'Python', 'SQL', 'Power BI', 'Machine Learning'],
    isJebCertified: false,
    certificationDate: '2026-03-14',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerJoyBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1580489944761-15a19d654956?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1580489944761-15a19d654956?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 40,
      subtitles: [
        {
          src: '/subtitles/lea-presentation.vtt',
          label: 'Français (Auto-transcrit)',
          srcLang: 'fr',
          default: true,
        },
      ],
    },
    stats: {
      recruiterViewsCount: 9800,
      solicitationsCount: 62,
      isSaved: true,
    },
  },
  {
    id: 'maxime-lefebvre',
    fullName: 'Maxime Lefebvre',
    headline: 'Chef de Projet Digital · Bordeaux, 33',
    location: 'Bordeaux, 33',
    age: 31,
    bio: 'Pilotage de projets agiles, coordination d\'équipes pluridisciplinaires et cadrage budgétaire. Esprit d\'équipe et orientation résultats.',
    skills: ['Gestion & Projet', 'Scrum / Agile', 'Management', 'Cadrage Produit', 'Anglais C1'],
    isJebCertified: true,
    certificationDate: '2026-01-10',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/WeAreGoingOnBullrun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 48,
      subtitles: [
        {
          src: '/subtitles/amina-presentation.vtt',
          label: 'Français (Auto-transcrit)',
          srcLang: 'fr',
          default: true,
        },
      ],
    },
    stats: {
      recruiterViewsCount: 15300,
      solicitationsCount: 112,
      isSaved: false,
    },
  },
  {
    id: 'chloe-dupont',
    fullName: 'Chloé Dupont',
    headline: 'Infirmière Diplômée d\'État · Marseille, 13',
    location: 'Marseille, 13',
    age: 27,
    bio: 'Infirmière en milieu hospitalier et soins à domicile. Rigueur, empathie et grande capacité d\'adaptation en situation d\'urgence.',
    skills: ['Santé & Soins', 'Urgences', 'Soins relationnels', 'Coordination médicale'],
    isJebCertified: true,
    certificationDate: '2026-03-22',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
      posterUrl: '/images/candidates/chloe-dupont-poster.jpg',
      avatarUrl: '/images/candidates/chloe-dupont-avatar.jpg',
      durationSeconds: 42,
    },
    stats: {
      recruiterViewsCount: 11200,
      solicitationsCount: 78,
      isSaved: false,
    },
  },
  {
    id: 'antoine-bernard',
    fullName: 'Antoine Bernard',
    headline: 'Menuisier Ébéniste · Strasbourg, 67',
    location: 'Strasbourg, 67',
    age: 29,
    bio: 'Artisan passionné par le travail du bois massif et l\'aménagement d\'intérieurs sur-mesure. Respect des traditions et conception 3D.',
    skills: ['Artisanat & BTP', 'Menuiserie', 'Ébénisterie', 'Lecture de plan', 'Autonomie'],
    isJebCertified: true,
    certificationDate: '2026-02-15',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerEscapes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1560250097-0b93528c311a?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1560250097-0b93528c311a?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 39,
    },
    stats: {
      recruiterViewsCount: 8400,
      solicitationsCount: 43,
      isSaved: true,
    },
  },
  {
    id: 'fatima-zahra',
    fullName: 'Fatima-Zahra K.',
    headline: 'Comptable Général · Toulouse, 31',
    location: 'Toulouse, 31',
    age: 32,
    bio: 'Gestion complète des clôtures mensuelles, déclarations fiscales et suivi de trésorerie. Rigoureuse et force de proposition.',
    skills: ['Finance & Gestion', 'Comptabilité', 'Fiscalité', 'Sage', 'Excel Avancé'],
    isJebCertified: true,
    certificationDate: '2026-04-05',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerFun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1573497019940-1c28c88b4f3e?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1573497019940-1c28c88b4f3e?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 46,
    },
    stats: {
      recruiterViewsCount: 14100,
      solicitationsCount: 95,
      isSaved: false,
    },
  },
  {
    id: 'kevin-moreau',
    fullName: 'Kévin Moreau',
    headline: 'Technicien Réseau & Cybersécurité · Rennes, 35',
    location: 'Rennes, 35',
    age: 26,
    bio: 'Administration d\'infrastructures réseau, supervision SOC et déploiement de solutions de sécurité souveraines.',
    skills: ['Tech & Numérique', 'Linux', 'Cisco', 'Cybersécurité', 'Firewall'],
    isJebCertified: true,
    certificationDate: '2026-01-28',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerJoyBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1519085360753-af0119f7cbe7?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1519085360753-af0119f7cbe7?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 50,
    },
    stats: {
      recruiterViewsCount: 16700,
      solicitationsCount: 130,
      isSaved: false,
    },
  },
  {
    id: 'elodie-roux',
    fullName: 'Élodie Roux',
    headline: 'Responsable Ressources Humaines · Nice, 06',
    location: 'Nice, 06',
    age: 34,
    bio: 'Recrutement inclusif, marque employeur et développement des compétences. Accompagnement bienveillant des équipes.',
    skills: ['Ressources Humaines', 'Recrutement', 'Droit social', 'Formation', 'GPEC'],
    isJebCertified: true,
    certificationDate: '2026-05-02',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/WeAreGoingOnBullrun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1573496799652-408c2ac9fe98?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1573496799652-408c2ac9fe98?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 44,
    },
    stats: {
      recruiterViewsCount: 13800,
      solicitationsCount: 88,
      isSaved: true,
    },
  },
  {
    id: 'julien-lambert',
    fullName: 'Julien Lambert',
    headline: 'Électricien Tertiaire & Industriel · Grenoble, 38',
    location: 'Grenoble, 38',
    age: 28,
    bio: 'Installation électrique, raccordement d\'armoires et maintenance préventive. Respect strict des normes de sécurité NF C 15-100.',
    skills: ['Artisanat & BTP', 'Électricité', 'Câblage', 'Normes NF', 'Diagnostic'],
    isJebCertified: true,
    certificationDate: '2026-03-08',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1539571696357-5a69c17a67c6?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1539571696357-5a69c17a67c6?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 37,
    },
    stats: {
      recruiterViewsCount: 9200,
      solicitationsCount: 55,
      isSaved: false,
    },
  },
  {
    id: 'manon-guerin',
    fullName: 'Manon Guérin',
    headline: 'Conseillère Relation Client · Montpellier, 34',
    location: 'Montpellier, 34',
    age: 23,
    bio: 'Accueil téléphonique, gestion des réclamations et fidélisation client multicanal. Excellente élocution et grand sens du service.',
    skills: ['Commerce & Vente', 'Relation client', 'CRM Zendesk', 'Écoute active'],
    isJebCertified: true,
    certificationDate: '2026-04-19',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerEscapes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1494790108377-be9c29b29330?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1494790108377-be9c29b29330?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 41,
    },
    stats: {
      recruiterViewsCount: 10500,
      solicitationsCount: 68,
      isSaved: true,
    },
  },
  {
    id: 'thomas-picard',
    fullName: 'Thomas Picard',
    headline: 'Cuisinier & Chef de Partie · Dijon, 21',
    location: 'Dijon, 21',
    age: 30,
    bio: 'Passionné par le terroir et la gastronomie moderne. Gestion du chaud et du froid, respect des normes HACCP et encadrement de commis.',
    skills: ['Hôtellerie & Restauration', 'Cuisine', 'HACCP', 'Gestion des stocks', 'Créativité'],
    isJebCertified: true,
    certificationDate: '2026-02-11',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerFun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1522075469751-3a6694fb2f61?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1522075469751-3a6694fb2f61?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 49,
    },
    stats: {
      recruiterViewsCount: 14700,
      solicitationsCount: 104,
      isSaved: false,
    },
  },
  {
    id: 'camille-marchand',
    fullName: 'Camille Marchand',
    headline: 'Juriste Droit des Affaires & RGPD · Lyon, 69',
    location: 'Lyon, 69',
    age: 29,
    bio: 'Conseil juridique opérationnel, rédaction contractuelle et mise en conformité RGPD. Spécialisée dans les entreprises technologiques.',
    skills: ['Droit & Juridique', 'Contrats', 'RGPD', 'Négociation', 'Veille juridique'],
    isJebCertified: true,
    certificationDate: '2026-03-30',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerJoyBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 43,
    },
    stats: {
      recruiterViewsCount: 12900,
      solicitationsCount: 74,
      isSaved: false,
    },
  },
  {
    id: 'youssef-alami',
    fullName: 'Youssef Alami',
    headline: 'Mécanicien Automobile Polyvalent · Rouen, 76',
    location: 'Rouen, 76',
    age: 27,
    bio: 'Diagnostic électronique multimarque, entretien moteur et systèmes de freinage. Travail méticuleux et souci constant de la satisfaction client.',
    skills: ['Artisanat & BTP', 'Mécanique', 'Diagnostic Valise', 'Pneumatique'],
    isJebCertified: true,
    certificationDate: '2026-01-19',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/WeAreGoingOnBullrun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1506794778202-cad84cf45f1d?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1506794778202-cad84cf45f1d?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 36,
    },
    stats: {
      recruiterViewsCount: 9700,
      solicitationsCount: 58,
      isSaved: true,
    },
  },
  {
    id: 'claire-garnier',
    fullName: 'Claire Garnier',
    headline: 'Opticienne Lunetière · Nantes, 44',
    location: 'Nantes, 44',
    age: 26,
    bio: 'Examens de vue, conseil morphologique et montage d\'équipements optiques de haute précision.',
    skills: ['Santé & Soins', 'Optométrie', 'Conseil visagisme', 'Montage atelier', 'Vente'],
    isJebCertified: true,
    certificationDate: '2026-04-28',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1580894732444-8ecded7900cd?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1580894732444-8ecded7900cd?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 40,
    },
    stats: {
      recruiterViewsCount: 11800,
      solicitationsCount: 71,
      isSaved: false,
    },
  },
  {
    id: 'baptiste-vidal',
    fullName: 'Baptiste Vidal',
    headline: 'Plombier Chauffagiste · Clermont-Ferrand, 63',
    location: 'Clermont-Ferrand, 63',
    age: 31,
    bio: 'Installation de pompes à chaleur, sanitaires et réseaux de chauffage. Dépannage rapide et conseils en efficacité énergétique.',
    skills: ['Artisanat & BTP', 'Plomberie', 'Chauffage', 'Soudure cuivre', 'Dépannage'],
    isJebCertified: true,
    certificationDate: '2026-02-04',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerEscapes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1472099645785-5658abf4ff4e?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1472099645785-5658abf4ff4e?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 45,
    },
    stats: {
      recruiterViewsCount: 8900,
      solicitationsCount: 49,
      isSaved: false,
    },
  },
  {
    id: 'ines-hadj',
    fullName: 'Inès Hadj',
    headline: 'Chargée de Communication Digitale · Paris, 75',
    location: 'Paris, 75',
    age: 25,
    bio: 'Stratégie de contenu, relations presse et animation des réseaux professionnels. Créativité éditoriale et respect de la charte de l\'État.',
    skills: ['Design & Création', 'Communication', 'Rédaction web', 'Relations presse', 'Canva'],
    isJebCertified: true,
    certificationDate: '2026-05-12',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerFun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1534528741775-53994a69daeb?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 38,
    },
    stats: {
      recruiterViewsCount: 16200,
      solicitationsCount: 118,
      isSaved: true,
    },
  },
  {
    id: 'damien-renaud',
    fullName: 'Damien Renaud',
    headline: 'Conducteur Poids Lourd & Logistique · Nancy, 54',
    location: 'Nancy, 54',
    age: 33,
    bio: 'Permis CE, FIMO et carte conducteur à jour. Ponctualité, respect rigoureux des temps de conduite et sens de la sécurité routière.',
    skills: ['Logistique & Transport', 'Permis CE', 'FIMO', 'Arrimage'],
    isJebCertified: true,
    certificationDate: '2026-03-17',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerJoyBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 47,
    },
    stats: {
      recruiterViewsCount: 10800,
      solicitationsCount: 65,
      isSaved: false,
    },
  },
  {
    id: 'celia-fontaine',
    fullName: 'Célia Fontaine',
    headline: 'Animatrice Petite Enfance · Orléans, 45',
    location: 'Orléans, 45',
    age: 24,
    bio: 'CAP Petite Enfance, accueil des tout-petits en crèche, mise en place d\'activités d\'éveil et bienveillance au quotidien.',
    skills: ['Social & Éducation', 'Petite enfance', 'Éveil musical', 'Premiers secours'],
    isJebCertified: true,
    certificationDate: '2026-04-22',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/WeAreGoingOnBullrun.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1573496359142-b8d87734a5a2?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 35,
    },
    stats: {
      recruiterViewsCount: 12100,
      solicitationsCount: 82,
      isSaved: false,
    },
  },
  {
    id: 'nicolas-page',
    fullName: 'Nicolas Page',
    headline: 'Développeur DevOps & Kubernetes · Lille, 59',
    location: 'Lille, 59',
    age: 29,
    bio: 'Automatisation CI/CD, conteneurisation Docker, gestion de clusters Kubernetes et sécurité des pipelines dans un environnement souverain.',
    skills: ['Tech & Numérique', 'Kubernetes', 'Docker', 'GitLab CI', 'Terraform'],
    isJebCertified: true,
    certificationDate: '2026-01-14',
    media: {
      videoUrl: 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4',
      posterUrl: 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e?auto=format&fit=crop&w=800&q=80',
      avatarUrl: 'https://images.unsplash.com/photo-1500648767791-00dcc994a43e?auto=format&fit=crop&w=200&q=80',
      durationSeconds: 43,
    },
    stats: {
      recruiterViewsCount: 17400,
      solicitationsCount: 145,
      isSaved: false,
    },
  },
]

export class CandidateMockRepository implements ICandidateRepository {
  private candidates: CandidateProfileEntity[]

  constructor() {
    this.candidates = MOCK_CANDIDATES.map((item) => new CandidateProfileEntity(item))
  }

  public async getAll(params?: IPaginationParams): Promise<IPaginatedResult<CandidateProfileEntity>> {
    const page = Math.max(1, params?.page ?? 1)
    const limit = Math.max(1, params?.limit ?? 20)
    let filtered = [...this.candidates]

    if (params?.query && params.query.trim() !== '') {
      const q = params.query.toLowerCase().trim()
      filtered = filtered.filter(
        (c) =>
          c.fullName.toLowerCase().includes(q) ||
          c.headline.toLowerCase().includes(q) ||
          c.location.toLowerCase().includes(q) ||
          c.skills.some((s) => s.toLowerCase().includes(q)),
      )
    }

    if (params?.skill && params.skill.trim() !== '') {
      const skillFilter = params.skill.toLowerCase().trim()
      filtered = filtered.filter((c) => c.skills.some((s) => s.toLowerCase().includes(skillFilter)))
    }

    if (params?.ids !== undefined) {
      filtered = filtered.filter((c) => params.ids!.includes(c.id))
    }

    if (params?.jebOnly) {
      filtered = filtered.filter((c) => c.isJebCertified)
    }

    const total = filtered.length
    const totalPages = Math.ceil(total / limit) || 1
    const startIndex = (page - 1) * limit
    const paginatedItems = filtered.slice(startIndex, startIndex + limit)

    return {
      items: paginatedItems,
      total,
      page,
      limit,
      totalPages,
      hasMore: page < totalPages,
    }
  }

  public async getById(id: string): Promise<CandidateProfileEntity | null> {
    const found = this.candidates.find((c) => c.id === id)
    return found ? found : null
  }

  public async save(candidate: CandidateProfileEntity): Promise<CandidateProfileEntity> {
    const index = this.candidates.findIndex((c) => c.id === candidate.id)
    if (index >= 0) {
      this.candidates[index] = candidate
    } else {
      this.candidates.push(candidate)
    }
    return candidate
  }
}

