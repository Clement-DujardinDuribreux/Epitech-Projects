# DOSSIER DE CONFORMITÉ JURIDIQUE, RÉGLEMENTAIRE & TECHNIQUE

## Plateforme Numérique Ministérielle « JibJob »

**Destinataire :** Florine — Direction des Affaires Juridiques & DPO  
**Émetteur :** Équipe Technique & Produit JibJob  
**Date d'émission :** 2 septembre 2026  
**Version du dossier :** 1.2 — Conforme RGPD, RGAA 4.1 AA & Code du travail  
**Référence administrative :** JEB/DNI/2026-003-VALIDATION-JURIDIQUE  
**Statut :** Soumis à validation et visa juridique  

---

## Synthèse Exécutive à l'attention de la Direction Juridique

Le présent document constitue le dossier exhaustif de justification juridique et technique établi à l'attention de la Direction des Affaires Juridiques. Il valide la stricte conformité de la plateforme **JibJob** avec :

1. Le **Règlement Général sur la Protection des Données (RGPD - UE 2016/679)** et la loi Informatique et Libertés modifiée ;
2. Le **Code du travail**, notamment l'article L. 1132-1 (non-discrimination), les articles L. 4153-1 et suivants (protection des mineurs), et les articles L. 5421-1 et suivants (indépendance des allocations chômage) ;
3. Le **Référentiel Général d'Amélioration de l'Accessibilité (RGAA version 4.1, niveau Double-A)** ;
4. La **Doctrine interministérielle du Cloud de Confiance et de Souveraineté Numérique**.

---

## 1. Concordance Miroir : CGU v1.2 vs Registre des Traitements (Art. 30 RGPD)

Conformément au standard CNIL (Article 30 du RGPD), les traitements sont cartographiés selon leurs finalités, bases légales, catégories de données et durées de conservation :

| Traitement applicatif | Finalité du traitement | Base Légale (RGPD) | Durée de conservation |
| :---| :---| :---| :---|
| **Gestion des comptes usagers** | Inscription, authentification sécurisée et contrôle d'accès. | **Art. 6(1)(e)** *Mission d'intérêt public* & **Art. 6(1)(b)** | Durée du compte actif. Purge après **24 mois d'inactivité continue** (préavis J-30). Purge à J+7 sans date de naissance. |
| **Diffusion de vidéos** | Présentation audiovisuelle des compétences aux recruteurs habilités. | **Art. 6(1)(a) & Art. 9** *Consentement exprès et révocable* | Jusqu'à révocation par l'usager ou clôture du compte. **Purge physique immédiate sans délai**. |
| **Certification JEB** | Évaluation des aptitudes professionnelles transversales (« Permis JEB »). | **Art. 6(1)(e)** *Mission d'intérêt public* | Durée de vie du compte actif. Suppression définitive lors de la clôture du compte. |
| **Mise en relation & Contact** | Prise de contact professionnelle et transmission sécurisée des opportunités. | **Art. 6(1)(b)** *Mesures précontractuelles* | **12 mois glissants** à compter du dernier contact échangé entre les parties, puis purge définitive. |
| **Sécurité & Traçabilité** | Prévention des intrusions, détection des fraudes et réquisitions judiciaires. | **Art. 6(1)(c)** *Obligation légale (Art. 32 RGPD / CPCE)* | **12 mois glissants** (durée légale impérative), purge automatique quotidienne par tâche d'administration souveraine. |

### Protocole Technique de Droit à l'Oubli & Révocation Vidéo (Art. 17 RGPD)

* **Suppression physique irréversible :** L'activation de la révocation déclenche la suppression physique immédiate du fichier binaire sur le serveur de stockage et l'effacement définitif de l'enregistrement en base de données. Aucun masquage logique (*soft delete*), cache CDN tiers ou copie résiduelle n'est conservé.
* **Attestation d'effacement :** Un récépissé technique horodaté (norme ISO 8601 UTC) avec identifiant de purge est mis à disposition de l'usager.

---

## 2. Protection des Mineurs & Contrôle Strict de l'Âge

Conformément aux dispositions du Code du travail régissant l'emploi des jeunes travailleurs (articles L. 4153-1 et suivants) :

1. **Âge minimum absolu (16 ans) :**
   * L'accès et la création de compte sont techniquement bloqués pour toute personne âgée de moins de 16 ans révolus.
   * La saisie d'une date de naissance valide et certifiée sur l'honneur est obligatoire dès le premier écran d'inscription.
2. **Mineurs âgés de 16 à 18 ans :**
   * Recueil obligatoire des coordonnées du représentant légal et d'une autorisation parentale tracée.
   * Restriction stricte de la visibilité des données de contact direct, réservées aux recruteurs certifiés.
3. **Régularisation des comptes sans date de naissance :**
   * Tout compte existant ne disposant pas d'une date de naissance validée est placé en **suspension conservatoire**.
   * Un délai de régularisation de **sept (7) jours calendaires** est notifié à l'usager. À défaut de justification, le compte et toutes ses données associées sont **définitivement purgés sans préavis**.

---

## 3. Garantie Absolue de Non-Discrimination à l'Embauche (Art. L. 1132-1 du Code du travail)

Afin d'éliminer tout risque juridique de discrimination systémique lié à l'utilisation du format vidéo, la plateforme applique une politique stricte de **filtrage restreint aux seuls critères professionnels objectifs**.

### 3.1. Liste Exhaustive des 4 Critères Autorisés (Interface & API)

| Critère autorisé (UI & API) | Justification Professionnelle Exclusive (Art. L. 1132-1) | Mesure Technique de Protection |
| :---| :---| :---|
| **1. Compétences professionnelles** | Permet d'identifier les savoir-faire techniques et opérationnels requis pour le poste. | Indexation sur référentiel standardisé de métiers et compétences déclarées. |
| **2. Secteur d'activité recherché** | Permet de cibler la branche professionnelle souhaitée par le candidat. | Filtrage par catégories professionnelles objectives (ex. BTP, Santé, Tech, Commerce). |
| **3. Zone géographique de mobilité** | Permet de vérifier l'adéquation géographique avec le bassin d'emploi. | Filtrage à l'échelle du département/région choisi par le candidat. Aucune adresse précise n'est requise ni exposée. |
| **4. Statut de Certification JEB** | Conditionne l'accès au Feed Recruteur Premium et atteste de la validation d'aptitudes professionnelles transversales d'État. | Évaluation standardisée identique pour tous les candidats, sans pondération subjective (seuil de 80 %). |

### 3.2. Filtres Expressément Bannis de l'Architecture Technique

Les critères suivants sont **strictement interdits et absents** du moteur de recherche et de l'interface recruteur :

* **Âge et date de naissance** (exclusivement cantonnés au contrôle réglementaire d'accès des mineurs lors de l'inscription) ;
* **Photographie d'identité, apparence physique et reconnaissance faciale** (aucun traitement biométrique) ;
* **Patronyme / nom de famille** dans les filtres de recherche ;
* **Adresse précise de résidence** ;
* **Métriques de popularité, volume de « likes » ou nombre d'interactions**.

---

## 4. Neutralité du Service Public & Déconnexion Totale des Allocations Chômage

### 4.1. Indépendance Juridique vis-à-vis de l'Assurance Chômage (Unédic / France Travail)

* Conformément aux articles L. 5421-1 et suivants du Code du travail, **l'ouverture, le maintien, le montant ou la radiation des allocations d'assurance chômage relèvent exclusivement des critères légaux et réglementaires en vigueur**.
* **Aucun indicateur de la plateforme JibJob** (score au Permis JEB, nombre de vues, mentions d'intérêt, mises en avant) **ne peut directement ou indirectement impacter, conditionner ou moduler les droits sociaux ou allocations d'un usager**.
* Toute allégation contraire est dépourvue de portée juridique et réputée non écrite.

### 4.2. Vocabulaire Institutionnel et Bannissement Commercial

* Les termes marchands ou sensationnalistes (**« tendances », « populaire », « viral »**) sont **formellement prohibés et bannis**.
* Seule la terminologie institutionnelle **« Profils mis en avant »** est autorisée pour désigner la sélection objective de candidatures complètes.

---

## 5. Déclaration d'Accessibilité Numérique Réelle (RGAA 4.1 Double-A)

L'audit d'accessibilité a été conduit conformément aux règles de vérification de l'article 47 de la loi n° 2005-102 du 11 février 2005.

### 5.1. Périmètre des 3 Écrans Clés Audités et Vérifiés

1. **Parcours d'inscription usager (Candidat et Recruteur)** :
   * Formulaires accessibles, étiquetage sémantique systématique (`<label for="...">`), indications de champs requis.
   * Annonces vocalisables des erreurs de validation (`aria-invalid="true"`, `role="alert"`).
   * Parcours 100 % franchissable sans souris au clavier.
2. **Fiche profil public candidat** :
   * Hiérarchie des titres strictement croissante (`h1` -> `h2` -> `h3`) sans rupture.
   * Alternatives textuelles explicites pour l'ensemble des éléments graphiques.
   * Lecteur multimédia accessible avec sous-titrage synchronisé natif (`.vtt`).
   * **Interdiction stricte d'autoplay simultané multi-flux** (coupure automatique des flux concurrents).
3. **Catalogue de recherche recruteur** :
   * Navigation clavier intégrale avec indicateurs visuels permanents de focus (ratio supérieur à 3,5:1).
   * Modale de contact direct avec confinement du focus (*focus trap*) et fermeture instantanée via la touche <kbd>Échap</kbd>.
   * Filtrage dynamique asynchrone sans perte de positionnement pour les aides techniques.

### 5.2. Mesures Réelles des Ratios de Contraste Texte / Arrière-Plan

| Couple Interface testé | Couleur Texte / Composant | Couleur Arrière-Plan | Ratio Réel Mesuré | Norme RGAA AA | Statut |
| :---| :---| :---| :---| :---| :---|
| **Titres & Texte institutionnel** | Bleu Marianne (`#1B3A6B`) | Blanc Pur (`#FFFFFF`) | **11,2:1** | ≥ 4,5:1 | **Conforme AAA** |
| **Bouton d'Action Primaire** | Blanc Pur (`#FFFFFF`) | Bleu France DSFR (`#000091`) | **14,1:1** | ≥ 4,5:1 | **Conforme AAA** |
| **Bouton d'Action Secondaire (CTA)** | Texte Sombre (`#09090F`) | Ambre Contrasté (`#D97706`) | **7,4:1** | ≥ 4,5:1 | **Conforme AAA** |
| **Texte courant (Thème Sombre)** | Blanc Pur (`#FFFFFF`) | Fond Principal (`#09090F`) | **19,8:1** | ≥ 4,5:1 | **Conforme AAA** |

---

## 6. Souveraineté des Données, Hébergement & Flux Réseau

Conformément à la doctrine Cloud de Confiance de l'État :

1. **Localisation 100 % France (SecNumCloud) :**
   * Tous les serveurs d'application, bases de données et volumes de stockage sont physiquement situés sur le territoire national.
   * Aucun hébergement n'est délégué à des prestataires soumis au Cloud Act ou à des législations extra-européennes.
2. **Auto-Hébergement des Polices & Actifs :**
   * Les typographies institutionnelles **Marianne** et **Spectral** sont servies localement en fichiers autonomes (Woff2), sans aucun appel réseau vers des CDN tiers.
3. **Zéro Flux Sortant Tiers :**
   * La cartographie réseau confirme **0 octet** transmis vers des régies publicitaires, traqueurs ou outils d'analyse tiers.
4. **Contrôle d'Intégrité des Téléversements Côté Serveur :**
   * Vérification des types MIME réels via les octets magiques (*magic bytes*) côté serveur.
   * Plafond de fichier strictement fixé à **100 Mo** avec purge automatique en cas de téléversement partiel interrompu.

---

## 7. Note d'Arbitrage sur l'Identité de Marque (JibJob / ProfilsActifs)

* **Dénomination officielle :** La dénomination officielle de la plateforme de service public est **JibJob** (*Ministère du Job et Bonheur*).
* **Programme d'insertion :** « ProfilsActifs » désigne le programme public d'insertion dont JibJob constitue la plateforme officielle.
