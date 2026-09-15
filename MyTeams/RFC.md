# Norme RFC - Protocole MyTeams

## 1. Commandes Client (Client -> Serveur)

| Commande | Arguments | Description |
| :--- | :--- | :--- |
| `/help` | - | Liste les commandes disponibles |
| `/login` | `["user_name"]` | Connexion au serveur |
| `/logout` | - | Déconnexion du serveur |
| `/users` | - | Récupère la liste de tous les utilisateurs |
| `/user` | `["user_uuid"]` | Récupère les informations d'un utilisateur spécifique |
| `/send` | `["user_uuid"] ["message_body"]` | Envoie un message privé à un utilisateur |
| `/messages` | `["user_uuid"]` | Récupère l'historique des messages privés avec un utilisateur |
| `/subscribe` | `["team_uuid"]` | S'abonne à une équipe |
| `/subscribed` | `?["team_uuid"]` | Liste les équipes abonnées, ou liste les utilisateurs abonnés à une équipe |
| `/unsubscribe` | `["team_uuid"]` | Se désabonne d'une équipe |
| `/use` | `?["team_uuid"] ?["channel_uuid"] ?["thread_uuid"]` | Définit le contexte d'exécution des commandes |
| `/create` | *(Dépend du contexte)* | Crée une équipe, un canal, un thread ou une réponse |
| `/list` | - | Liste toutes les équipes, canaux, threads ou réponses selon le contexte |
| `/info` | - | Récupère les informations de l'utilisateur actuel, de l'équipe, du canal ou du thread selon le contexte |

*(Note : Ce sont les actions saisies par l'utilisateur sur le client, qui sont envoyées au serveur. La syntaxe exacte sur le socket peut simplement être la chaîne brute, par ex. `/login "userName"`).*

---

## 2. Réponses Serveur (Serveur -> Client)

Les réponses sont formatées de la façon suivante : `<Code>` suivi de façon optionnelle par ` "Arg1" "Arg2"...` séparés par des espaces et entourés de guillemets.

### 200 - Succès & Événements

| Code | Nom | Format |
| :---: | :--- | :--- |
| **201** | Aide | `201` |
| **220** | Connecté | `220 "userUuid" "userName"` |
| **221** | Déconnecté | `221 "userUuid" "userName"` |
| **250** | Commande User | `250 "userUuid" "userName" "userStatus"` |
| **251** | Commande Users | `251 "userUuid" "userName" "userStatus"` |
| **260** | Message Reçu (Événement)| `260 "senderUuid" "messageBody"` |
| **261** | Historique Messages | `261 "senderUuid" "timestamp" "messageBody"` |
| **270** | Abonnement | `270 "userUuid" "teamUuid"` |
| **271** | Désabonnement | `271 "userUuid" "teamUuid"` |

### 210 - Contexte Commande Info

| Code | Nom | Format |
| :---: | :--- | :--- |
| **211** | Info Thread | `211 "threadUuid" "userUuid" "threadTimestamp" "threadTitle" "threadBody"` |
| **212** | Info Canal | `212 "channelUuid" "channelName" "channelDescription"` |
| **213** | Info Équipe | `213 "teamUuid" "teamName" "teamDescription"` |

### 280 - Contexte Commande List

| Code | Nom | Format |
| :---: | :--- | :--- |
| **281** | Liste Réponses | `281 "threadUuid" "userUuid" "replyTimestamp" "replyBody"` |
| **282** | Liste Threads | `282 "threadUuid" "userUuid" "threadTimestamp" "threadTitle" "threadBody"` |
| **283** | Liste Canaux | `283 "channelUuid" "channelName" "channelDescription"` |
| **284** | Liste Équipes | `284 "teamUuid" "teamName" "teamDescription"` |

### 290 - Création & Événements de Notification Serveur

| Code | Nom | Format |
| :---: | :--- | :--- |
| **291** | Réponse Créée (Création) | `291 "threadUuid" "userUuid" "replyTimestamp" "replyBody"` |
| **292** | Thread Créé (Création)| `292 "threadUuid" "userUuid" "threadTimestamp" "threadTitle" "threadBody"` |
| **293** | Canal Créé (Création)| `293 "channelUuid" "channelName" "channelDescription"` |
| **294** | Équipe Créée (Création) | `294 "teamUuid" "teamName" "teamDescription"` |
| **295** | Notif. Réponse (Événement)| `295 "teamUuid" "threadUuid" "userUuid" "replyBody"` |
| **296** | Notif. Thread (Événement)| `296 "threadUuid" "userUuid" "threadTimestamp" "threadTitle" "threadBody"` |
| **297** | Notif. Canal (Événement)| `297 "channelUuid" "channelName" "channelDescription"` |
| **298** | Notif. Équipe (Événement) | `298 "teamUuid" "teamName" "teamDescription"` |

### 500 - Erreurs & Échecs

| Code | Nom | Format |
| :---: | :--- | :--- |
| **501** | Non Autorisé | `501` |
| **502** | Mauvais Arguments | `502` |
| **503** | Utilisateur Introuvable | `503 "userUuid"` |
| **504** | Équipe Introuvable | `504 "teamUuid"` |
| **505** | Canal Introuvable | `505 "channelUuid"` |
| **506** | Thread Introuvable | `506 "threadUuid"` |
| **507** | Existe Déjà | `507` |