[BITS 32]
global kReadCPUID, kSwitchAndExecute64bitKernel
section .text

; return CPUID
; PARAM: DWORD dwEAX, DWORD *pdwEAX, *pdwEBX, *pdwECX, *pdwEDX
kReadCPUID:
	push ebp
	mov ebp, esp
	push eax
	push ebx
	push ecx
	push edx
	push esi

	mov eax, dword [ebp + 8] ; dwEAX
	cpuid

	mov esi, dword [ebp + 12]
	mov dword [ esi ], eax

	mov esi, dword [ebp + 16]
	mov dword [ esi ], ebx

	mov esi, dword [ebp + 20]
	mov dword [ esi ], ecx

	mov esi, dword [ebp + 24]
	mov dword [ esi ], edx
	
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret

; switch IA32e mode
; PARAM: None
kSwitchAndExecute64bitKernel:
	mov eax, cr4
	or eax, 0x20		; PAE bit setting from cr4
	mov cr4, eax

	mov eax, 0x100000	; PML4 table setting from cr3
	mov cr3, eax

	mov ecx, 0xC0000080	; IA32_EFER address
	rdmsr
	or eax, 0x100		; LME bit setting from IA32_EFER
	wrmsr

	mov eax, cr0
	or eax, 0xE0000000
	xor eax, 0x60000000
	mov cr0, eax		; NW bit, CD bit unsetting, PG bit setting from cr0

	jmp 0x08:0x200000
	jmp $


