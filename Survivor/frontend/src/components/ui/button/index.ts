import { type VariantProps, cva } from 'class-variance-authority'

export { default as Button } from './Button.vue'

export const buttonVariants = cva(
  'inline-flex items-center justify-center gap-2 whitespace-nowrap rounded-md text-sm font-semibold font-heading transition-all focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-[#000091] focus-visible:ring-offset-2 disabled:pointer-events-none disabled:opacity-50 [&_svg]:pointer-events-none [&_svg]:size-4 [&_svg]:shrink-0 select-none cursor-pointer',
  {
    variants: {
      variant: {
        default: 'bg-[#000091] text-white hover:bg-[#1212ff] shadow-sm',
        secondaryCta: 'bg-[#D97706] text-[#09090f] font-bold border border-[#B45309] shadow-md hover:bg-[#B45309] hover:text-white',
        destructive: 'bg-destructive text-destructive-foreground shadow-sm hover:bg-destructive/90',
        outline: 'border border-border bg-transparent text-foreground shadow-sm hover:bg-muted/40 hover:text-foreground',
        secondary: 'bg-secondary text-secondary-foreground shadow-sm hover:bg-secondary/80',
        ghost: 'hover:bg-white/10 text-foreground',
        link: 'text-foreground underline-offset-4 hover:underline',
        cyan: 'bg-[#D97706] text-[#09090f] font-bold border border-[#B45309] shadow-md hover:bg-[#B45309] hover:text-white',
        gold: 'bg-gradient-to-r from-[#f5c842] to-[#c5941a] text-[#09090f] font-bold shadow-md hover:opacity-90',
      },
      size: {
        default: 'h-9 px-4 py-2',
        sm: 'h-8 rounded-md px-3 text-xs',
        lg: 'h-10 rounded-md px-8',
        xl: 'h-12 rounded-xl px-6 text-base',
        icon: 'h-9 w-9',
        iconLg: 'h-12 w-12 rounded-full',
      },
    },
    defaultVariants: {
      variant: 'default',
      size: 'default',
    },
  },
)

export type ButtonVariants = VariantProps<typeof buttonVariants>
