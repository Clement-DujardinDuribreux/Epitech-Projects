<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { useRoute } from 'vue-router'
import { Shield } from 'lucide-vue-next'
import type { LegalDocumentEntity, LegalSectionId } from '@/core/domain/entities/LegalDocument.entity'
import { LegalComplianceService } from '@/core/services/LegalComplianceService'
import { LegalMockRepository } from '@/core/infrastructure/repositories/LegalMockRepository'
import { useDevicePreview } from '@/composables/useDevicePreview'
import DevicePreviewBar from '@/components/common/DevicePreviewBar.vue'
import ComplianceTabViewer from '@/components/legal/ComplianceTabViewer.vue'

const route = useRoute()
const { currentFormat } = useDevicePreview()
const service = new LegalComplianceService(new LegalMockRepository())

const documents = ref<LegalDocumentEntity[]>([])
const initialTab = ref<LegalSectionId>('cgu')

onMounted(async () => {
  documents.value = await service.fetchAllDocuments()
  if (route.query.section) {
    initialTab.value = route.query.section as LegalSectionId
  }
})
</script>

<template>
  <div class="flex-1 w-full flex flex-col items-center font-body py-4">
    <DevicePreviewBar :active-format="currentFormat" @update:format="(f) => (currentFormat = f)" />

    <main
      class="w-full flex-1 p-3 sm:p-6 md:p-8 space-y-6 transition-all duration-300"
      :class="{
        'max-w-[390px]': currentFormat === 'mobile',
        'max-w-[768px]': currentFormat === 'tablet',
        'max-w-6xl': currentFormat === 'desktop',
      }"
    >
      <div class="space-y-1 pb-2">
        <div class="flex items-center gap-2 text-[#000091] dark:text-[#90b8ff] font-bold text-sm font-heading">
          <Shield class="w-4 h-4" />
          <span>Transparence & Souveraineté Juridique</span>
        </div>
      </div>

      <ComplianceTabViewer
        v-if="documents.length > 0"
        :documents="documents"
        :default-tab="initialTab"
      />
    </main>
  </div>
</template>
