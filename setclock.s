;  *********************************************
;  File:        setclock.s
;  Title:       setup for external XTAL clock
;  Purpose:     LM3S9D96-DK
;  Business:    HSA Elektrotechnik
;  Compiler:    MDK-ARM
;  Author/Date: Franz Haunstetter / 06.09.13
;  Comment:     new
;  *********************************************

SYSCTL_RCC_R          EQU 0x400FE060  ; Main run-mode clock config register
SYSCTL_RCC_SYSDIV_M   EQU 0x07800000  ; System Clock Divisor
SYSCTL_RCC_USESYSDIV  EQU 0x00400000  ; Enable System Clock Divider
SYSCTL_RCC_SYSDIV_S   EQU 23          ; System Clock Divider shift
SYSCTL_RCC_PWRDN      EQU 0x00002000  ; PLL Power Down
SYSCTL_RCC_BYPASS     EQU 0x00000800  ; PLL Bypass
SYSCTL_RCC_XTAL_M     EQU 0x000007C0  ; Crystal Value
SYSCTL_RCC_XTAL_16MHZ EQU 0x00000540  ; 16 MHz
SYSCTL_RCC_OSCSRC_M   EQU 0x00000030  ; Oscillator Source
SYSCTL_RCC_OSCSRC_MAIN EQU 0x00000000 ; MOSC
SYSCTL_RCC_IOSCDIS    EQU 0x00000002  ; Internal Oscillator Disable
SYSCTL_RCC_MOSCDIS    EQU 0x00000001  ; Main Oscillator Disable

SYSCTL_RCC2_R         EQU 0x400FE070  ; 2nd run-mode clock config register
SYSCTL_RCC2_USERCC2   EQU 0x80000000  ; Use RCC2
SYSCTL_RCC2_DIV400    EQU 0x40000000  ; Divide PLL as 400 MHz instead of 200
SYSCTL_RCC2_SYSDIV2_M EQU 0x1F800000  ; System Clock Divisor 2
SYSCTL_RCC2_SYSDIV2LSB EQU 0x00400000 ; Additional LSB for SYSDIV2
SYSCTL_RCC2_SYSDIV2_5 EQU 0x02000000  ; System clock /5
SYSCTL_RCC2_SYSDIV2_S EQU 23          ; System Clock Divider shift
SYSCTL_RCC2_PWRDN2    EQU 0x00002000  ; Power-Down PLL 2
SYSCTL_RCC2_BYPASS2   EQU 0x00000800  ; PLL Bypass 2
SYSCTL_RCC2_OSCSRC2_M EQU 0x00000070  ; Oscillator Source 2
SYSCTL_RCC2_OSCSRC2_MO EQU 0x00000000 ; MOSC

;******************************************************************************
;
; Place code into the standard code section.
;
;******************************************************************************
        AREA    CLOCK, CODE, READONLY
        THUMB

        EXPORT  SetClockXTAL
SetClockXTAL
        ;
        ; Sets the external clock source for synchronizing the system clock
		; by setting RCC/RCC2 for PLL 16 MHz -> 400 MHz -> 80 MHz output
        ;
		
        LDR     R0, = SYSCTL_RCC_R
		LDR     R1, [R0]
		AND     R1, #:NOT:(SYSCTL_RCC_XTAL_M)
		AND     R1, #:NOT:(SYSCTL_RCC_MOSCDIS)
		ORR     R1, #SYSCTL_RCC_XTAL_16MHZ
		STR     R1, [R0]
		
        LDR     R0, = SYSCTL_RCC2_R
		LDR     R1, [R0]
		AND     R1, #:NOT:(SYSCTL_RCC2_SYSDIV2_M:OR:SYSCTL_RCC2_SYSDIV2LSB)
		AND     R1, #:NOT:(SYSCTL_RCC2_PWRDN2:OR:SYSCTL_RCC2_BYPASS2)
		AND     R1, #:NOT:(SYSCTL_RCC2_OSCSRC2_M)
        ORR     R1, #SYSCTL_RCC2_USERCC2:OR:SYSCTL_RCC2_DIV400
		ORR     R1, #2:SHL:SYSCTL_RCC2_SYSDIV2_S
		ORR     R1, #SYSCTL_RCC2_OSCSRC2_MO
		STR     R1, [R0]

		LDR		R2, [R0]   ; just to give it some time to settle
		
        BX		LR

;******************************************************************************
;
; Make sure the end of this section is aligned.
;
;******************************************************************************
        ALIGN

;******************************************************************************
;
; Tell the assembler that we're done.
;
;******************************************************************************
        END
