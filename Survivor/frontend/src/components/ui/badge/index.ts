import { type VariantProps, cva } from 'class-variance-authority'

export { default as Badge } from './Badge.vue'

export const badgeVariants = cva(
  'inline-flex items-center rounded-full border px-2.5 py-0.5 text-xs font-semibold font-heading transition-colors focus:outline-none focus:ring-2 focus:ring-ring focus:ring-offset-2',
  {
    variants: {
      variant: {
        default:
          'border-transparent bg-primary text-primary-foreground shadow hover:bg-primary/80',
        secondary:
          'border-transparent bg-secondary text-secondary-foreground hover:bg-secondary/80',
        destructive:
          'border-transparent bg-destructive text-destructive-foreground shadow hover:bg-destructive/80',
        outline: 'text-foreground',
        tech: 'border-[#1B3A6B]/60 bg-[#1B3A6B]/25 text-[#90b8ff] hover:bg-[#1B3A6B]/35',
        jeb: 'border-[rgba(245,200,66,0.45)] bg-gradient-to-r from-[rgba(245,200,66,0.18)] to-[rgba(197,148,26,0.12)] text-[#f5c842] font-bold',
        pill: 'border-white/10 bg-[#1a1a1c] text-white/80 hover:bg-white/10 px-3.5 py-1.5 text-xs',
      },
    },
    defaultVariants: {
      variant: 'default',
    },
  },
)

export type BadgeVariants = VariantProps<typeof badgeVariants>
