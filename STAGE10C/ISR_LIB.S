IPL_CLEAR_MASK	equ	$F8FF
IPL_ISOLATE	equ	$0700
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Name: isr_subs.s				;
;Purpose: Maintains a group of interrupt	;
; service routines to replace the standard O/S 	;
; calls 					;
;						;
;Author: Tyrone Lagore				;
;Version: March 28, 2014			;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


		xdef _vbl_isr,_input_isr,_mask_ipl
		xref _do_VBL,_do_get_input

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Name: vbl_isr					;
;Purpose: Replaces the standard vertical blank	;
; ISR invoked upon a vertical blank IRQ by the	;
; screen display hardware.			;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_vbl_isr:
		movem.l		d0-2/a0-2,-(sp)

		jsr			_do_VBL

		movem.l		(sp)+,d0-2/a0-2
		rte


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;						;
;						;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_input_isr:
		movem.l		d0-2/a0-2,-(sp)

		jsr			_do_get_input

		movem.l		(sp)+,d0-2/a0-2
		rte
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;int mask_ipl(int mask)				;
;						;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MASK		equ		8

_mask_ipl:
		link		a6,#0
		move.w		SR,d0			;get current status reg

		movem.l		d1-2/a0-2,-(sp)
		
		move.w		d0,d1
		move.w		d0,d2

		andi.w		#IPL_ISOLATE,d0		;isolate IPL mask
		lsr.w		#8,d0				;keep it as return

		andi.w		#IPL_CLEAR_MASK,d2	;clear IPL bits

		move.w		MASK(a6),d1		;get the mask
		lsl.w		#8,d1			;shift it to IPL pos

		or.w		d1,d2			;or in new mask
		move.w		d2,SR			;set as status reg

		movem.l		(sp)+,d1-2/a0-2
		unlk		a6
		rts
