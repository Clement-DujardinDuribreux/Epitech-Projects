# Plan de Test — ProfilsActifs

Aucun test automatisé (pytest/vitest) n'existe dans le repo. Ce document couvre les tests manuels API + UI à faire pour valider le projet à fond.

> **Mise à jour (reset DB)** : après un `docker compose down -v` + `docker compose up --build` (volume MySQL neuf), les bugs #1 (comptes de démo absents) et #3 (500 sur `/recruiter/solicitations`) sont **résolus**. L'espace Admin a donc pu être testé (voir section 4 et bugs #12-14 ci-dessous).
>
> **Mise à jour (10/09, corrections rapides)** : les 8 corrections listées en section B du récapitulatif (bugs **#2, #4, #6, #10, #11, #12, #13, #14**) ont été relues directement dans le code source le 10/09 et sont **confirmées présentes** (charset utf8mb4, `min_length=8` sur le mot de passe, champ `certificated` retiré des champs modifiables, route `/recruteur/contacts` ajoutée, fallback mock du catalogue supprimé, tracking recruteur conditionné à l'authentification, modération admin sans fallback silencieux, journal d'audit vide au lieu de données factices). ⚠️ Cette vérification est une **relecture de code**, pas un re-test API/UI en direct comme pour #1/#3 — à revalider vite en live si possible avant la défense, sinon le dire clairement au jury. Détail ligne par ligne dans le tableau *Bugs trouvés* ci-dessous.

---

## 0. Setup

```bash
cp .env.example .env
# remplir MYSQL_HOST=db, MYSQL_PORT=3306, MYSQL_DATABASE, MYSQL_USER, MYSQL_PASSWORD, MYSQL_ROOT_PASSWORD, AUTH_SECRET_KEY
docker compose up --build
```

- Front : http://localhost:10001
- API : http://localhost:10000
- Swagger : http://localhost:10000/docs (tester chaque endpoint direct ici, pratique)
- `GET /health` doit retourner `{"status":"ok"}` (sinon DB pas connectée)

Comptes de démo (`docs/COMPTE_ADMIN.md`) :

| Rôle | Email | Mot de passe |
|---|---|---|
| Admin | admin@profilsactifs.gouv.fr | AdminProfils2026! |
| Recruteur | recruteur@profilsactifs.fr | Recruteur2026! |
| Candidat | lea.dubois@profilsactifs.fr | DemoCandidat2026! |

Ouvrir la console navigateur (F12) sur chaque page testée : zéro erreur JS/warning Vue attendu.

---

## 1. Auth (`/auth`)

- [x] `POST /auth/register` job_seeker : email valide, password, firstname, name → 201, user créé
- [x] `POST /auth/register` recruiter : idem avec `role: "recruiter"`
- [x] Register avec email déjà utilisé → erreur propre (pas 500) — 409 `Email already registered`
- [x] Register avec email invalide → 422 — OK
- [x] Register avec password vide → **corrigé** : `RegisterRequest.password` a maintenant `Field(min_length=8)` dans `schemas/auth.py` (bug #6, relu en code le 10/09, à reconfirmer en live avec un `POST /auth/register` password `""` → 422 attendu)
- [x] Register avec `birthdate` < 16 ans → refusé (422) — OK
- [x] `POST /auth/login` bon email/password → 200 + token JWT
- [x] Login mauvais password → 401, message générique identique à "email inconnu" (pas d'énumération de compte) — OK
- [x] `GET /auth/me` sans token → 401 ; avec token bidon → 401 ; avec token valide → infos user — OK
- [x] `GET /auth/me/age-status` : cohérent avec `birthdate` enregistrée — OK
- [x] `PATCH /auth/me/birthdate` : modifier, revérifier `age-status` — OK, refuse aussi < 16 ans
- [x] `DELETE /auth/delete` : supprime le compte, login suivant échoue bien (401) — OK
- [ ] Token expiré (24h) : non testé en conditions réelles (nécessite d'attendre 24h), mais `exp` bien présent dans le payload (vérifié via JWT décodé)
- [x] Header `Authorization` mal formé (pas de `Bearer `) → 401 — OK

### Front

- [ ] `/register`, `/login` : non testés au clavier/formulaire (testés uniquement via API), à refaire en UI manuelle
- [x] Accès `/login`/`/register` déjà connecté / accès `/dashboard` sans être connecté → logique de redirection présente et correcte dans `router/index.ts` (vérifié par lecture de code)

---

## 2. Espace Candidat

- [x] Profil / compétences / secteurs / vidéo : endpoints fonctionnels, testés via API
- [x] `POST /video/upload` fichier < 100 Mo → 200. **Fichier non-vidéo (.pdf, .html) → toujours accepté sans contrôle, bug #5 encore ouvert (correction complexe, voir récapitulatif C)**
- [x] `POST /certification` : seuil 80 % correctement appliqué (testé score 10/20 → non certifié, 17/20 → certifié)
- [x] Repasser la certif : le score se met à jour correctement
- [x] `GET /interactions` : fonctionne, retourne vues + sollicitations réelles

### Front

- [x] `/dashboard` : charge bien les vraies données via `/job_seeker/interactions` au premier rendu. **Actions "marquer lu/non lu", "archiver", "répondre" → toujours mockées, bug #9 encore ouvert (correction complexe, voir récapitulatif C)**
- [ ] `/certification` (reprise après interruption) : non testé en UI (nécessite un compte candidat, bloqué par l'absence de seed — voir tableau)

---

## 3. Espace Recruteur

- [x] `GET /feed/users` pagination + filtres (localisation, sector, certified) → fonctionne correctement en isolation
- [x] `GET /feed/user/{id}` → fonctionne
- [x] `POST /profile/follow/{id}` + `GET /profile/follow` + unfollow → fonctionnent
- [x] RBAC : token `job_seeker` sur endpoint recruteur → 403 ; token recruteur sur endpoint candidat → 403 ; token candidat sur endpoint admin → 403 — tout est correct
- [x] `POST /recruiter/solicitations`, `GET /recruiter/solicitations`, `PATCH .../status`, `DELETE ...` → **résolu après reset DB (voir mise à jour en tête de document), tous répondent 200/201 correctement maintenant**

### Front

- [x] `/recruteur/dashboard` : grille, filtres, favoris — fonctionnent avec l'API réelle. **Bug #2 (bascule sur 21 faux profils si la base réelle est vide) corrigé le 10/09** : le fallback automatique sur liste vide a été retiré de `CandidateHttpRepository.getAll()`, il ne reste qu'un fallback sur erreur réseau/API (comportement normal et attendu). À reconfirmer en live avec une base vide si le temps le permet
- [ ] `/recruteur/contact/:candidatId` : non testé en UI
- [x] `/recruteur/contacts` → **corrigé** : la route existe maintenant dans `router/index.ts` (redirige vers `/recruteur/dashboard?tab=suivi`), bug #11 résolu

---

## 4. Espace Admin

**Testé après reset de la DB (voir mise à jour en tête de document).**

- [x] `admin@profilsactifs.gouv.fr` / `AdminProfils2026!` → connexion OK, redirection vers `/admin`
- [x] `/admin` (dashboard KPIs) : chiffres réels et cohérents avec la DB (5 profils, 4 certifiés, 1 recruteur, 0 sollicitation, 0 vidéo en attente)
- [x] `/admin/kpis`, `/admin/media/pending`, valid/reject : le frontend appelle bien l'API réelle en priorité (fallback mock uniquement si l'appel échoue) — pas de bug de câblage ici
- [x] **Bug #13 corrigé le 10/09** : `AdminModerationView.vue` n'affiche plus de fausses vidéos mock — `loadItems()` n'utilise plus que l'API réelle, et si le `PATCH` de validation/rejet échoue, une `alert()` s'affiche à l'admin au lieu de faire disparaître l'item silencieusement comme si ça avait marché
- [x] **Bug #14 corrigé le 10/09** : le "Journal d'Audit & Activité Récente" du dashboard admin renvoie maintenant une liste vide (`recentActivities = []`) au lieu des 4 fausses entrées codées en dur. Honnête, mais la fonctionnalité elle-même (vrai journal d'activité) n'existe toujours pas côté backend — à assumer clairement si le jury demande "et le vrai journal, il est où ?"
- [ ] `PATCH /admin/media/reject/{id}` avec motif → **toujours cassé, bug #7 encore ouvert** : le motif choisi dans la modale n'est jamais envoyé au backend, qui n'a de toute façon aucune colonne pour le stocker (correction complexe, voir récapitulatif C)
- [x] `/admin/certification` : toujours 100% mock, aucun appel API au chargement (network vide), bug #8 encore ouvert (correction complexe, voir récapitulatif C)
- [x] Accès `/admin/*` avec token non-admin → 403 (`require_admin`) — vérifié précédemment

---

## 5. Pages légales / statiques

- [x] `/cgu` : rendu correct, pas de HTML brut. **Le texte de l'article 4 affirme toujours un contrôle "magic bytes" du type MIME qui n'existe pas dans le code — incohérence documentaire/technique tant que le bug #5 n'est pas corrigé, voir tableau**
- [x] `/conformite` : onglets s'affichent sans erreur
- [x] `/500`, route inconnue → `/404` : pages d'erreur correctes

---

## 6. Sécurité

- [x] IDOR profil candidat / sollicitations : non reproductible, tout est bien scopé sur l'ID du token JWT
- [x] CORS : restreint à localhost par regex — correct en dev
- [x] Mot de passe : jamais renvoyé dans aucune réponse API vérifiée
- [x] Injection SQL : toutes les requêtes dans `repositories/*.py` sont paramétrées — aucune faille trouvée
- [x] Upload vidéo : pas de path traversal (nom de fichier régénéré en UUID côté serveur) — **mais absence totale de contrôle de type de fichier, bug #5 encore ouvert**
- [x] Rate limiting : confirmé absent sur `/auth/login` (déjà noté comme hors scope par le plan)
- [x] Mot de passe vide accepté à l'inscription → **corrigé le 10/09** (`Field(min_length=8)`, bug #6)

---

## 7. Responsive / cross-browser

- [x] `/recruteur/dashboard` testé à 375px (mobile) : mise en page correcte, onglets et filtres en scroll horizontal (comportement mobile standard, pas un bug)
- [ ] `/dashboard`, `/profils/:id`, `/certification` en mobile : non testés (nécessite compte candidat opérationnel)
- [ ] Firefox : non testé (session limitée à un navigateur Chromium)

---

## 8. Non-régression après chaque test

- Confirmé : `/job_seeker/certification`, `/auth/*`, `/profile/follow` persistent bien en base (revérifiés via GET après action)
- **Toujours cassé : les actions de la boîte de réception candidat (lu/non lu/archive/réponse) ne persistent pas en base, bug #9 encore ouvert (correction complexe, voir récapitulatif C)**
- `docker compose logs backend` non consultable depuis cette session (pas d'accès shell à la machine) — à vérifier manuellement pour confirmer la cause exacte du bug 500 sur `/recruiter/solicitations` (déjà résolu depuis, mais la cause précise n'a jamais été confirmée par les logs)

---

## RÉCAPITULATIF FINAL — à voir en un coup d'oeil

### A. Tests manuels restants (pas encore faits, à faire à la main)

- [ ] `/register` et `/login` : remplir les formulaires au clavier (pas juste via API), essayer de les faire échouer (email sans @, mot de passe vide, champs vides) et vérifier les messages d'erreur affichés + zéro erreur console (F12)
- [ ] Tester dans Firefox en parallèle de Chrome (cette session n'a testé qu'un navigateur Chromium) — comparer surtout la lecture vidéo et les inputs de date
- [ ] `/dashboard`, `/profils/:id`, `/certification` en affichage mobile (375px) — seul `/recruteur/dashboard` a été vérifié en mobile
- [ ] `/certification` : recharger la page en plein quiz et vérifier que la progression est conservée (annoncé dans les commits, jamais vérifié en UI)
- [ ] `/recruteur/contact/:candidatId` : tester le formulaire d'envoi de sollicitation en UI (testé côté API seulement)
- [ ] Token expiré 24h : attendre réellement 24h (ou modifier `VALID_TIME` temporairement) et vérifier le 401
- [ ] **Nouveau (10/09)** : les 8 corrections ci-dessous n'ont été vérifiées que par relecture de code, pas re-testées en live — si le temps le permet avant la défense, refaire au moins : `/recruteur/dashboard` avec DB vide (bug #2), `POST /auth/register` avec mot de passe vide (bug #6), `/recruteur/contacts` en navigation (bug #11), console F12 sur la page d'accueil déconnecté (bug #10)

### B. Corrections rapides — toutes faites et confirmées en code le 10/09

- [x] Encodage accentué cassé ("LÃ©a" au lieu de "Léa") → `charset='utf8mb4'` ajouté dans `core/database.py` (bug #12)
- [x] Mot de passe vide accepté à l'inscription → longueur minimale ajoutée sur `password` dans `schemas/auth.py` (bug #6)
- [x] Auto-certification via `PUT /job_seeker/profile` → champ `certificated` retiré de `JobSeekerProfileUpdate` (bug #4)
- [x] Route `/recruteur/contacts` manquante (404) → ajoutée dans `router/index.ts` (bug #11)
- [x] Erreurs console 401 sur chaque page → l'API recruteur-tracking n'est plus appelée sans token (bug #10)
- [x] Catalogue recruteur qui affichait 21 faux profils quand la base est vide → fallback mock retiré de `CandidateHttpRepository.ts` (bug #2)
- [x] File de modération admin avec 5 fausses vidéos qui "disparaissaient" au clic → fallback retiré dans `AdminModerationView.vue` (bug #13)
- [x] Journal d'audit admin codé en dur → remplacé par une liste vide honnête (bug #14)

### C. Corrections plus complexes (toujours à faire — plusieurs fichiers + backend + parfois la base de données)

- [x] Boîte de réception candidat qui ne persiste rien (lu/non lu/archivé/réponse) → il n'existe aucun endpoint backend pour ça, il faut le créer (colonne(s) de statut + route + rebrancher `DashboardView.vue` sur du réel au lieu de `SolicitationMockRepository`) (bug #9)
- [x] Motif de rejet vidéo jamais transmis ni stocké → ajouter une colonne `reason` à la table `media`, la faire remonter dans `PATCH /admin/media/reject/{id}`, l'exposer au candidat (bug #7)
- [ ] Gestion des questions de certification 100% mockée → créer une vraie table + des endpoints CRUD backend, puis rebrancher `/admin/certification` ET le quiz réel (`questions.json`) dessus (bug #8)
- [x] Upload vidéo sans contrôle de type réel → ajouter une inspection des magic bytes côté backend (ex. lib `python-magic`) dans `services/job_seeker.py::save_uploaded_video`, comme le CGU l'annonce déjà (bug #5)
