# MINISTÈRE DU JOB ET BONHEUR
### Direction des Affaires Juridiques
### Plateforme Numérique « JibJob »

---

# REGISTRE DES ACTIVITÉS DE TRAITEMENT (ART. 30 RGPD)

**Statut du document :** Document officiel de conformité (Standard CNIL - Art. 30)  
**Destinataire :** Florine Pontaillac, Conseillère juridique au Cabinet du Ministre  
**Responsable de traitement :** Ministère du Job et Bonheur — Direction du Numérique et de l'Insertion  
**Modalités d'exercice des droits :** Espace personnel de l'usager ou formulaire de contact du portail  
**Date d'arrêté du registre :** 2 septembre 2026 — Version 1.2  
**Localisation de l'infrastructure :** Centres de données souverains situés en France (aucun transfert hors Union Européenne)  

---

### FICHE 1 — GESTION DES ACCÈS ET DES COMPTES USAGERS

* **Finalité principale :** Enrôlement, authentification sécurisée, contrôle d'accès et administration des profils candidats et recruteurs.
* **Sous-finalité d'ordre public :** Contrôle impératif de l'âge légal (≥ 16 ans) selon les articles L. 4153-1 et suivants du Code du travail.
* **Base légale (RGPD) :** Mission d'intérêt public (Article 6-1-e) et exécution des conditions contractuelles d'utilisation (Article 6-1-b).
* **Catégories de personnes concernées :** Candidats demandeurs d'emploi et représentants habilités des personnes morales recruteuses.
* **Catégories de données traitées :**
  * Candidats : données d'état civil (nom, prénom), adresse électronique, empreinte cryptographique du mot de passe (hachage Argon2id), date de naissance certifiée, compétences professionnelles déclarées, zone de mobilité (département/région).
  * Recruteurs : identité du contact professionnel, adresse électronique professionnelle, raison sociale, numéro SIRET vérifié.
* **Durée de conservation et purge :**
  * Données conservées pendant la durée d'activité du compte.
  * Purge automatique après 24 mois d'inactivité continue (préavis notifié à J-30).
  * Comptes sans date de naissance : suspension conservatoire immédiate et purge physique définitive à J+7 sans régularisation.
* **Catégories de destinataires habilités :** Administrateurs techniques de la plateforme (habilitations nominatives tracées) et recruteurs vérifiés pour les seules informations publiques du profil.
* **Environnement de stockage et sécurité :** Base de données relationnelle souveraine, accès cloisonné selon le principe du moindre privilège, données chiffrées au repos (AES-256).

---

### FICHE 2 — GESTION ET DIFFUSION DES PRÉSENTATIONS VIDÉO

* **Finalité principale :** Restitution audiovisuelle du projet professionnel et valorisation des compétences auprès des recruteurs habilités.
* **Base légale (RGPD) :** Consentement exprès, univoque et révocable (Article 6-1-a et Article 9-2-a).
* **Catégories de personnes concernées :** Candidats volontaires publiant une séquence audiovisuelle.
* **Catégories de données traitées :**
  * Données audiovisuelles : fichier binaire vidéo (format standard MP4/WebM, durée ≤ 120s, volume ≤ 100 Mo).
  * Contrôle d'intégrité : vérification côté serveur du type MIME réel par les octets magiques (magic bytes) et purge automatique des téléversements partiels interrompus.
  * Données textuelles associées : piste de sous-titres synchronisés (`.vtt`) générée localement.
  * Données de traçabilité légale : empreinte cryptographique SHA-256 du consentement, version des CGU acceptées et horodatage UTC à la seconde.
* **Régime de conservation et révocation (Art. 17 RGPD) :**
  * Données conservées jusqu'à la demande expresse de suppression ou la clôture du compte.
  * Révocation du consentement : effacement physique immédiat sur disque et suppression irréversible de l'enregistrement en base sans délai. Interdiction stricte du masquage logique (soft delete).
* **Catégories de destinataires habilités :** Recruteurs authentifiés disposant d'un compte validé par SIRET.
* **Environnement de stockage et sécurité :** Système de fichiers et volume de stockage d'objets souverains situés en France, isolés de tout CDN commercial tiers, flux de diffusion protégés par TLS 1.3.

---

### FICHE 3 — ÉVALUATION ET ATTESTATION DES COMPÉTENCES « PERMIS JEB »

* **Finalité principale :** Évaluation standardisée et délivrance d'une attestation objective d'aptitudes professionnelles transversales.
* **Garantie de non-discrimination :** Évaluation purement facultative. Le badge ne conditionne ni l'accès à la plateforme ni la visibilité du profil dans le catalogue de recherche.
* **Déconnexion d'ordre public :** Indépendance absolue vis-à-vis des droits à indemnisation ou allocations chômage (Article L. 5421-1 du Code du travail).
* **Base légale (RGPD) :** Mission d'intérêt public (Article 6-1-e).
* **Catégories de personnes concernées :** Candidats ayant volontairement passé l'évaluation.
* **Catégories de données traitées :** Réponses fournies aux 100 questions objectives standardisées, résultat chiffré global, statut d'obtention de l'attestation numérique (seuil fixé à 80 %), horodatage de délivrance.
* **Durée de conservation :** Associée à la durée de détention du compte actif. Suppression définitive lors de la clôture du compte.
* **Catégories de destinataires habilités :** Candidat usager et recruteurs consultant le profil public.
* **Environnement de stockage et sécurité :** Base de données relationnelle sécurisée, calcul algorithmique déterministe et transparent sans pondération opaque.

---

### FICHE 4 — MISE EN RELATION PROFESSIONNELLE ET MESSAGERIE

* **Finalité principale :** Échange professionnel direct entre employeurs habilités et candidats en recherche active.
* **Base légale (RGPD) :** Mesures précontractuelles prises à la demande de la personne concernée (Article 6-1-b).
* **Catégories de personnes concernées :** Candidats sollicités et recruteurs émetteurs.
* **Catégories de données traitées :** Identifiants des parties prenantes, corps du message professionnel, date et heure d'envoi.
* **Durée de conservation :** 12 mois glissants à compter du dernier échange intervenu entre les parties, puis purge définitive.
* **Catégories de destinataires habilités :** Candidat destinataire et recruteur émetteur exclusivement.
* **Environnement de stockage et sécurité :** Module de messagerie interne sécurisé, chiffrement en transit et au repos, interdiction absolue de cession ou transfert tiers.

---

### FICHE 5 — TRAÇABILITÉ, SÉCURITÉ ET AUDIT DU SYSTÈME

* **Finalité principale :** Détection des intrusions, maintien de l'intégrité de l'infrastructure, conformité aux réquisitions judiciaires et prévention des fraudes.
* **Base légale (RGPD & Droit interne) :** Obligation légale (Article 6-1-c RGPD et article L. 34-1 du CPCE).
* **Catégories de personnes concernées :** Tout utilisateur connecté à la plateforme.
* **Catégories de données traitées :** Adresses IP de connexion (anonymisées au terme du délai légal), horodatage UTC des requêtes, agents utilisateurs (User-Agent), identifiants de sessions techniques, codes de réponse HTTP.
* **Durée de conservation :** 12 mois à compter de leur enregistrement (durée légale impérative). Purge automatique quotidienne exécutée par tâche d'administration souveraine.
* **Catégories de destinataires habilités :** Équipe d'administration système et officiers de police judiciaire sur réquisition légale formelle.
* **Environnement de stockage et sécurité :** Serveur de journalisation dédié, intégrité garantie par hachage cryptographique et accès restreint au personnel habilité sécurité.

---

### MESURES DE SÉCURITÉ TECHNIQUES APPLICABLES (STANDARD CNIL & ANSSI)

1. **Chiffrement généralisé :** Chiffrement systématique des flux réseau sous protocole `TLS 1.3` et chiffrement au repos des bases et volumes via `AES-256`.
2. **Cloisonnement strict des données :** Séparation logique et étanchéité entre les données d'identification, les évaluations et les fichiers audiovisuels.
3. **Contrôle d'intégrité des flux :** Validation côté serveur des types MIME réels par octets magiques (*magic bytes*) et purge automatique des flux partiels.
4. **Zéro traqueur :** 0 octet sortant vers des régies tierces, polices Marianne et Spectral auto-hébergées en Woff2.
5. **Localisation souveraine :** Infrastructure et sauvegardes physiquement hébergées en France (SecNumCloud / Cloud de Confiance), hors portée du Cloud Act américain.
