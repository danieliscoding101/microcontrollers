maxLineas equ 8
largoMensaje equ 44

section .data
    numEstrellas db 0
    largoLinea db  1

section .bss
mensaje: resb largoMensaje

section .text
    global _start
    
_start:
    mov al, [largoLinea]
    mov bl, [numEstrellas]
    mov ecx, mensaje

linea:
    ;Llena de '*' la línea
    mov byte [ecx],'*' ;coloca un '*'
    inc ecx
    inc bl
    cmp bl,al
    jne linea

lineaTerminada:
    ;Cambia de línea
    mov byte [ecx], 0x0a ;coloca un fin de línea
    inc ecx
    inc al
    mov bl, 0
    cmp al, maxLineas
    jng linea
    
consola:
    ;Muestra mensaje
    mov eax, 4
    mov ebx, 1
    mov ecx, mensaje
    mov edx, largoMensaje
    int 0x80    

fin:
    ;Sale del programa  
    mov eax, 1
    xor ebx, ebx
    int 0x80