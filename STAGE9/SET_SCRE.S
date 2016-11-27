		xdef		_set_screen

VBASE		equ		8
BFFR_ADDRESS	equ		12


_set_screen:	link		a6,#0
		movem.l		a0-2/d0-2,-(sp)
		move.l		BFFR_ADDRESS(a6),d0
		move.l		VBASE(a6),a0

		lsr.l		#8,d0
		movep.w		d0,0(a0)
		
		movem.l		(sp)+,a0-2/d0-2
		unlk		a6

		rts
