;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Name: isr_subs.s				;
;Purpose: Maintains a group of interrupt	;
; service routines to replace the standard O/S 	;
; calls 					;
;						;
;Author: Tyrone Lagore				;
;Version: March 28, 2014			;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


		xdef _vbl_isr,_input_isr
		xref _do_VBL,_do_get_input

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Name: vbl_isr					;
;Purpose: Replaces the standard vertical blank	;
; ISR invoked upon a vertical blank IRQ by the	;
; screen display hardware.			;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_vbl_isr:
		link 		a6,#0
		movem.l		d0-2/a0-2,-(sp)

		jsr		_do_VBL

		movem.l		(sp)+,d0-2/a0-2
		unlk		a6
		rte


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;						;
;						;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_input_isr:
		link		a6,#0
		movem.l		d0-2/a0-2,-(sp)

	

		movem.l		(sp)+,d0-2/a0-2
		unlk		a6
