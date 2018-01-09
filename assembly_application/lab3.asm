 .equ __P24FJ64GA002,1
        .include "p24Fxxxx.inc"

#include "xc.inc"

        config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
        config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & COE_ON & BKBUG_ON & GWRP_ON & GCP_ON & JTAGEN_OFF

	.global Xstart
	
	.bss	
	final: .space 2*10
	Xstart: .space 2*3
	A:	.space 2*3
	counter: .space 1
 
	
	   
    .text
    .global _main



    

_main:
  mov #0x9fff, w0
    mov w0, AD1PCFG	   ;sets all AN pins as digital
    mov #3, w0
    mov w0, TRISB	   ;sets RB2-9 as digital outputs
    bset TRISA, #0	   ;sets RA0 to input with pull-up resistor
    bset CNPU1, #2
    mov #0xfffc, w0	   ;sets LEDs initially off
    mov w0, LATB
    
   
    mov	   #final, w6		;sets final number to register 6
    mov	   #Xstart, w14	;sets the X0 variable to its correct initial value of 0x1330E
    mov	   #0x330e, w0 
    mov	   w0, [w14++]
    mov	   #0x0001, w0
    mov	   w0, [w14--]		
    	   
    
    mov	   #0,	w0
    mov	   w0, counter		;sets the first button press counter
waitButton:
   BTSC PORTA, #0
   bra waitButton
   mov #275, w0
   bra debounce				;goes to first button press
debounce:				;delay for debounce of the button
	repeat #996
	nop
	dec WREG0
	bra nz, debounce		;sets to zero after first button press
	mov counter, w0				;if first button press
	BTSC w0, #0
	bra nextseed                    ;any other button press			
firstseed:		   ;SetValuesNet
    inc counter
    mov #0x0005, w2	   ;Set "a" value in w2, w1, w0
    mov #0xdeec, w1
    mov #0xe66d, w0
    mov [w14++], w3	   ;Set X0 to w3, w4, w5. 
    mov [w14++], w4
    mov [w14], w5
    bra multiply            
nextseed:
    mov #0x0005, w2	   ;Set A value in w2, w1, w0
    mov #0xdeec, w1
    mov #0xe66d, w0
    mov w6, w3		   ;Moves last x value into (w6, w7, w8) for next 
    mov w7, w4              ;multiplication
    mov w8, w5
multiply:
    MUL.UU w0, w3, w12	   ; multiplies A1 * B1
    mov w12, w6		   ; moves value into w6
    mov w13, w7		   
    MUL.UU w1, w3, w12	   ;multiplies B1 * A2
    ADD w12, w7, w7
    ADDC w13, #0, w8	   ;adds carry to value
    MUL.UU w3, w2, w12	   ;multiplies B1 * A3, but results in zero first seed
    ADD w12, w8, w8
    ADDC w13, #0, w9	   ;adds carry
    MUL.UU w0, w4, w12	   ;multiplies A1 * B2
    ADD w12, w7, w7
    ADDC w13, w8, w8	   ;adds carry
    ADDC w9, #0, w9	   ;adds carry to value multiplied
    MUL.UU w1, w4, w12	   ;multiplies A2 * B2
    ADD w12, w8, w8         ;adds carry
    ADDC w9, w13, w9
    ADDC w10,#0 ,w10	   ;adds carry to value
    MUL.UU w4, w2, w12	   ;multiplies A3 * B2
    ADD w12, w9, w9	   
    ADDC w13, w10, w10
    ADDC w11, #0, w11	   ;adds carry to value
    MUL.UU w5, w0, w12	   ;multiplies A1 * B3
    ADD w12, w8, w8
    ADDC w13, w9, w9
    ADDC w10, #0, w10
    ADDC w11, #0, w11	   ;adds carry to value
    MUL.UU w5, w1, w12	   ;multiplies A2 * B3
    ADD w12, w9, w9
    ADDC w13, w10, w10
    ADDC w11,#0, w11	   ;adds carry to value
    MUL.UU w5, w2, w12	   ;multiplies A3 * B3
    ADD w12, w10, w10
    ADDC w13, w11, w11	   ;adds carry to value
add:
    ADD w6, #11, w6		;Adds c = 11 to a * Xn value calculated in multiply
    ADDC w7, #0, w7
    ADDC w8, #0, w8
    ADDC w9, #0, w9
    ADDC w10, #0, w10
    ADDC w11, #0, w11
mod48:
    MOV w6, w0			;puts least significant 8 bits in w0
    SL w0, #2, w0		;shifts bits to put in RB2 first, not RB0
setLEDs:
    mov w0, LATB		;sets bit values to LEDs
    bra waitButton		;goes back to waiting for the button
    
    
    
    .end
