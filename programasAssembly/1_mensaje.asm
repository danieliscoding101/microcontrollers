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
