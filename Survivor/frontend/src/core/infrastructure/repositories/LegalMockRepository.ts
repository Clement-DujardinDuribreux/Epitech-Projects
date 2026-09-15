import { LegalDocumentEntity, type ILegalDocumentProps, type LegalSectionId } from "../../domain/entities/LegalDocument.entity"
import type { ILegalRepository } from "../../interfaces/ILegalRepository"

const LEGAL_DOCUMENTS: ILegalDocumentProps[] = [
  {
    id: "cgu",
    title: "Conditions Générales d'Utilisation (CGU)",
    shortTitle: "CGU Officielles",
    icon: "FileText",
    lastUpdated: "2 Septembre 2026",
    badgeText: "En attente de relecture juriste",
    summary: "Règles d'accès, protection des mineurs (16 ans+), contrôle d'âge, droits à l'image, suppression physique sans délai et principe absolu de non-discrimination.",
    contentMarkdown: `> ⚠️ **AVERTISSEMENT TEMPORAIRE** : Ce document a été pré-validé mais est actuellement en attente de relecture par la juriste de la mission (**Florine Pontaillac**, conseillère juridique au sein du cabinet ministériel). Version provisoire soumise à validation ministérielle.

---

# CONDITIONS GÉNÉRALES D'UTILISATION (CGU)
## Plateforme Ministérielle « JibJob » (Programme ProfilsActifs)

**Statut du document :** Projet soumis à visa juridique  
**Destinataire :** Florine Pontaillac, Conseillère juridique au Cabinet du Ministre  
**Date d'émission :** 2 septembre 2026 — Version 1.2  
**Application :** Plateforme de service public JibJob (Programme ProfilsActifs)  

---

### ARTICLE 1 — OBJET ET CHAMP D'APPLICATION

Les présentes Conditions Générales d'Utilisation régissent l'accès et l'utilisation de la plateforme numérique ministérielle « JibJob », opérée sous l'autorité du Ministère du Job et Bonheur dans le cadre de la politique publique d'insertion professionnelle ProfilsActifs.

JibJob a pour finalité exclusive la mise en relation professionnelle directe et non discriminatoire entre des personnes physiques en recherche d'emploi (ci-après « les Candidats ») et des employeurs légalement déclarés (ci-après « les Recruteurs »), au moyen de courtes présentations audiovisuelles et d'un dispositif d'attestation objective de compétences transversales.

Toute inscription ou navigation sur la plateforme emporte acceptation sans réserve des présentes stipulations.

---

### ARTICLE 2 — CONDITIONS D'ACCÈS ET CRÉATION DE COMPTE

1. **Accès Candidats :** L'accès aux services, la création d'un profil personnel, la passation de l'évaluation du Permis JEB et la publication de contenus sont 100 % gratuits et sans frais.
2. **Accès Recruteurs :** L'accès au catalogue général et au suivi des candidats est réservé aux employeurs justifiant d'une immatriculation valide au registre du commerce ou au répertoire des entreprises (numéro SIRET vérifié). L'usage des données à des fins de prospection commerciale non liée à une offre d'emploi est formellement prohibé.

---

### ARTICLE 3 — CONTRÔLE DE L'ÂGE ET PROTECTION DES MINEURS

Conformément aux dispositions d'ordre public des articles L. 4153-1 et suivants du Code du travail :

1. **Seuil d'âge légal :** L'inscription est strictement interdite à toute personne âgée de moins de 16 ans révolus. Le système technique bloque toute tentative de création de compte n'atteignant pas cet âge.
2. **Mineurs âgés de 16 à 18 ans :** L'inscription est subordonnée au recueil d'une autorisation parentale tracée. Les coordonnées de contact direct du mineur sont masquées par défaut et réservées aux recruteurs habilités.
3. **Suspension et purge pour défaut de date de naissance :** Tout compte existant ne disposant pas d'une date de naissance validée est placé en suspension conservatoire. L'usager dispose d'un délai impératif de sept (7) jours calendaires pour régulariser son profil. À l'expiration de ce délai, le compte et l'ensemble de ses données associées sont physiquement et définitivement purgés sans préavis.

---

### ARTICLE 4 — DÉPÔT ET DIFFUSION DES VIDÉOS PROFESSIONNELLES

1. **Format technique et intégrité :** Les vidéos de présentation sont limitées à une durée maximale de 90 secondes et à un volume de données n'excédant pas 100 mégaoctets. Le serveur procède à un contrôle strict du type MIME réel par inspection des octets magiques (magic bytes). Tout téléversement partiel ou interrompu est automatiquement et immédiatement purgé.
2. **Sous-titrage obligatoire :** Chaque séquence doit intégrer ou faire l'objet d'un sous-titrage synchronisé au format WebVTT afin de garantir l'accessibilité universelle.
3. **Propriété intellectuelle :** Le Candidat conserve l'entière propriété intellectuelle et les droits de la personnalité attachés à son image. Il concède à l'administration une licence d'hébergement et de diffusion gratuite, non exclusive et limitée à la consultation par les Recruteurs authentifiés.

---

### ARTICLE 5 — DROIT À L'IMAGE ET RÉVOCATION IMMÉDIATE

1. **Traçabilité du consentement :** La mise en ligne d'une séquence vidéo requiert le consentement exprès de l'usager, horodaté selon la norme ISO 8601 UTC à la seconde et indexé sur l'empreinte cryptographique (SHA-256) de la version précise des CGU acceptées.
2. **Suppression physique immédiate (Art. 17 RGPD) :** L'usager peut révoquer son consentement et retirer sa vidéo à tout instant depuis son espace personnel.
3. **Proscription absolue du masquage (soft delete) :** L'ordre de révocation déclenche sans délai l'effacement physique direct et irréversible du fichier sur le serveur de stockage et la suppression immédiate de l'enregistrement associé en base de données. Aucun archivage intermédiaire, masquage logique ou copie miroir résiduelle n'est conservé.

---

### ARTICLE 6 — ATTESTATION OBJECTIVE D'APTITUDES « PERMIS JEB »

1. **Nature de l'attestation :** Le « Permis JEB » est un examen technique standardisé certifiant des connaissances professionnelles transversales (cadre légal du travail, culture d'entreprise, communication et gestion de projet).
2. **Accès et validation :** L'obtention du badge « Permis JEB » (atteinte du seuil de 80 % de réussite sur les 20 questions standardisées) constitue une mise en avant des compétences.
3. **Référencement au catalogue général :** Les Candidats ne disposant pas du badge demeurent inscrits et consultables au sein du catalogue général de la plateforme. La démarche est purement facultative.
4. **Sauvegarde de passation :** Le questionnaire dispose d'une sauvegarde automatique en cours d'épreuve pour permettre une reprise immédiate en cas de rupture de session.

---

### ARTICLE 7 — DÉCONNEXION ABSOLUE D'AVEC LE RÉGIME D'ASSURANCE CHÔMAGE

Conformément aux dispositions d'ordre public des articles L. 5421-1 et suivants du Code du travail :

1. L'ouverture, le versement, le montant ou la déchéance des allocations d'assurance chômage ou de solidarité relèvent de la compétence exclusive des organismes légalement désignés (France Travail, Unédic).
2. Aucun indicateur d'activité sur la plateforme (nombre de vues, prise de contact, mentions d'intérêt, réussite ou échec au Permis JEB) ne peut légalement conditionner, moduler ou suspendre les droits sociaux ou allocations d'un usager.
3. Toute clause ou consigne prétendant lier les droits aux allocations à l'utilisation de la plateforme est nulle de plein droit et réputée non écrite.

---

### ARTICLE 8 — VOCABULAIRE INSTITUTIONNEL ET PROSCRIPTION DES MÉTRIQUES DE POPULARITÉ

1. Aucun système de valorisation marchande, compteur public de « likes », mesure de popularité ou incitation à la viralité n'est implanté.
2. Le flux de consultation exclut tout algorithme d'engagement cherchant à favoriser des profils en fonction de métriques d'audience ou de volume d'interactions. Les termes sensationnalistes ou marchands (« tendances », « populaire », « viral ») sont strictement proscrits.
3. Toute valorisation d'une candidature complète répond exclusivement à la dénomination institutionnelle « profils mis en avant ».

---

### ARTICLE 9 — PROTECTION DES DONNÉES PERSONNELLES (RGPD)

1. Les données collectées sont traitées sous la responsabilité du Ministère du Job et Bonheur conformément au Règlement (UE) 2016/679 (RGPD) et à la loi n° 78-17 modifiée.
2. Tout usager dispose d'un droit d'accès, de rectification, d'effacement, de limitation et de portabilité sur ses données.
3. L'exercice de ces droits s'effectue directement depuis l'espace personnel de l'usager ou via le formulaire de contact du portail.

---

### ARTICLE 10 — MODÉRATION ET SIGNALEMENT DES CONTENUS

1. Tout contenu vidéo présentant un caractère discriminatoire, diffamatoire, violent, injurieux ou commercial est immédiatement retiré de la diffusion publique.
2. Tout usager dispose d'un dispositif de signalement direct accessible sur chaque fiche profil.
3. En cas de manquement grave ou répété, le compte de l'usager peut être clôturé sans indemnité ni préavis.

---

### ARTICLE 11 — SÉCURITÉ DE L'INFRASTRUCTURE ET SOUVERAINETÉ

1. L'administration assure la mise en œuvre de protocoles de sécurité conformes aux exigences de l'ANSSI (chiffrement TLS 1.3, hachage des mots de passe Argon2id, chiffrement au repos AES-256, hébergement 100 % souverain sur le territoire national).
2. La plateforme garantit l'absence totale de transfert de données hors de l'Union Européenne et l'absence de traqueurs ou régies publicitaires tierces. Les polices institutionnelles Marianne et Spectral sont auto-hébergées.
3. L'éditeur ne peut être tenu responsable des déclarations inexactes émises par un usager lors de son inscription ou dans ses contenus audiovisuels.

---

### ARTICLE 12 — DROIT APPLICABLE ET JURIDICTION COMPÉTENTE

1. Les présentes CGU sont régies exclusivement par le droit français.
2. Tout litige relatif à leur validité, interprétation ou exécution relève de la compétence exclusive des tribunaux du ressort de Paris.`,
  },
  {
    id: "rgpd",
    title: "Registre des Traitements & Protection des Données (RGPD - Art. 30)",
    shortTitle: "Registre RGPD",
    icon: "ShieldCheck",
    lastUpdated: "2 Septembre 2026",
    badgeText: "Standard CNIL — Art. 30",
    summary: "Cartographie officielle des finalités, bases légales, catégories de données, durées de conservation et mesures de sécurité ANSSI.",
    contentMarkdown: `### 1. Responsable de Traitement & Exercice des Droits
- **Responsable** : Ministère du Job et Bonheur — Direction du Numérique et de l'Insertion.
- **Modalités d'exercice des droits** : Espace personnel de l'usager ou formulaire de contact interne du portail.

---

### 2. Cartographie des Traitements Réglementaires

#### Fiche 1 — Gestion des comptes et contrôle de l'âge
- **Finalité** : Enrôlement, authentification et contrôle d'ordre public de l'âge légal (≥ 16 ans, L. 4153-1).
- **Base légale** : Mission d'intérêt public (Art. 6-1-e) et respect des CGU (Art. 6-1-b).
- **Catégories de données** : État civil, courriel, empreinte du mot de passe haché (Argon2id), date de naissance certifiée, compétences, mobilité, raison sociale et SIRET.
- **Conservation** : Durée du compte actif ; purge à 24 mois d'inactivité continue (préavis J-30) ; purge à J+7 sans date de naissance.

#### Fiche 2 — Gestion et diffusion des vidéos professionnelles
- **Finalité** : Présentation audiovisuelle du projet professionnel auprès des recruteurs habilités.
- **Base légale** : Consentement exprès et révocable (Art. 6-1-a et Art. 9-2-a).
- **Catégories de données** : Fichier binaire vidéo (≤ 90s, ≤ 100 Mo), piste de sous-titres (.vtt), empreinte SHA-256 du consentement, horodatage UTC.
- **Conservation** : Jusqu'à révocation usager ; effacement physique immédiat sur disque et suppression sans délai (zéro soft delete).

#### Fiche 3 — Évaluation et attestation Permis JEB
- **Finalité** : Attestation objective de connaissances transversales (20 questions standardisées, seuil 80 %).
- **Base légale** : Mission d'intérêt public (Art. 6-1-e). Déconnexion totale allocations chômage (L. 5421-1).
- **Catégories de données** : Réponses aux questions objectives, score global, horodatage et statut de certification.
- **Conservation** : Durée de vie du compte actif.

#### Fiche 4 — Mise en relation et messagerie professionnelle
- **Finalité** : Échange direct entre employeurs vérifiés et candidats en recherche active.
- **Base légale** : Mesures précontractuelles (Art. 6-1-b).
- **Catégories de données** : Identifiants des interlocuteurs, corps des messages professionnels, date et heure.
- **Conservation** : 12 mois glissants à compter du dernier contact.

#### Fiche 5 — Traçabilité, sécurité et journaux d'audit
- **Finalité** : Prévention des fraudes, détection des intrusions et réquisitions judiciaires.
- **Base légale** : Obligation légale (Art. 6-1-c RGPD et Art. L. 34-1 CPCE).
- **Catégories de données** : Adresses IP, horodatage UTC, agents utilisateurs, identifiants techniques de session.
- **Conservation** : 12 mois glissants (durée légale impérative).

---

### 3. Mesures de Sécurité ANSSI & CNIL
Chiffrement TLS 1.3 en transit, chiffrement AES-256 au repos, contrôle serveur des *magic bytes*, hébergement 100 % souverain en France, zéro traqueur tiers.`,
  },
  {
    id: "rgaa",
    title: "Déclaration d'Accessibilité Numérique (RGAA 4.1)",
    shortTitle: "Accessibilité RGAA",
    icon: "Eye",
    lastUpdated: "2 Septembre 2026",
    badgeText: "Niveau AA Vérifié",
    summary: "Audit des 3 écrans réels (inscription, profil public, catalogue recruteur) et mesures précises des ratios de contraste texte/fond.",
    contentMarkdown: `### 1. Engagement de Conformité
Le Ministère du Job et Bonheur s'engage à rendre sa plateforme accessible conformément à l'article 47 de la loi n° 2005-102 du 11 février 2005.

### 2. Écrans Réels Audités (RGAA 4.1 - Niveau Double-A)
1. **Parcours Inscription & Authentification** :
   - Étiquetage explicite systématique (\`<label for="...">\`).
   - Vocalisation immédiate des messages d'erreur (\`aria-invalid="true"\`, \`role="alert"\`).
   - Opérabilité complète au clavier seul.
2. **Fiche Profil Public Candidat** :
   - Sous-titrage synchronisé WebVTT obligatoire pour chaque vidéo.
   - Blocage technique de la lecture simultanée multi-flux.
   - Alternatives textuelles systématiques pour chaque visuel.
3. **Catalogue de Recherche Recruteur** :
   - Navigation clavier intégrale avec focus visible (ratio > 3,5:1).
   - Confinement du focus (*focus trap*) dans la modale de contact et fermeture par touche <kbd>Échap</kbd>.

### 3. Mesures Réelles des Ratios de Contraste
| Composant testé | Valeurs colorimétriques | Ratio mesuré | Norme RGAA AA |
|---|---|---|---|
| **Titres & Textes institutionnels** | Texte \`#1B3A6B\` / Fond \`#FFFFFF\` | **11,2 : 1** | ≥ 4,5 : 1 (Conforme AAA) |
| **Bouton Primaire** | Texte \`#FFFFFF\` / Fond \`#000091\` | **14,1 : 1** | ≥ 4,5 : 1 (Conforme AAA) |
| **Bouton Secondaire (CTA)** | Texte \`#09090F\` / Fond \`#D97706\` | **7,4 : 1** | ≥ 4,5 : 1 (Conforme AAA) |
| **Texte courant (Sombre)** | Texte \`#FFFFFF\` / Fond \`#09090F\` | **19,8 : 1** | ≥ 4,5 : 1 (Conforme AAA) |

### 4. Voie de Recours auprès du Défenseur des Droits
En cas de défaut d'accessibilité non résolu par le formulaire de contact interne :
- En ligne : [defenseurdesdroits.fr](https://www.defenseurdesdroits.fr)
- Par courrier gratuit : *Défenseur des droits, Libre réponse 71120, 75342 Paris CEDEX 07*.`,
  },
  {
    id: "filtrage",
    title: "Note de Cadrage : Critères de Filtrage & Non-Discrimination",
    shortTitle: "Critères de Filtrage",
    icon: "SlidersHorizontal",
    lastUpdated: "2 Septembre 2026",
    badgeText: "Éthique & Neutralité",
    summary: "Justifications des 4 critères professionnels exclusifs, non-discrimination (L. 1132-1) et filtres bannis.",
    contentMarkdown: `### 1. Principe de Légalité (Art. L. 1132-1 du Code du travail)
Aucun candidat ne peut être écarté d'un recrutement sur le fondement de son origine, son sexe, son âge ou son apparence physique. L'interface et l'API restreignent techniquement les requêtes aux seuls paramètres professionnels objectifs.

### 2. Les 4 Critères Strictement Autorisés (Justifications Unitaires)
1. **Compétences et métiers déclarés** : Isole les qualifications techniques requises pour le poste sans interférence sur la personne du candidat.
2. **Secteur d'activité professionnel** : Cible la branche professionnelle souhaitée au moyen d'un référentiel de nomenclature standardisé.
3. **Bassin géographique de mobilité** : Vérifie l'adéquation au trajet professionnel à l'échelle exclusive du département ou de la région, sans saisie ni exposition d'adresse précise.
4. **Statut de certification « Permis JEB »** : Atteste de connaissances transversales objectives, sur démarche facultative du candidat et sans impacter la visibilité des profils non certifiés.

### 3. Filtres et Traitements Définitivement Bannis
- **Âge et date de naissance** : Réservés exclusivement au contrôle réglementaire d'accès des mineurs (≥ 16 ans) lors de l'inscription, exclus de tout moteur de requête recruteur.
- **Biométrie et traits physiques** : Aucun algorithme de traitement d'image ou de reconnaissance faciale.
- **Patronyme** : Recherche par nom de famille désactivée pour prévenir tout tri à l'embauche.
- **Métriques de popularité** : Aucun like ni compteur d'audience ; emploi exclusif de « profils mis en avant ».
- **Indicateurs sociaux et allocations chômage** : Déconnexion absolue d'ordre public (Art. L. 5421-1).`,
  },
  {
    id: "souverainete",
    title: "Note d'Hébergement, Flux Sortants & Souveraineté Numérique",
    shortTitle: "Souveraineté & Flux",
    icon: "Server",
    lastUpdated: "2 Septembre 2026",
    badgeText: "100% Souverain France",
    summary: "Hébergement souverain SecNumCloud, zéro traqueur tiers, polices Marianne/Spectral auto-hébergées et Whisper local.",
    contentMarkdown: `### 1. Hébergement 100 % Localisé en France (SecNumCloud)
- L'ensemble des serveurs applicatifs, bases de données relationnelles et volumes de stockage de fichiers médias sont physiquement situés sur le territoire français.
- Immunité absolue contre le Cloud Act américain : aucun hébergement chez un fournisseur soumis à une législation extra-européenne.

### 2. Cartographie des Flux : Zéro Traqueur Tiers
- **Flux sortants vers des régies tierces** : Strictement **0 octet** (aucun Google Analytics, aucun pixel publicitaire, aucun cookie de traçage).
- **Sous-titrage automatique souverain** : Transcription audio réalisée en local sur nos serveurs via un modèle open-source souverain (type **Whisper** auto-hébergé). Aucun flux audio ne quitte l'infrastructure.
- **Typographies d'État auto-hébergées** : Polices Marianne et Spectral embarquées localement au format Woff2 (zéro appel vers Google Fonts ou CDN tiers).
- **Hébergement vidéo exclusif** : Tous les flux audiovisuels sont hébergés et diffusés directement par l'infrastructure souveraine.

### 3. Contrôle d'Intégrité et Sécurité Serveur
- Chiffrement systématique en transit (\`TLS 1.3\`) et au repos (\`AES-256\`).
- Hachage des mots de passe avec l'algorithme robuste \`Argon2id\`.
- Plafond de fichier fixé à **100 Mo** avec contrôle serveur du type MIME réel par inspection des octets magiques (*magic bytes*) et purge automatique des téléversements partiels interrompus.`,
  },
]

export class LegalMockRepository implements ILegalRepository {
  async getAllDocuments(): Promise<LegalDocumentEntity[]> {
    return LEGAL_DOCUMENTS.map((props) => new LegalDocumentEntity(props))
  }

  async getDocumentById(id: LegalSectionId): Promise<LegalDocumentEntity | null> {
    const props = LEGAL_DOCUMENTS.find((d) => d.id === id)
    if (!props) return null
    return new LegalDocumentEntity(props)
  }
}
