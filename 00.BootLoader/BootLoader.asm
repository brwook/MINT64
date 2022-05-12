[ORG 0x00]
[BITS 16]

section .text
jmp 0x07C0:_start
TOTALSECTORCOUNT: dw 2
KERNEL32SECTORCOUNT: dw 2
_start:
	mov ax, 0x07c0
	mov ds, ax
	mov ax, 0xb800
	mov es, ax
	
	mov ax, 0x0000
	mov ss, ax
	mov sp, 0xfffe
	mov bp, 0xfffe
	
	mov si, 0

clearScreenLoop:
	mov byte [ es: si ], 0
	mov byte [ es: si + 1 ], 0x0C
	add si, 2
	cmp si, 80 * 25 * 2
	jl clearScreenLoop
	
	push MSG1
	push 0
	push 0
	call printMsg
	add sp, 6
	
	push IMAGELOADINGMSG
	push 1
	push 0
	call printMsg
	add sp, 6
	
resetDisk:
	mov ax, 0
	mov dl, 0
	int 0x13
	jc handleDiskError
	
	mov si, 0x1000
	mov es, si
	mov bx, 0
	mov di, word [ TOTALSECTORCOUNT ]

readData:
	cmp di, 0
	je readEnd
	sub di, 0x1
	
	mov ah, 0x2
	mov al, 0x1
	mov ch, byte [ TRACKNUMBER ]
	mov cl, byte [ SECTORNUMBER ]
	mov dh, byte [ HEADNUMBER ]
	mov dl, 0x0
	int 0x13
	jc handleDiskError
	
	add si, 0x20
	mov es, si
	
	mov al, byte [ SECTORNUMBER ]
	add al, 0x1
	mov byte [ SECTORNUMBER ], al
	cmp al, 19
	jl readData
	
	xor byte [ HEADNUMBER ], 0x1
	mov byte [ SECTORNUMBER ], 0x1
	
	cmp byte [ HEADNUMBER ], 0x0
	jne readData
	
	add byte [ TRACKNUMBER ], 0x1
	jmp readData

readEnd:
	push LOADINGCOMPLETEMSG
	push 1
	push 20
	call printMsg
	add sp, 6

	jmp 0x1000:0x0000

handleDiskError:
	push DISKERRORMSG
	push 1
	push 20
	call printMsg
	jmp $

printMsg:
	push bp
	mov bp, sp
	
	push es
	push si
	push di
	push ax
	push cx
	push dx
	
	mov ax, 0xb800
	mov es, ax
	
	mov ax, word [ bp + 6 ] ; di = (y * 80 * 2);
	mov si, 160
	mul si
	mov di, ax
	
	mov ax, word [ bp + 4 ] ; di += (x * 2);
	mov si, 2
	mul si
	add di, ax
		
	mov si, word [ bp + 8 ]
	
printMsgLoop:
	mov cl, byte [ si ]
	cmp cl, 0
	je printMsgEnd
	
	mov byte [ es: di ], cl
	add si, 1
	add di, 2
	
	jmp printMsgLoop

printMsgEnd:
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp
	ret

MSG1: 			db 'Brwook OS Boot Loader', 0
DISKERRORMSG:		db 'Disk Error!', 0
IMAGELOADINGMSG:	db 'OS Image Loading...', 0
LOADINGCOMPLETEMSG:	db 'Complete', 0
SECTORNUMBER:		db 0x02
HEADNUMBER:		db 0x00
TRACKNUMBER:		db 0x00

times 510 - ($ - $$)	db 0x00
db 0x55, 0xAA
