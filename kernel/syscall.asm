
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               syscall.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

%include "sconst.inc"

_NR_get_ticks       equ 0 ; 要跟 global.c 中 sys_call_table 的定义相对应！
_NP_my_disp_str		equ 1
_NR_my_delay		equ	2
_NR_semaphore_p		equ 3
_NR_semaphore_v		equ 4

INT_VECTOR_SYS_CALL equ 0x90

; 导出符号 这里就是给外面可以用的了
global	get_ticks
global	my_disp_str
global	my_delay
global	semaphore_p
global	semaphore_v

bits 32
[section .text]

; ====================================================================
;                              get_ticks
; ====================================================================
get_ticks:
	mov	eax, _NR_get_ticks	;因为无参数所以只需要eax，其他的需要ebx,ecx,edx
	int	INT_VECTOR_SYS_CALL
	ret
; ====================================================================
;                              my_disp_str
; ====================================================================
my_disp_str:
	mov eax, _NP_my_disp_str
	mov ebx, [esp+4]
	mov ecx, [esp+8]
	int INT_VECTOR_SYS_CALL
	ret
; ====================================================================
;                              process_delay
; ====================================================================
my_delay:
	mov	eax, _NR_my_delay
	mov	ebx, [esp+4]
	mov	ecx, [esp+8]
	int	INT_VECTOR_SYS_CALL
	ret
; ====================================================================
;                            semaphore_p
; ====================================================================
semaphore_p:
	mov	eax, _NR_semaphore_p
	mov	ebx, [esp+4]
	int	INT_VECTOR_SYS_CALL
	ret
; ====================================================================
;                            semaphore_v
; ====================================================================
semaphore_v:
	mov	eax, _NR_semaphore_v
	mov	ebx, [esp+4]
	int	INT_VECTOR_SYS_CALL
	ret