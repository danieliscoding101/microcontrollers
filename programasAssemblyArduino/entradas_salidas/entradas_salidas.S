; Definición de constantes tomadas desde m328Pdef.inc

#define __SFR_OFFSET 0x00
#include "avr/io.h"

.global ajustes
.global verificaBotones

;--------------------------------
ajustes:
;--------------------------------
  ; DDR* configuran el puerto como entrada/salida
  ; DDRB está mapeado sobre pines 8-13.
  ; pines 1L = salida
    sbi DDRB, 0 ;salida
    sbi DDRB, 1 ;salida
    sbi DDRB, 2 ;salida

  ; DDRD está mapeado sobre pines 0-7
  ; pines 0L = entrada
    cbi DDRD, 2 ;entrada
    sbi PORTD, 2 ;pullup activada

    cbi DDRD, 3 ;entrada
    sbi PORTD, 3 ;pullup activada

    cbi DDRD, 4 ;entrada
    sbi PORTD, 4 ;pullup activada

    ret ;retorna desde la llamada

;--------------------------------
verificaBotones:
;--------------------------------
; Verifica si estados de entradas
    sbic PIND, 2
    rjmp apagaB0
    rcall enciendeB0
finB0:
    sbic PIND, 3
    rjmp apagaB1
    rcall enciendeB1
finB1:
    sbic PIND, 4
    rjmp apagaB2
    rcall enciendeB2
finB2:
    ret

; Se usa PORT* para escribir en un pin configurado como salida
; Al igual que con DDR*:
; PORTB mapeado sobre pines 8-13 (igual que DDRB)

enciendeB0:
    sbi PORTB, 0
    ret
enciendeB1:
    sbi PORTB, 1
    ret
enciendeB2:
    sbi PORTB, 2
    ret

apagaB0:
    cbi PORTB, 0
    rjmp finB0
apagaB1:
    cbi PORTB, 1
    rjmp finB1
apagaB2:
    cbi PORTB, 2
    rjmp finB2

