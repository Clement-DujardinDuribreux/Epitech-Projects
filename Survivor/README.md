# PROFILSACTIFS

Plateforme innovante de recrutement vidéo et de certification des compétences transversales.

PROFILSACTIFS remplace le CV classique par des vidéos courtes au format vertical (9:16) et une certification des compétences transversales, facilitant la rencontre directe et sans discrimination entre candidats et employeurs.

---

## Fonctionnalités principales

- **Catalogue Recruteur (`/recruteur/dashboard`)** : Découverte des candidats en grille paginée (20 par page), filtres par compétences, secteur, localisation et certification JEB, avec mise en favoris et contact direct.
- **Profils Candidats (`/profils/:id`)** : Fiche complète avec lecteur vidéo, badge de certification d'aptitudes, compétences, métriques professionnelles et bouton de sollicitation directe.
- **Certification des Aptitudes (`/certification`)** : Questionnaire standardisé certifiant les aptitudes professionnelles des candidats.
- **Pack Légal & Conformité (`/conformite`)** : Espace regroupant les CGU, le registre RGPD (Art. 30), la déclaration d'accessibilité RGAA 4.1 et les garanties d'hébergement souverain en France/UE.

---

## Installation et Lancement

### Avec Docker (Recommandé)

```bash
# 1. Cloner le projet
git clone <URL_DU_DEPOT>
cd G-SVR-500-LIL-5-1-survivor-6

# 2. Configurer les variables d'environnement
cp .env.example .env

# 3. Lancer l'application
docker compose up --build
```

L'application est disponible sur :
- **Frontend** : [http://localhost:10001](http://localhost:10001)
- **Backend (API)** : [http://localhost:10000](http://localhost:10000)
- **Documentation API (Swagger)** : [http://localhost:10000/docs](http://localhost:10000/docs)

---

### En développement local

**Frontend (Vue 3 / TypeScript)** :
```bash
cd frontend
npm install
npm run dev
```
Accès local : [http://localhost:5173/recruteur/dashboard](http://localhost:5173/recruteur/dashboard)

**Backend (FastAPI / uv)** :
```bash
cd backend
uv sync
uv run uvicorn main:app --reload --port 8000
```

---

## Liens utiles

- **Catalogue Recruteur** : [http://localhost:5173/recruteur/dashboard](http://localhost:5173/recruteur/dashboard)
- **Exemple de profil (Léa D.)** : [http://localhost:5173/profils/lea-d](http://localhost:5173/profils/lea-d)
- **Pack Légal & RGPD** : [http://localhost:5173/conformite](http://localhost:5173/conformite)
- **Comptes de test & Rôles BDD** : voir [docs/COMPTE_ADMIN.md](./docs/COMPTE_ADMIN.md)
- **Documentation technique avancée** : voir [frontend/docs/GUIDE_DEVELOPPEUR.md](./frontend/docs/GUIDE_DEVELOPPEUR.md)
