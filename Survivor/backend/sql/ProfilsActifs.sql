CREATE TABLE IF NOT EXISTS `users`(
    `id` CHAR(36) NOT NULL,
    `email` VARCHAR(255) NOT NULL,
    `firstname` VARCHAR(255) NOT NULL,
    `name` VARCHAR(255) NOT NULL,
    `password` VARCHAR(255) NOT NULL,
    `role` ENUM('job_seeker', 'recruiter', 'admin') NOT NULL DEFAULT 'job_seeker',
    `created_at` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP(),
    `last_update_at` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP(),
    `localisation` VARCHAR(255) NULL,
    `certificated` BOOLEAN NULL,
    `birthdate` DATE NULL,
    `headline` VARCHAR(255) NULL,
    `bio` TEXT NULL,
    `phone` VARCHAR(30) NULL,
    `avatar_url` VARCHAR(500) NULL,
    UNIQUE KEY `users_email_unique` (`email`),
    PRIMARY KEY(`id`)
);


CREATE TABLE IF NOT EXISTS `media`(
    `id` CHAR(36) NOT NULL,
    `user_id` CHAR(36) NOT NULL,
    `link` VARCHAR(255) NOT NULL,
    `valid` BOOLEAN NOT NULL DEFAULT 0,
    `validate_by` CHAR(36) NULL,
    `reason` TEXT NULL,
    PRIMARY KEY(`id`)
);


CREATE TABLE IF NOT EXISTS `follow`(
    `id` CHAR(36) NOT NULL,
    `user_id_recruiter` CHAR(36) NOT NULL,
    `user_id_js` CHAR(36) NOT NULL,
    UNIQUE KEY `follow_recruiter_js_unique` (`user_id_recruiter`, `user_id_js`),
    PRIMARY KEY(`id`)
);


CREATE TABLE IF NOT EXISTS `competences`(
    `id` CHAR(36) NOT NULL,
    `user_id` CHAR(36) NOT NULL,
    `competence` VARCHAR(255) NOT NULL,
    PRIMARY KEY(`id`)
);


CREATE TABLE IF NOT EXISTS `sector`(
    `id` CHAR(36) NOT NULL,
    `user_id` CHAR(36) NOT NULL,
    `sector` VARCHAR(255) NOT NULL,
    PRIMARY KEY(`id`)
);


CREATE TABLE IF NOT EXISTS `solicitations`(
    `id` CHAR(36) NOT NULL,
    `recruiter_id` CHAR(36) NOT NULL,
    `job_seeker_id` CHAR(36) NOT NULL,
    `message` TEXT NOT NULL,
    `status` ENUM('sent', 'contacted', 'interview', 'rejected') NOT NULL DEFAULT 'sent',
    `is_read` BOOLEAN NOT NULL DEFAULT 0,
    `is_archived` BOOLEAN NOT NULL DEFAULT 0,
    `reply` TEXT NULL,
    `replied_at` TIMESTAMP NULL,
    `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP(),
    `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP() ON UPDATE CURRENT_TIMESTAMP(),
    PRIMARY KEY(`id`)
);


CREATE TABLE IF NOT EXISTS `profile_views`(
    `id` CHAR(36) NOT NULL,
    `job_seeker_id` CHAR(36) NOT NULL,
    `recruiter_id` CHAR(36) NULL,
    `viewed_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP(),
    PRIMARY KEY(`id`)
);


-- Colonnes ajoutées après la mise en service initiale : idempotentes (IF NOT EXISTS)
-- pour pouvoir être ré-appliquées sur une base déjà existante (le script ci-dessus
-- ne s'exécute qu'une fois, au tout premier démarrage du volume MySQL).
ALTER TABLE `users` ADD COLUMN IF NOT EXISTS `headline` VARCHAR(255) NULL;
ALTER TABLE `users` ADD COLUMN IF NOT EXISTS `bio` TEXT NULL;
ALTER TABLE `users` ADD COLUMN IF NOT EXISTS `phone` VARCHAR(30) NULL;
ALTER TABLE `users` ADD COLUMN IF NOT EXISTS `avatar_url` VARCHAR(500) NULL;
ALTER TABLE `solicitations` ADD COLUMN IF NOT EXISTS `is_read` BOOLEAN NOT NULL DEFAULT 0;
ALTER TABLE `solicitations` ADD COLUMN IF NOT EXISTS `is_archived` BOOLEAN NOT NULL DEFAULT 0;
ALTER TABLE `solicitations` ADD COLUMN IF NOT EXISTS `reply` TEXT NULL;
ALTER TABLE `solicitations` ADD COLUMN IF NOT EXISTS `replied_at` TIMESTAMP NULL;


-- Contraintes et clés étrangères
ALTER TABLE `competences` ADD CONSTRAINT `competences_user_id_foreign` FOREIGN KEY(`user_id`) REFERENCES `users`(`id`) ON DELETE CASCADE;
ALTER TABLE `media` ADD CONSTRAINT `media_user_id_foreign` FOREIGN KEY(`user_id`) REFERENCES `users`(`id`) ON DELETE CASCADE;
ALTER TABLE `media` ADD CONSTRAINT `media_validate_by_foreign` FOREIGN KEY(`validate_by`) REFERENCES `users`(`id`) ON DELETE SET NULL;
ALTER TABLE `sector` ADD CONSTRAINT `sector_user_id_foreign` FOREIGN KEY(`user_id`) REFERENCES `users`(`id`) ON DELETE CASCADE;
ALTER TABLE `follow` ADD CONSTRAINT `follow_user_id_js_foreign` FOREIGN KEY(`user_id_js`) REFERENCES `users`(`id`) ON DELETE CASCADE;
ALTER TABLE `follow` ADD CONSTRAINT `follow_user_id_recruiter_foreign` FOREIGN KEY(`user_id_recruiter`) REFERENCES `users`(`id`) ON DELETE CASCADE;
ALTER TABLE `solicitations` ADD CONSTRAINT `solicitations_recruiter_foreign` FOREIGN KEY(`recruiter_id`) REFERENCES `users`(`id`) ON DELETE CASCADE;
ALTER TABLE `solicitations` ADD CONSTRAINT `solicitations_job_seeker_foreign` FOREIGN KEY(`job_seeker_id`) REFERENCES `users`(`id`) ON DELETE CASCADE;
ALTER TABLE `profile_views` ADD CONSTRAINT `profile_views_job_seeker_foreign` FOREIGN KEY(`job_seeker_id`) REFERENCES `users`(`id`) ON DELETE CASCADE;


-- ============================================================================
-- Données initiales de démonstration (Seed)
-- ============================================================================

-- 1. Compte Administrateur par défaut
-- Identifiants : admin@profilsactifs.gouv.fr / AdminProfils2026!
INSERT INTO `users` (`id`, `email`, `firstname`, `name`, `password`, `role`, `localisation`, `certificated`, `birthdate`)
VALUES (
    'a0000000-0000-0000-0000-000000000001',
    'admin@profilsactifs.gouv.fr',
    'Super',
    'Admin',
    '$2b$12$Rhva3rGaF6h/hTNTkrkSheMkLuIYS0m7RDpyuemeOhHmQ7c8nzwLm',
    'admin',
    'Paris, 75',
    1,
    '1985-01-01'
) ON DUPLICATE KEY UPDATE `role` = 'admin';

-- 2. Compte Recruteur Partenaire
-- Identifiants : recruteur@profilsactifs.fr / Recruteur2026!
INSERT INTO `users` (`id`, `email`, `firstname`, `name`, `password`, `role`, `localisation`, `certificated`, `birthdate`)
VALUES (
    'r0000000-0000-0000-0000-000000000001',
    'recruteur@profilsactifs.fr',
    'Sophie',
    'Recrutement',
    '$2b$12$TZKPR0OqbTQaIMJx.e0vz.K8QwdUIbUGF5QFKxsEuQoJgcxY5gc.2',
    'recruiter',
    'Paris, 75',
    NULL,
    '1990-05-15'
) ON DUPLICATE KEY UPDATE `role` = 'recruiter';

-- 3. Candidats de démonstration avec compétences, secteurs et pitchs vidéo
-- Mot de passe commun : DemoCandidat2026!
INSERT INTO `users` (`id`, `email`, `firstname`, `name`, `password`, `role`, `localisation`, `certificated`, `birthdate`)
VALUES
(
    'c1000000-0000-0000-0000-000000000001',
    'lea.dubois@profilsactifs.fr',
    'Léa',
    'Dubois',
    '$2b$12$hz3aZTSI5dkBJMofSgZqcu7Jclgewk4yhV4OU2ipfm7Uzwl6ThE2.',
    'job_seeker',
    'Paris, 75',
    1,
    '1998-03-12'
),
(
    'c1000000-0000-0000-0000-000000000002',
    'thomas.robert@profilsactifs.fr',
    'Thomas',
    'Robert',
    '$2b$12$hz3aZTSI5dkBJMofSgZqcu7Jclgewk4yhV4OU2ipfm7Uzwl6ThE2.',
    'job_seeker',
    'Lyon, 69',
    1,
    '1995-07-24'
),
(
    'c1000000-0000-0000-0000-000000000003',
    'sarah.benali@profilsactifs.fr',
    'Sarah',
    'Benali',
    '$2b$12$hz3aZTSI5dkBJMofSgZqcu7Jclgewk4yhV4OU2ipfm7Uzwl6ThE2.',
    'job_seeker',
    'Lille, 59',
    1,
    '2000-11-05'
),
(
    'c1000000-0000-0000-0000-000000000004',
    'lucas.bernard@profilsactifs.fr',
    'Lucas',
    'Bernard',
    '$2b$12$hz3aZTSI5dkBJMofSgZqcu7Jclgewk4yhV4OU2ipfm7Uzwl6ThE2.',
    'job_seeker',
    'Bordeaux, 33',
    0,
    '1997-09-18'
),
(
    'c1000000-0000-0000-0000-000000000005',
    'camille.martin@profilsactifs.fr',
    'Camille',
    'Martin',
    '$2b$12$hz3aZTSI5dkBJMofSgZqcu7Jclgewk4yhV4OU2ipfm7Uzwl6ThE2.',
    'job_seeker',
    'Nantes, 44',
    1,
    '1996-01-30'
)
ON DUPLICATE KEY UPDATE `firstname` = VALUES(`firstname`);

-- Secteurs des candidats
INSERT INTO `sector` (`id`, `user_id`, `sector`) VALUES
('s0000000-0000-0000-0000-000000000001', 'c1000000-0000-0000-0000-000000000001', 'Informatique & Tech'),
('s0000000-0000-0000-0000-000000000002', 'c1000000-0000-0000-0000-000000000002', 'Management & Conseil'),
('s0000000-0000-0000-0000-000000000003', 'c1000000-0000-0000-0000-000000000003', 'Design & Création'),
('s0000000-0000-0000-0000-000000000004', 'c1000000-0000-0000-0000-000000000004', 'Commerce & Vente'),
('s0000000-0000-0000-0000-000000000005', 'c1000000-0000-0000-0000-000000000005', 'Ressources Humaines')
ON DUPLICATE KEY UPDATE `sector` = VALUES(`sector`);

-- Compétences des candidats
INSERT INTO `competences` (`id`, `user_id`, `competence`) VALUES
('k0000000-0000-0000-0000-000000000001', 'c1000000-0000-0000-0000-000000000001', 'Vue.js'),
('k0000000-0000-0000-0000-000000000002', 'c1000000-0000-0000-0000-000000000001', 'TypeScript'),
('k0000000-0000-0000-0000-000000000003', 'c1000000-0000-0000-0000-000000000001', 'Tailwind CSS'),
('k0000000-0000-0000-0000-000000000004', 'c1000000-0000-0000-0000-000000000002', 'Gestion de projet'),
('k0000000-0000-0000-0000-000000000005', 'c1000000-0000-0000-0000-000000000002', 'Agile & Scrum'),
('k0000000-0000-0000-0000-000000000006', 'c1000000-0000-0000-0000-000000000003', 'UI/UX Design'),
('k0000000-0000-0000-0000-000000000007', 'c1000000-0000-0000-0000-000000000003', 'Figma'),
('k0000000-0000-0000-0000-000000000008', 'c1000000-0000-0000-0000-000000000004', 'Négociation'),
('k0000000-0000-0000-0000-000000000009', 'c1000000-0000-0000-0000-000000000004', 'Prospection'),
('k0000000-0000-0000-0000-000000000010', 'c1000000-0000-0000-0000-000000000005', 'Recrutement'),
('k0000000-0000-0000-0000-000000000011', 'c1000000-0000-0000-0000-000000000005', 'Droit du travail')
ON DUPLICATE KEY UPDATE `competence` = VALUES(`competence`);

-- Pitchs Vidéo des candidats
INSERT INTO `media` (`id`, `user_id`, `link`, `valid`, `validate_by`) VALUES
('m0000000-0000-0000-0000-000000000001', 'c1000000-0000-0000-0000-000000000001', 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4', 1, 'a0000000-0000-0000-0000-000000000001'),
('m0000000-0000-0000-0000-000000000002', 'c1000000-0000-0000-0000-000000000002', 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerEscapes.mp4', 1, 'a0000000-0000-0000-0000-000000000001'),
('m0000000-0000-0000-0000-000000000003', 'c1000000-0000-0000-0000-000000000003', 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerFun.mp4', 1, 'a0000000-0000-0000-0000-000000000001'),
('m0000000-0000-0000-0000-000000000004', 'c1000000-0000-0000-0000-000000000004', 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerJoyBlazes.mp4', 1, 'a0000000-0000-0000-0000-000000000001'),
('m0000000-0000-0000-0000-000000000005', 'c1000000-0000-0000-0000-000000000005', 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerMeltdowns.mp4', 1, 'a0000000-0000-0000-0000-000000000001')
ON DUPLICATE KEY UPDATE `link` = VALUES(`link`), `valid` = VALUES(`valid`);

-- 4. Candidat de test avec vidéo en attente de modération
-- Identifiants : julie.moreau@profilsactifs.fr / DemoCandidat2026!
INSERT INTO `users` (`id`, `email`, `firstname`, `name`, `password`, `role`, `localisation`, `certificated`, `birthdate`)
VALUES (
    'c1000000-0000-0000-0000-000000000006',
    'julie.moreau@profilsactifs.fr',
    'Julie',
    'Moreau',
    '$2b$12$hz3aZTSI5dkBJMofSgZqcu7Jclgewk4yhV4OU2ipfm7Uzwl6ThE2.',
    'job_seeker',
    'Marseille, 13',
    0,
    '1999-06-20'
) ON DUPLICATE KEY UPDATE `firstname` = VALUES(`firstname`);

INSERT INTO `sector` (`id`, `user_id`, `sector`) VALUES
('s0000000-0000-0000-0000-000000000006', 'c1000000-0000-0000-0000-000000000006', 'Marketing & Communication')
ON DUPLICATE KEY UPDATE `sector` = VALUES(`sector`);

INSERT INTO `competences` (`id`, `user_id`, `competence`) VALUES
('k0000000-0000-0000-0000-000000000012', 'c1000000-0000-0000-0000-000000000006', 'Community Management'),
('k0000000-0000-0000-0000-000000000013', 'c1000000-0000-0000-0000-000000000006', 'SEO/SEA')
ON DUPLICATE KEY UPDATE `competence` = VALUES(`competence`);

INSERT INTO `media` (`id`, `user_id`, `link`, `valid`) VALUES
('m0000000-0000-0000-0000-000000000006', 'c1000000-0000-0000-0000-000000000006', 'https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ElephantsDream.mp4', 0)
ON DUPLICATE KEY UPDATE `link` = VALUES(`link`), `valid` = VALUES(`valid`);
