# Comptes de Démonstration & Rôles — ProfilsActifs

Ce document récapitule les identifiants préconfigurés en base de données MySQL (`backend/sql/ProfilsActifs.sql`) ainsi que les permissions, rôles et URLs associées à chaque typologie d'utilisateur sur la plateforme **ProfilsActifs**.

---

## 1. Comptes de Test & Identifiants

| Rôle | Adresse e-mail | Mot de passe | Statut BDD |
| :--- | :--- | :--- | :--- |
| **Administrateur** | `admin@profilsactifs.gouv.fr` | `AdminProfils2026!` | Actif (ID: `a0000000-0000-0000-0000-000000000001`) |
| **Recruteur** | `recruteur@profilsactifs.fr` | `Recruteur2026!` | Actif (ID: `r0000000-0000-0000-0000-000000000001`) |
| **Candidat (Démo)** | `lea.dubois@profilsactifs.fr` | `DemoCandidat2026!` | Actif (ID: `c1000000-0000-0000-0000-000000000001`, Certifié JEB) |

> **Remarque de sécurité** : Tous les mots de passe sont hashés en base de données via **Bcrypt** avec sel renforcé (12 rounds).

---

## 2. Présentation des Rôles et Permissions

### 2.1. Espace Administration (`admin`)

L'administrateur veille à la conformité institutionnelle, au respect de la charte déontologique et au suivi des indicateurs opérationnels nationaux.

- **Permissions** :
  - Consultation des indicateurs clés en temps réel (KPIs) issus de la base MySQL (`GET /admin/kpis`).
  - File de modération déontologique et RGPD des vidéos candidates (`GET /admin/media/pending`).
  - Validation et publication de capsules vidéo (`PATCH /admin/media/valid/{id}`).
  - Rejet motivé avec motif républicain et message d'accompagnement pour le candidat (`PATCH /admin/media/reject/{id}`).
  - Pilotage du référentiel des 20 questions standardisées de certification JEB.
  - Exportation de rapports d'audit au format JSON.
- **URLs associées** :
  - Tableau de bord KPIs : [http://localhost:10001/admin/dashboard](http://localhost:10001/admin/dashboard)
  - File de Modération Vidéo : [http://localhost:10001/admin/moderation](http://localhost:10001/admin/moderation)
  - Référentiel de Certification : [http://localhost:10001/admin/certification](http://localhost:10001/admin/certification)

---

### 2.2. Espace Recruteur (`recruiter`)

Le recruteur dispose d'un espace d'exploration des talents centré sur les compétences transversales et le pitch vidéo court.

- **Permissions** :
  - Exploration du catalogue de profils avec pagination (20 par page) et filtres (métier, secteur, localisation, certification JEB).
  - Consultation de la fiche détaillée d'un candidat, enregistrant automatiquement une vue de profil (`POST /feed/user/{id}/view`).
  - Ajout et suppression de candidats dans la liste des favoris (`POST /profile/follow/{id}`, `DELETE /profile/unfollow/{id}`).
  - Envoi de sollicitations professionnelles directes (`POST /recruiter/solicitations`).
  - Suivi des prises de contact et des entretiens planifiés (`GET /recruiter/solicitations`, `PATCH /recruiter/solicitations/{id}/status`, `DELETE /recruiter/solicitations/{id}`).
- **URLs associées** :
  - Catalogue Recruteur : [http://localhost:10001/recruteur/dashboard](http://localhost:10001/recruteur/dashboard)
  - Suivi des Contacts : [http://localhost:10001/recruteur/contacts](http://localhost:10001/recruteur/contacts)
  - Exemple de profil candidat : [http://localhost:10001/profils/c1000000-0000-0000-0000-000000000001](http://localhost:10001/profils/c1000000-0000-0000-0000-000000000001)

---

### 2.3. Espace Candidat (`job_seeker`)

Le candidat valorise ses compétences professionnelles et ses soft skills sans discrimination à l'embauche.

- **Permissions** :
  - Téléversement direct de sa capsule vidéo (< 100 Mo) ou renseignement d'une URL sécurisée.
  - Renseignement de ses compétences, secteurs de recherche et bio professionnelle.
  - Passage du test de certification des aptitudes transversales (obtention du Badge JEB si score ≥ 80%).
  - Boîte de réception des sollicitations transmises par les recruteurs avec possibilité de réponse directe.
  - Consultation des statistiques d'impact de son profil (vues recruteurs, prises de contact).
- **URLs associées** :
  - Espace Candidat : [http://localhost:10001/dashboard](http://localhost:10001/dashboard)
  - Édition du Profil : [http://localhost:10001/candidat/profil](http://localhost:10001/candidat/profil)
  - Test de Certification : [http://localhost:10001/certification](http://localhost:10001/certification)

---

## 3. URLs d'Accès & Environnements

| Composant | URL Locale | Description |
| :--- | :--- | :--- |
| **Application Web (Frontend)** | [http://localhost:10001](http://localhost:10001) | Interface Vue 3 / Vite / Tailwind CSS |
| **API Backend (FastAPI)** | [http://localhost:10000](http://localhost:10000) | Serveur REST Python FastAPI |
| **Documentation Swagger (API Docs)** | [http://localhost:10000/docs](http://localhost:10000/docs) | Spécification OpenAPI interactive |
| **Documentation ReDoc** | [http://localhost:10000/redoc](http://localhost:10000/redoc) | Documentation API alternative |
