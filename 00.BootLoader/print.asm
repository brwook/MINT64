[ORG 0x00]
[BITS 16]
section .text

jmp 0x07C0:_start
_start:
	mov ax, 0x07C0
	mov ds, ax

	mov ax, 0xB800
	mov es, ax

	mov si, 0

cleanScreen:
	mov byte [ es: si ], 0
	mov byte [ es: si + 1 ], 0x0c
	add si, 2
	cmp si, 80 * 25 * 2
	jl cleanScreen

writeScreen:
	mov si, 0
	mov di, 0

writeScreenLoop:
	mov cl, byte [ si + msg1 ]
	cmp cl, 0
	je _loop
	
	mov byte [ es: di ], cl
	add si, 1
	add di, 2
	jmp writeScreenLoop

_loop:
	jmp $

msg1: db "Brwook OS Boot Loader...", 0
times 510 - ($ - $$) db 0x00
db 0x55, 0xAA

