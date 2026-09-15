<script setup lang="ts">
import { RouterLink } from 'vue-router'
import { Info } from 'lucide-vue-next'
</script>

<template>
  <footer class="mt-auto shrink-0 border-t border-border bg-card text-card-foreground font-body">
    <div class="container mx-auto px-4 py-10">
      <div class="grid grid-cols-1 md:grid-cols-4 gap-8">
        <div>
          <h3 class="text-lg font-bold mb-3 flex items-center gap-1 font-heading">
            <span class="text-white">Profils</span><span class="text-[#f5c842]">Actifs</span>
          </h3>
          <p class="text-sm text-muted-foreground leading-relaxed font-body">
            Plateforme de mise en relation professionnelle. Le CV vidéo et la valorisation des compétences.
          </p>
        </div>

        <div>
          <h4 class="text-sm font-semibold uppercase tracking-wider mb-3 font-heading">Candidats</h4>
          <ul class="space-y-2 text-sm font-body">
            <li>
              <RouterLink to="/" class="text-muted-foreground hover:text-foreground transition-colors">
                Catalogue de profils
              </RouterLink>
            </li>
            <li>
              <RouterLink to="/certification" class="text-muted-foreground hover:text-foreground transition-colors">
                Certification
              </RouterLink>
            </li>
            <li>
              <RouterLink to="/dashboard" class="text-muted-foreground hover:text-foreground transition-colors">
                Tableau de bord
              </RouterLink>
            </li>
            <li>
              <RouterLink to="/dashboard/profile" class="text-muted-foreground hover:text-foreground transition-colors">
                Gérer ma vidéo
              </RouterLink>
            </li>
          </ul>
        </div>

        <div>
          <h4 class="text-sm font-semibold uppercase tracking-wider mb-3 font-heading">Recruteurs</h4>
          <ul class="space-y-2 text-sm font-body">
            <li>
              <RouterLink to="/recruteur/dashboard" class="text-muted-foreground hover:text-foreground transition-colors">
                Espace Recruteur
              </RouterLink>
            </li>
            <li>
              <RouterLink to="/login" class="text-muted-foreground hover:text-foreground transition-colors">
                Connexion
              </RouterLink>
            </li>
            <li>
              <RouterLink to="/register" class="text-muted-foreground hover:text-foreground transition-colors">
                Créer un compte
              </RouterLink>
            </li>
            <li>
              <RouterLink to="/admin" class="text-muted-foreground hover:text-foreground transition-colors">
                Administration
              </RouterLink>
            </li>
          </ul>
        </div>

        <div>
          <h4 class="text-sm font-semibold uppercase tracking-wider mb-3 font-heading">Informations</h4>
          <ul class="space-y-2 text-sm font-body">
            <li>
              <span class="text-muted-foreground">Support : contact@profilsactifs.fr</span>
            </li>
          </ul>
        </div>
      </div>

      <!-- Avertissement légal obligatoire -->
      <div class="mt-8 p-3.5 rounded-xl border border-amber-500/25 bg-amber-500/10 text-amber-200/90 text-xs flex items-center gap-2.5">
        <Info class="w-4 h-4 text-amber-400 shrink-0" />
        <p class="font-medium">
          Démonstrateur technique, ne constitue pas un service public en exploitation.
        </p>
      </div>

      <div class="mt-8 pt-6 border-t border-border flex flex-col sm:flex-row justify-between items-center text-xs text-muted-foreground gap-4 font-body">
        <p>© 2026 ProfilsActifs — Démonstrateur technique, ne constitue pas un service public en exploitation.</p>
        <div class="flex gap-4 font-heading">
          <RouterLink to="/feed" class="hover:text-foreground transition-colors">Feed Vidéo</RouterLink>
          <RouterLink to="/recruteur/dashboard" class="hover:text-foreground transition-colors">Catalogue Recruteur</RouterLink>
        </div>
      </div>
    </div>
  </footer>
</template>
