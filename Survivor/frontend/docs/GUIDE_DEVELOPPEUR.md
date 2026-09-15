# Documentation Technique et Guide d'Architecture — PROFILSACTIFS

## 1. Introduction et Périmètre du Projet

PROFILSACTIFS est une plateforme applicative web innovante. Elle a pour vocation de faciliter la mise en relation entre demandeurs d'emploi et recruteurs au travers de présentations vidéo courtes, d'une certification d'aptitudes transversales, et d'une conformité stricte aux exigences d'accessibilité numérique (RGAA 4.1) et de souveraineté des données (RGPD).

Ce document constitue la référence technique à destination des équipes d'ingénierie logicielle pour la maintenance, l'extension et l'intégration continue du projet.

---

## 2. Architecture Système

Le projet frontend repose sur une approche **Clean Architecture** stricte couplée aux principes **SOLID**, garantissant l'indépendance totale de la logique métier vis-à-vis du framework d'interface utilisateur (Vue 3) et des bibliothèques externes.

### 2.1 Arborescence Normative du Répertoire `frontend/src/`

```text
frontend/src/
├── core/
│   ├── domain/
│   │   ├── entities/                   # Entités métier pures (CandidateProfile, LegalDocument)
│   │   └── value-objects/              # Objets de valeur immuables (CandidateStats, VideoSource)
│   ├── interfaces/                     # Contrats d'interfaces de données (ICandidateRepository, ILegalRepository)
│   ├── infrastructure/
│   │   └── repositories/               # Implémentations concrètes d'accès aux données (CandidateMockRepository, etc.)
│   ├── services/                       # Services applicatifs orchestrateurs (CandidateFeedService, ThemeService...)
│   └── config/                         # Configuration statique de l'application et presets de thèmes
│
├── components/
│   ├── ui/                             # Primitives d'interface officielles shadcn-vue (Button, Badge, Card, Dialog...)
│   ├── common/                         # Composants génériques transverses (BrandLogo, JebBadge, DevicePreviewBar)
│   ├── feed/                           # Composants du flux vidéo vertical (Card, Actions, Nav, TopBar)
│   ├── profile/                        # Composants de la vue profil détaillée (Hero, Metrics, About, Skills...)
│   └── legal/                          # Composant visualiseur du référentiel juridique et conformité
│
├── composables/                        # Composables réactifs Vue 3 encapsulant les flux d'état
├── views/                              # Contrôleurs de vues / Pages routées (VideoFeedView, ProfileDetailView...)
└── styles/                             # Système de Design Tokens CSS et styles globaux (theme-tokens.css, globals.css)
```

### 2.2 Règles de Dépendance et Flux de Données

Les flux de dépendance respectent la règle d'inversion des dépendances (DIP) :

```text
Vues & Composants (Presentation)
             ↓
        Composables
             ↓
    Services Applicatifs
             ↓
  Interfaces & Entités Métier (Domain)
             ↑
   Repositories (Infrastructure)
```

- La couche **Domain** ne possède aucune dépendance vers Vue, le DOM ou des bibliothèques tierces.
- Les entités sont instanciées avec des structures de données typées et encapsulent leurs propres règles d'invariance métier.
- L'infrastructure implémente les interfaces définies dans le domaine et peut être substituée sans aucun impact sur la couche de présentation.

---

## 3. Gestion des Données Candidats et Flux Vidéo

### 3.1 Contrat de Données Candidat

Le modèle de données d'un profil candidat est formalisé par l'entité `CandidateProfileEntity` et les Value Objects associés (`CandidateStatsVO`, `VideoSourceVO`).

Emplacement du mock actuel :
`frontend/src/core/infrastructure/repositories/CandidateMockRepository.ts`

```typescript
export interface ICandidateProfileProps {
  id: string                          // Identifiant immuable (kebab-case, ex: 'amina-traore')
  fullName: string                    // Nom et prénom du candidat
  headline: string                    // Titre professionnel et localisation synthétique
  location: string                    // Ville et département (ex: 'Paris, 75')
  age?: number                        // Âge (optionnel)
  bio: string                         // Texte de présentation détaillée
  skills: string[]                    // Tableau des compétences déclarées
  isJebCertified: boolean             // Indicateur d'obtention de la certification Permis JEB
  certificationDate?: string          // Date ISO de délivrance de la certification
  media: {
    videoUrl?: string                 // URL directe du flux vidéo (MP4, HLS, WebM)
    posterUrl: string                 // URL de l'image de couverture / affiche vidéo
    avatarUrl: string                 // URL de la photo de profil (format carré ou circulaire)
    durationSeconds?: number          // Durée du média en secondes
    subtitles?: Array<{               // Pistes de sous-titrage synchronisées
      src: string                     // Chemin relatif ou absolu du fichier .vtt
      label: string                   // Libellé de la piste affiché à l'utilisateur
      srcLang: string                 // Code ISO de la langue (ex: 'fr')
      default?: boolean               // Piste activée par défaut
    }>
  }
  stats: {
    recruiterViewsCount: number       // Nombre cumulé de consultations recruteurs
    recommendationsCount: number      // Nombre de recommandations / boosts obtenus
    solicitationsCount: number        // Nombre de prises de contact directes reçues
    sharesCount: number               // Nombre de partages du profil
    isRecommended: boolean            // État de recommandation par l'utilisateur courant
    isSaved: boolean                  // État d'enregistrement dans les favoris
  }
}
```

### 3.2 Spécification des Fichiers de Sous-titres (WebVTT)

La conformité au critère RGAA 4.1 (Accessibilité multimédia) impose l'adjonction d'une piste de transcription textuelle synchronisée au format WebVTT (`.vtt`).

Emplacement standard des fichiers statiques :
`frontend/public/subtitles/`

Structure minimale normalisée :

```vtt
WEBVTT - Transcription officielle PROFILSACTIFS

00:00:00.500 --> 00:00:03.200
Bonjour, je suis Amina, développeuse Full-Stack certifiée JEB.

00:00:03.500 --> 00:00:07.000
Je conçois des applications web modernes, performantes et accessibles.

00:00:07.200 --> 00:00:10.500
Disponible immédiatement en région parisienne ou en télétravail.
```

Le composant `CandidateFeedCard.vue` écoute l'événement `cuechange` de la balise HTML5 `<track>` native pour restituer le sous-titre actif dans un conteneur accessible (`aria-live="polite"`).

---

## 4. Système de Design Tokens et Direction Artistique

L'application repose sur un système de design tokens CSS sémantiques aligné sur la charte officielle Figma **ProfilsActifs Dark Neon**.

### 4.1 Architecture des Tokens CSS

Fichier de définition des tokens :
`frontend/src/styles/theme-tokens.css`

Les tokens sont directement liés aux variables CSS de Tailwind CSS v4 :

| Variable CSS                                    | Valeur standard               | Rôle fonctionnel                                 |
| ----------------------------------------------- | ----------------------------- | ------------------------------------------------- |
| `--background` / `--theme-bg-main`          | `#09090f`                   | Couleur d'arrière-plan principal                 |
| `--card` / `--theme-bg-card`                | `#0e0e16`                   | Surface des cartes et conteneurs de profil        |
| `--foreground` / `--theme-text-primary`     | `#ffffff`                   | Couleur typographique principale                  |
| `--muted-foreground` / `--theme-text-muted` | `rgba(255, 255, 255, 0.6)`  | Couleur typographique secondaire / atténuée     |
| `--primary` / `--theme-accent-primary`      | `#1a6fff`                   | Couleur d'accentuation électrique et boutons     |
| `--accent` / `--theme-accent-cyan`          | `#00d4ff`                   | Couleur cyan pour les boutons d'appel à l'action |
| `--theme-accent-gold`                         | `#f5c842`                   | Couleur dorée officielle du badge Permis JEB     |
| `--border` / `--theme-border-subtle`        | `rgba(255, 255, 255, 0.12)` | Délimitations et bordures structurelles          |

---

## 5. Référentiel Juridique, RGPD et Accessibilité

Le socle juridique et documentaire est géré de manière unifiée via l'entité `LegalDocumentEntity` et exposé sur les routes `/conformite` et `/cgu`.

Fichier source :
`frontend/src/core/infrastructure/repositories/LegalMockRepository.ts`

### 5.1 Documents Administrés

| Identifiant (`LegalSectionId`) | Intitulé officiel                       | Fondement réglementaire                                                  |
| -------------------------------- | ---------------------------------------- | ------------------------------------------------------------------------- |
| `cgu`                          | Conditions Générales d'Utilisation     | Droit des contrats et propriété intellectuelle                          |
| `rgpd`                         | Registre des Traitements                 | Article 30 du Règlement Général sur la Protection des Données         |
| `rgaa`                         | Déclaration d'Accessibilité Numérique | Référentiel Général d'Amélioration de l'Accessibilité (RGAA 4.1 AA) |
| `filtrage`                     | Note sur les Critères de Filtrage       | Non-discrimination et neutralité algorithmique                           |
| `souverainete`                 | Note d'Hébergement et Flux Sortants     | Souveraineté des infrastructures UE et transcription locale Whisper      |

---

## 6. Composants d'Interface et Bibliothèque shadcn-vue

L'ensemble des composants interactifs de bas niveau est implémenté via la bibliothèque standard **shadcn-vue** située dans `frontend/src/components/ui/`.

### 6.1 Inventaire des Primitives UI

| Composant     | Répertoire                  | Dépendance Radix-Vue                | Variantes Disponibles                                                                                           |
| ------------- | ---------------------------- | ------------------------------------ | --------------------------------------------------------------------------------------------------------------- |
| `Button`    | `components/ui/button/`    | `Primitive`                        | `default`, `destructive`, `outline`, `secondary`, `ghost`, `link`, `cyan`, `electric`, `gold` |
| `Badge`     | `components/ui/badge/`     | HTML Standard                        | `default`, `secondary`, `destructive`, `outline`, `tech`, `jeb`, `pill`                           |
| `Card`      | `components/ui/card/`      | HTML Standard                        | `Card`, `CardHeader`, `CardTitle`, `CardDescription`, `CardContent`, `CardFooter`                   |
| `Dialog`    | `components/ui/dialog/`    | `DialogRoot`, `DialogContent`... | Modales de confirmation et formulaire de mise en relation recruteur                                             |
| `Tabs`      | `components/ui/tabs/`      | `TabsRoot`, `TabsList`...        | Navigation par onglets (utilisé pour le Hub Juridique)                                                         |
| `Avatar`    | `components/ui/avatar/`    | `AvatarRoot`, `AvatarImage`...   | Affichage des photos de profil avec repli textuel initiales                                                     |
| `Separator` | `components/ui/separator/` | `Separator`                        | Lignes de séparation sémantiques horizontales et verticales                                                   |

### 6.2 Modification des Variantes de Composants

Les variantes sont déclarées au moyen de la fonction `cva` (Class Variance Authority).
Exemple pour le composant `Button` (`frontend/src/components/ui/button/index.ts`) :

```typescript
export const buttonVariants = cva(
  'inline-flex items-center justify-center gap-2 whitespace-nowrap rounded-md text-sm font-medium transition-colors ...',
  {
    variants: {
      variant: {
        default: 'bg-primary text-primary-foreground shadow hover:bg-primary/90',
        cyan: 'bg-[#00d4ff] text-[#09090f] font-bold shadow-md hover:bg-[#00d4ff]/90',
        electric: 'bg-[#1a6fff] text-white font-bold shadow-md hover:bg-[#1a6fff]/90',
        gold: 'bg-gradient-to-r from-[#f5c842] to-[#c5941a] text-[#09090f] font-bold shadow-md',
      },
      size: {
        default: 'h-9 px-4 py-2',
        sm: 'h-8 rounded-md px-3 text-xs',
        lg: 'h-10 rounded-md px-8',
        xl: 'h-12 rounded-xl px-6 text-base',
      },
    },
    defaultVariants: {
      variant: 'default',
      size: 'default',
    },
  },
)
```

---

## 7. Protocole de Connexion Backend

Le découplage par Clean Architecture permet de substituer le repository fictif par une implémentation HTTP connectée à une API distante sans modifier aucun composant de vue ni composable.

### 7.1 Implémentation du Repository HTTP

Créer la classe concrète dans `frontend/src/core/infrastructure/repositories/CandidateHttpRepository.ts` :

```typescript
import type { ICandidateRepository } from '../../interfaces/ICandidateRepository'
import { CandidateProfileEntity } from '../../domain/entities/CandidateProfile.entity'

export class CandidateHttpRepository implements ICandidateRepository {
  private readonly endpoint: string

  constructor(endpoint: string = import.meta.env.VITE_API_BASE_URL || '/api/v1') {
    this.endpoint = endpoint
  }

  public async getAll(): Promise<CandidateProfileEntity[]> {
    const response = await fetch(`${this.endpoint}/candidates`, {
      headers: { 'Accept': 'application/json' },
    })
    if (!response.ok) {
      throw new Error(`Erreur HTTP lors de la récupération des profils: ${response.statusText}`)
    }
    const data = await response.json()
    return data.map((item: unknown) => new CandidateProfileEntity(item as never))
  }

  public async getById(id: string): Promise<CandidateProfileEntity | null> {
    const response = await fetch(`${this.endpoint}/candidates/${encodeURIComponent(id)}`, {
      headers: { 'Accept': 'application/json' },
    })
    if (response.status === 404) return null
    if (!response.ok) {
      throw new Error(`Erreur HTTP lors de la récupération du candidat ${id}: ${response.statusText}`)
    }
    const data = await response.json()
    return new CandidateProfileEntity(data)
  }

  public async save(candidate: CandidateProfileEntity): Promise<CandidateProfileEntity> {
    const response = await fetch(`${this.endpoint}/candidates/${encodeURIComponent(candidate.id)}`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
      },
      body: JSON.stringify(candidate.toJSON()),
    })
    if (!response.ok) {
      throw new Error(`Erreur HTTP lors de la sauvegarde du candidat ${candidate.id}`)
    }
    const data = await response.json()
    return new CandidateProfileEntity(data)
  }
}
```

### 7.2 Injection dans les Composables

Dans `frontend/src/composables/useCandidateFeed.ts` et `frontend/src/composables/useCandidateProfile.ts`, modifier le constructeur injecté par défaut :

```typescript
// Remplacement du mock par le client HTTP
const repository = new CandidateHttpRepository()
const service = new CandidateFeedService(repository)
```

---

## 8. Commandes d'Exploitation et Intégration Continue

Toutes les commandes doivent être exécutées depuis le sous-répertoire `frontend/` :

### 8.1 Développement Local

```bash
npm run dev
```

Démarre le serveur Vite avec rechargement à chaud sur `http://localhost:5173/`.

### 8.2 Contrôle de Typage et Compilation de Production

```bash
npm run build
```

Exécute la vérification statique stricte `vue-tsc -b` suivie du bundler `vite build`. La commande doit obligatoirement retourner le code de sortie `0` avant tout déploiement en intégration continue (CI).

### 8.3 Prévisualisation du Bundle de Production

```bash
npm run preview
```

Instancie un serveur HTTP local servant les fichiers minifiés du dossier `dist/`.

---

## 9. Référentiel des Routes Applicatives

| Route              | Nom de Route        | Vue Associée               | Description                                                                 |
| ------------------ | ------------------- | --------------------------- | --------------------------------------------------------------------------- |
| `/`              | `Home`            | `HomeView.vue`            | Point d'entrée de l'application (redirige ou affiche le feed vidéo)       |
| `/feed`          | `VideoFeed`       | `VideoFeedView.vue`       | Flux vidéo vertical immersif TikTok-like avec navigation clavier / molette |
| `/profils/:id`   | `ProfileDetail`   | `ProfileDetailView.vue`   | Fiche détaillée publique d'un candidat avec métriques de recrutement     |
| `/conformite`    | `LegalCompliance` | `LegalComplianceView.vue` | Hub documentaire et légal (CGU, RGPD Art. 30, RGAA 4.1, Souveraineté)     |
| `/cgu`           | `CGU`             | `LegalComplianceView.vue` | Alias direct vers les Conditions Générales d'Utilisation                  |
| `/login`         | `Login`           | `LoginView.vue`           | Portail d'authentification des utilisateurs                                 |
| `/register`      | `Register`        | `RegisterView.vue`        | Portail d'inscription candidat / recruteur                                  |
| `/certification` | `Certification`   | `CertificationView.vue`   | Espace officiel de passage du Permis JEB                                    |
