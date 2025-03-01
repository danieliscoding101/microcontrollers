    tope equ 7 ;número de elementos de la serie
section .data
    serie db 0, 0, 0, 0, 0, 0, 0, 0 ;almacena los valores de la serie
    contador db 0

section .bss
    a resb '0'

section .text
    global _start

_start:
    mov ecx, tope    ;carga tope del contador
    mov esi, serie      ;puntero al array serie

    ; Inicializa los dos primeros elementos de la serie   
    mov byte [esi], 0  
    inc byte [contador]
    mov byte [esi + 1], 1
    inc byte [contador]
    
numFib:
    mov ecx, [contador]

    mov byte al, [esi + ecx - 1]  ;carga el último elemento de la serie en AL
    add byte al, [esi + ecx - 2]  ;suma el penúltimo elemento de la serie
    mov byte [esi + ecx], al  ;almacena el resultado en la siguiente posición del array
    
    inc byte [contador]
    mov al, [contador]
    mov bl, tope
    cmp al,bl
    jne numFib

    mov byte [contador], 0  ;reinicia contador
 
 numero2ASCII:
   ;Cambia los números a ASCII y guarda en serie
    mov esi, serie          ;puntero al array serie
    mov ecx, [contador]

	add esi, ecx    ;incrementa puntero

	mov eax, [esi]
	add eax, '0'
	mov [esi],eax
    
    inc byte [contador]
    mov al, [contador]
    mov bl, tope
    cmp al,bl
    jne numero2ASCII

    ;Imprime los elementos de la serie en la consola
    mov eax, 4
    mov ebx, 1
    mov ecx, serie
    mov edx, tope
    int 0x80

    ;Fin del programa
    mov eax, 1
    xor ebx, ebx
    int 0x80