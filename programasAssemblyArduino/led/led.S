;---------------
; Código en Assembly
;---------------
#define __SFR_OFFSET 0x00
#include "avr/io.h"
;------------------------
.global inicio
.global led

inicio:
    sbi   DDRB, 4             ;ajusta PB4 (D12) como salida
    ret                       ;regresa a setup()
;---------------------------------------------------------------------------
led:
    cpi   R24, 0x00           ;valor en R24 pasado desde la llamada se compara con 0
    breq  ledOFF              ;salta (jump, branch) si es igual, a la subrutina ledOFF
    sbi   PORTB, 4            ;pone D12 a 1L
    rcall retardo             ;retardo
    ret                       ;regresa a loop()
;---------------------------------------------------------------------------
ledOFF:
    cbi   PORTB, 4            ;pone D12 a 0L
    rcall retardo
    ret                       ;regresa a loop()
;---------------------------------------------------------------------------
.equ  delayVal, 10000         ;valor inicial del contador para bucleInterno
;---------------------------------------------------------------------------
retardo:
    ldi   R20, 100            ;valor inicial de conteo para bucleExterno
bucleExterno:
    ldi   R30, lo8(delayVal)   ;low byte de delayVal en R30
    ldi   R31, hi8(delayVal)  ;high byte de delayVal en R31
bucleInterno:
    sbiw  R30, 1              ;resta 1 del valor de 16-bit en R31, R30
    brne  bucleInterno            ;salta si countVal no es igual a 0
    ;--------------
    subi  R20, 1              ;resta 1 de R20
    brne  bucleExterno            ;salta si R20 no es igual a 0
    ret
;---------------------------------------------------------------------------