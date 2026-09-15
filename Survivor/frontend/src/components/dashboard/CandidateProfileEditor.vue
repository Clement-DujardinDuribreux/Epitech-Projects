<script setup lang="ts">
import { ref, watch } from 'vue'
import { X, Plus, User, MapPin, Mail, Phone, Briefcase, FileText, CheckCircle2 } from 'lucide-vue-next'
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar'
import { Button } from '@/components/ui/button'
import { Badge } from '@/components/ui/badge'

export interface ICandidateProfileFormData {
  fullName: string
  headline: string
  location: string
  email: string
  phone: string
  bio: string
  skills: string[]
  avatarUrl: string
  avatarFile?: File
}

// Pas de fausses valeurs pré-remplies : un champ vide reste vide tant que le
// candidat (ou son profil chargé depuis le backend) ne l'a pas renseigné.
const DEFAULTS = {
  fullName: '',
  headline: '',
  location: '',
  email: '',
  phone: '',
  bio: '',
  avatarUrl: '',
  skills: [] as string[],
}

const props = defineProps<{
  initialData?: Partial<ICandidateProfileFormData>
}>()

const emit = defineEmits<{
  (e: 'save', data: ICandidateProfileFormData): void
}>()

const fullName = ref(props.initialData?.fullName || DEFAULTS.fullName)
const headline = ref(props.initialData?.headline || DEFAULTS.headline)
const location = ref(props.initialData?.location || DEFAULTS.location)
const email = ref(props.initialData?.email || DEFAULTS.email)
const phone = ref(props.initialData?.phone || DEFAULTS.phone)
const bio = ref(props.initialData?.bio || DEFAULTS.bio)
const avatarUrl = ref(props.initialData?.avatarUrl || DEFAULTS.avatarUrl)
const avatarFile = ref<File | undefined>(undefined)
const avatarInput = ref<HTMLInputElement | null>(null)

const skills = ref<string[]>(props.initialData?.skills ? [...props.initialData.skills] : [...DEFAULTS.skills])
const newSkill = ref('')
const saveSuccess = ref(false)

// Le profil réel est chargé de façon asynchrone par le parent : on resynchronise
// les champs (non modifiés localement par défaut) dès qu'il arrive.
watch(
  () => props.initialData,
  (data) => {
    if (!data) return
    fullName.value = data.fullName || DEFAULTS.fullName
    headline.value = data.headline || DEFAULTS.headline
    location.value = data.location || DEFAULTS.location
    email.value = data.email || DEFAULTS.email
    phone.value = data.phone || DEFAULTS.phone
    bio.value = data.bio || DEFAULTS.bio
    avatarUrl.value = data.avatarUrl || DEFAULTS.avatarUrl
    skills.value = data.skills ? [...data.skills] : [...DEFAULTS.skills]
  }
)

function pickAvatar(): void {
  avatarInput.value?.click()
}

function onAvatarChange(event: Event): void {
  const file = (event.target as HTMLInputElement).files?.[0]
  if (!file) return
  avatarFile.value = file
  avatarUrl.value = URL.createObjectURL(file)
}

function addSkill(): void {
  const value = newSkill.value.trim()
  if (!value || skills.value.includes(value)) return
  skills.value.push(value)
  newSkill.value = ''
}

function removeSkill(skill: string): void {
  skills.value = skills.value.filter((s) => s !== skill)
}

function handleSave(): void {
  emit('save', {
    fullName: fullName.value,
    headline: headline.value,
    location: location.value,
    email: email.value,
    phone: phone.value,
    bio: bio.value,
    skills: [...skills.value],
    avatarUrl: avatarUrl.value,
    avatarFile: avatarFile.value,
  })
  saveSuccess.value = true
  setTimeout(() => {
    saveSuccess.value = false
  }, 2500)
}
</script>

<template>
  <div class="rounded-xl bg-[#121218] border border-white/10 p-6 space-y-6">
    <div class="flex items-center justify-between border-b border-white/10 pb-4">
      <div>
        <h3 class="text-base font-bold font-heading text-white flex items-center gap-2">
          <User class="w-4 h-4 text-[#1a6fff]" />
          Édition du profil candidat
        </h3>
        <p class="text-xs text-white/50 font-body mt-0.5">
          Mettez à jour vos coordonnées et compétences visibles par les recruteurs
        </p>
      </div>

      <div v-if="saveSuccess" class="flex items-center gap-1.5 text-xs text-emerald-400 font-medium font-heading">
        <CheckCircle2 class="w-4 h-4 text-emerald-400" />
        Profil enregistré !
      </div>
    </div>

    <!-- Avatar et Nom -->
    <div class="flex flex-col sm:flex-row sm:items-center gap-5">
      <div class="relative group shrink-0">
        <Avatar size="xl" class="ring-2 ring-white/10">
          <AvatarImage v-if="avatarUrl" :src="avatarUrl" :alt="fullName" />
          <AvatarFallback class="bg-[#1a1a24] text-[#00d4ff] font-heading font-bold text-lg">
            {{ fullName.slice(0, 2) }}
          </AvatarFallback>
        </Avatar>
        <button
          type="button"
          class="absolute inset-0 flex items-center justify-center rounded-full bg-black/70 text-white text-xs font-body opacity-0 group-hover:opacity-100 transition-opacity backdrop-blur-xs"
          @click="pickAvatar"
        >
          Changer
        </button>
        <input
          ref="avatarInput"
          type="file"
          accept="image/*"
          class="hidden"
          @change="onAvatarChange"
        />
      </div>

      <div class="flex-1 grid grid-cols-1 sm:grid-cols-2 gap-4">
        <div class="space-y-1">
          <label class="text-xs font-medium text-white/70 font-body flex items-center gap-1.5">
            <User class="w-3.5 h-3.5 text-white/40" /> Nom complet
          </label>
          <input
            v-model="fullName"
            type="text"
            class="w-full rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] font-body"
            placeholder="Prénom et nom"
          />
        </div>

        <div class="space-y-1">
          <label class="text-xs font-medium text-white/70 font-body flex items-center gap-1.5">
            <Briefcase class="w-3.5 h-3.5 text-white/40" /> Poste / style recherché
          </label>
          <input
            v-model="headline"
            type="text"
            class="w-full rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] font-body"
            placeholder="Ex. Développeur full stack"
          />
        </div>
      </div>
    </div>

    <!-- Coordonnées & Localisation -->
    <div class="grid grid-cols-1 sm:grid-cols-3 gap-4">
      <div class="space-y-1">
        <label class="text-xs font-medium text-white/70 font-body flex items-center gap-1.5">
          <MapPin class="w-3.5 h-3.5 text-white/40" /> Localisation
        </label>
        <input
          v-model="location"
          type="text"
          class="w-full rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] font-body"
          placeholder="Ex. Paris, 75"
        />
      </div>

      <div class="space-y-1">
        <label class="text-xs font-medium text-white/70 font-body flex items-center gap-1.5">
          <Mail class="w-3.5 h-3.5 text-white/40" /> Adresse email
        </label>
        <input
          v-model="email"
          type="email"
          class="w-full rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] font-body"
          placeholder="contact@exemple.com"
        />
      </div>

      <div class="space-y-1">
        <label class="text-xs font-medium text-white/70 font-body flex items-center gap-1.5">
          <Phone class="w-3.5 h-3.5 text-white/40" /> Téléphone
        </label>
        <input
          v-model="phone"
          type="tel"
          class="w-full rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] font-body"
          placeholder="+33 6 00 00 00 00"
        />
      </div>
    </div>

    <!-- Bio / À propos -->
    <div class="space-y-1">
      <label class="text-xs font-medium text-white/70 font-body flex items-center gap-1.5">
        <FileText class="w-3.5 h-3.5 text-white/40" /> À propos (Parcours & Motivations)
      </label>
      <textarea
        v-model="bio"
        rows="4"
        class="w-full rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2.5 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] resize-none font-body leading-relaxed"
        placeholder="Présentez votre parcours, vos réalisations et ce qui vous anime..."
      />
    </div>

    <!-- Compétences -->
    <div class="space-y-2">
      <label class="text-xs font-medium text-white/70 font-body">Compétences clés déclarées</label>
      <div class="flex flex-wrap gap-2">
        <Badge
          v-for="skill in skills"
          :key="skill"
          variant="pill"
          class="gap-1.5 pr-2 bg-[#1a1a24] border-white/10 text-white hover:border-[#1a6fff]"
        >
          {{ skill }}
          <button
            type="button"
            class="hover:text-red-400 transition-colors"
            @click="removeSkill(skill)"
            title="Supprimer cette compétence"
          >
            <X class="w-3 h-3" />
          </button>
        </Badge>
      </div>

      <div class="flex gap-2 pt-1">
        <input
          v-model="newSkill"
          type="text"
          class="flex-1 rounded-md bg-[#1a1a24] border border-white/10 px-3 py-2 text-sm text-white placeholder:text-white/30 outline-none focus:border-[#1a6fff] font-body"
          placeholder="Ajouter une compétence (ex: React, SQL, Gestion de projet...)"
          @keydown.enter.prevent="addSkill"
        />
        <Button type="button" variant="secondary" size="icon" class="bg-[#1a1a24] hover:bg-white/10" @click="addSkill">
          <Plus class="w-4 h-4 text-white" />
        </Button>
      </div>
    </div>

    <!-- Bouton Enregistrer -->
    <div class="flex justify-end pt-2">
      <Button
        type="button"
        class="bg-[#1a6fff] hover:bg-[#155bd6] text-white font-medium"
        @click="handleSave"
      >
        Enregistrer les modifications
      </Button>
    </div>
  </div>
</template>
