<script setup lang="ts">
import { ref } from 'vue'
import { marked } from 'marked'
import {
  FileText,
  ShieldCheck,
  Eye,
  SlidersHorizontal,
  Server,
  Calendar,
  CheckCircle2,
} from 'lucide-vue-next'
import type { LegalDocumentEntity, LegalSectionId } from '@/core/domain/entities/LegalDocument.entity'
import { Tabs, TabsList, TabsTrigger, TabsContent } from '@/components/ui/tabs'
import { Card, CardHeader, CardTitle, CardDescription, CardContent } from '@/components/ui/card'
import { Badge } from '@/components/ui/badge'

const props = defineProps<{
  documents: LegalDocumentEntity[]
  defaultTab?: LegalSectionId
}>()

const activeTab = ref<string>(props.defaultTab ?? 'cgu')

marked.setOptions({
  gfm: true,
  breaks: true,
})

function renderMarkdown(content: string): string {
  return marked.parse(content) as string
}

const iconMap = {
  FileText,
  ShieldCheck,
  Eye,
  SlidersHorizontal,
  Server,
}

function getIcon(name: string) {
  return iconMap[name as keyof typeof iconMap] ?? FileText
}
</script>

<template>
  <div class="w-full max-w-5xl mx-auto space-y-6">
    <Tabs v-model="activeTab" class="w-full">
      <!-- Onglets de sélection des textes juridiques -->
      <TabsList class="grid grid-cols-2 sm:grid-cols-3 md:grid-cols-5 w-full h-auto p-1.5 bg-card border border-border rounded-xl gap-1">
        <TabsTrigger
          v-for="doc in documents"
          :key="doc.id"
          :value="doc.id"
          class="flex items-center justify-center sm:justify-start gap-2 py-2.5 px-3 text-xs md:text-sm font-semibold font-heading rounded-lg data-[state=active]:bg-[#000091] data-[state=active]:text-white transition-all cursor-pointer"
        >
          <component :is="getIcon(doc.icon)" class="w-4 h-4 shrink-0" />
          <span class="truncate">{{ doc.shortTitle }}</span>
        </TabsTrigger>
      </TabsList>

      <TabsContent
        v-for="doc in documents"
        :key="doc.id"
        :value="doc.id"
        class="mt-4 space-y-4"
      >
        <Card class="border-border bg-card shadow-lg overflow-hidden">
          <CardHeader class="border-b border-border/50 pb-4 bg-muted/10">
            <div class="flex flex-wrap items-center justify-between gap-3">
              <div class="flex items-center gap-3">
                <div class="w-10 h-10 rounded-xl bg-[#000091]/10 text-[#000091] dark:text-[#90b8ff] flex items-center justify-center font-bold">
                  <component :is="getIcon(doc.icon)" class="w-5 h-5" />
                </div>
                <div>
                  <CardTitle class="text-lg sm:text-xl md:text-2xl font-bold font-heading tracking-tight text-foreground">
                    {{ doc.title }}
                  </CardTitle>
                  <CardDescription class="text-xs text-muted-foreground mt-0.5 flex items-center gap-2 font-heading">
                    <Calendar class="w-3.5 h-3.5" />
                    <span>Dernière mise à jour : {{ doc.lastUpdated }}</span>
                  </CardDescription>
                </div>
              </div>

              <Badge v-if="doc.badgeText" variant="secondary" class="gap-1 font-semibold font-heading text-xs py-1 px-3">
                <CheckCircle2 class="w-3.5 h-3.5 text-emerald-500" />
                <span>{{ doc.badgeText }}</span>
              </Badge>
            </div>

            <div class="mt-3 p-3 rounded-lg bg-muted/40 border border-border text-xs md:text-sm text-foreground/90 font-body">
              <span class="font-bold text-[#000091] dark:text-[#90b8ff] mr-1 font-heading">Synthèse :</span>
              {{ doc.summary }}
            </div>
          </CardHeader>

          <!-- Rendu Markdown stylisé avec typographie institutionnelle -->
          <CardContent class="pt-6 font-body">
            <div
              class="markdown-content space-y-4 text-foreground/90 leading-relaxed"
              v-html="renderMarkdown(doc.contentMarkdown)"
            />
          </CardContent>
        </Card>
      </TabsContent>
    </Tabs>
  </div>
</template>

<style scoped>
:deep(.markdown-content) {
  font-family: 'Spectral', serif;
  font-size: 0.95rem;
  line-height: 1.75;
  color: var(--foreground, #ffffff);
}

:deep(.markdown-content h1) {
  font-family: 'Marianne', sans-serif;
  font-size: 1.5rem;
  font-weight: 800;
  color: var(--foreground, #ffffff);
  margin-top: 1.5rem;
  margin-bottom: 0.75rem;
  padding-bottom: 0.5rem;
  border-bottom: 1px solid var(--border, rgba(255, 255, 255, 0.15));
}

:deep(.markdown-content h2) {
  font-family: 'Marianne', sans-serif;
  font-size: 1.25rem;
  font-weight: 700;
  color: var(--foreground, #ffffff);
  margin-top: 1.5rem;
  margin-bottom: 0.5rem;
}

:deep(.markdown-content h3) {
  font-family: 'Marianne', sans-serif;
  font-size: 1.1rem;
  font-weight: 600;
  color: var(--foreground, #ffffff);
  margin-top: 1.25rem;
  margin-bottom: 0.35rem;
}

:deep(.markdown-content p) {
  margin-bottom: 1rem;
}

:deep(.markdown-content ul) {
  list-style-type: disc;
  padding-left: 1.5rem;
  margin-bottom: 1rem;
  space-y: 0.35rem;
}

:deep(.markdown-content ol) {
  list-style-type: decimal;
  padding-left: 1.5rem;
  margin-bottom: 1rem;
  space-y: 0.35rem;
}

:deep(.markdown-content li) {
  margin-bottom: 0.25rem;
}

:deep(.markdown-content blockquote) {
  border-left: 3px solid #000091;
  background: rgba(0, 0, 145, 0.08);
  padding: 0.75rem 1rem;
  margin: 1rem 0;
  border-radius: 0 0.5rem 0.5rem 0;
  font-style: italic;
}

:deep(.markdown-content hr) {
  border: 0;
  border-top: 1px solid var(--border, rgba(255, 255, 255, 0.15));
  margin: 2rem 0;
}

:deep(.markdown-content strong) {
  font-weight: 700;
  color: var(--foreground, #ffffff);
}

:deep(.markdown-content code) {
  background: rgba(255, 255, 255, 0.1);
  padding: 0.15rem 0.4rem;
  border-radius: 0.25rem;
  font-family: monospace;
  font-size: 0.85em;
  color: #f5c842;
}

:deep(.markdown-content table) {
  width: 100%;
  border-collapse: collapse;
  margin: 1.25rem 0;
  font-size: 0.85rem;
  border-radius: 0.5rem;
  overflow: hidden;
  border: 1px solid var(--border, rgba(255, 255, 255, 0.15));
}

:deep(.markdown-content thead) {
  background: rgba(255, 255, 255, 0.08);
  font-family: 'Marianne', sans-serif;
  font-weight: 700;
}

:deep(.markdown-content th),
:deep(.markdown-content td) {
  border: 1px solid var(--border, rgba(255, 255, 255, 0.15));
  padding: 0.6rem 0.75rem;
  text-align: left;
}

:deep(.markdown-content tr:nth-child(even)) {
  background: rgba(255, 255, 255, 0.02);
}
</style>
