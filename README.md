# test
Mensaje.asm
```c
; Programa para imprimir un mensaje en la pantalla en Linux

section .data
    mensaje db '¡Hola, mundo!',0xd,0xa   ; mensaje a imprimir

section .text
    global _start   ; punto de entrada del programa

_start:
    ; escribe el mensaje en la salida estándar
    mov eax, 4      ; función del sistema para escribir en pantalla
    mov ebx, 1      ; salida estándar (STDOUT)
    mov ecx, mensaje ; dirección del mensaje
    mov edx, 15     ; tamaño del mensaje
    int 0x80        ; llama al sistema para escribir en pantalla

    ; termina el programa
    mov eax, 1      ; función del sistema para salir del programa
    xor ebx, ebx    ; código de salida (0 = sin errores)
    int 0x80        ; llama al sistema para salir del programa

```
### Conteo.asm
```c
section .data; Declarar datos o constantes inicializados
    msg db "Número: "
    ent db 0x0a
    num db 0
   
section .bss; declarar variables
    numASCII resb '0'

section .text; comienza el global start el kernel lee el inicio del programa
    global _start
_start:
    
;Bucle para contar del 0 al 9
conteo:
        ;Se muestra el mensaje "Número "
        mov eax, 4
        mov ebx, 1
        mov ecx, msg
        mov edx, 9
        int 0x80
        
        ;Transforma valor numérico a ASCII
	mov eax,[num]
	add eax,'0'
	mov [numASCII],eax
        
        ;Muestra número
        mov eax, 4
        mov ebx, 1
        mov ecx, numASCII
        mov edx, 1
        int 0x80
        
        ;Fin de línea
        mov eax, 4
        mov ebx, 1
        mov ecx, ent
        mov edx, 1
        int 0x80        
        
        ;Salta a "fin" si se llegó a 9
        cmp byte [num], 9
        je fin
        
        ;Se incrementa el contador y se salta a "conteo"
        inc byte [num]
        jmp conteo
    
;Etiqueta de fin
fin:
        ;Sale del programa  
        mov eax, 1
        xor ebx, ebx
        int 0x80
```
### Condicional.asm
```c
section .data
    num db 10           ;para verificar
    msg1 db 'Mayor', 0x0A ;si el número a verificar es mayor
    msg2 db 'Menor/igual', 0x0A ;sie el número a verificar es menor

section .text
global _start

_start:
    mov al, [num]        ; carga en el registro AL
    cmp al, 20           ; compara AL con 20
    jg  mayor            ; salta si es mayor
 
menor:
    mov eax, 4           ; función de LINUX para escribir en pantalla
    mov ebx, 1           ; STDOUT
    mov ecx, msg2        ; dirección de msg2
    mov edx, 12          ; tamaño del mensaje
    int 0x80             ; LINUX escribe el mensaje en pantalla
    jmp fin              ; salta incondicionalmente a "fin"

mayor:
    mov eax, 4           ; función de LINUX para escribir en pantalla
    mov ebx, 1           ; STDOUT
    mov ecx, msg1        ; dirección de msg1
    mov edx, 6           ; tamaño del mensaje
    int 0x80             ; LINUX escribe el mensaje en pantalla
    
fin:
    mov eax, 1           ; función de LINUX para terminar el programa
    xor ebx, ebx         ; 0 = sin errores
    int 0x80             ; LINUX termina el programa


```
```c
//-------------------------
// Subrutinas para llamada
//-------------------------
extern "C"
{
  void inicio();
  void led(byte);
}
//----------------------------------------------------
void setup()
{
  inicio();
}
//----------------------------------------------------
void loop()
{
  led(1);
  
}

```
```c
;---------------
; Código en Assembly
;---------------
#define __SFR_OFFSET 0x00
#include "avr/io.h"
;------------------------
.global inicio
.global led

inicio:
    sbi   DDRB, 5             ;ajusta PB4 (D12) como salida
    ret                       ;regresa a setup()
;---------------------------------------------------------------------------
led:
    cpi   R24, 0x00           ;valor en R24 pasado desde la llamada se compara con 0
    breq  ledOFF              ;salta (jump, branch) si es igual, a la subrutina ledOFF
    sbi   PORTB, 5            ;pone D12 a 1L
    nop
    nop
    nop
    nop
    nop
    nop
    ;rcall retardo             ;retardo
    ret                       ;regresa a loop()
;---------------------------------------------------------------------------
ledOFF:
    cbi   PORTB, 5            ;pone D12 a 0L
   
    ;rcall retardo
    
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
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

```
