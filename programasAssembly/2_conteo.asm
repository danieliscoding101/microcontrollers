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