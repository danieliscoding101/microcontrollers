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

