IPL_CLEAR_MASK	equ	$F8FF
IPL_ISOLATE	equ	$0700
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Name: serial.s				;
;Purpose: Maintains a group of interrupt	;
; service routines to interface with the USART
;						;
;Author: Tyrone Lagore				;
;Version: April 10, 2014			;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


		xdef _tx_isr,_rx_isr
		xref _tx_empty_isr,_rx_full_isr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Name: tx_isr					;
;Purpose: Replaces the standard transmit register;
; emtpy ISR.					;
;		;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_tx_isr:
		movem.l		d0-2/a0-2,-(sp)

		jsr			_rx_full_isr

		movem.l		(sp)+,d0-2/a0-2
		rte


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Name: rx_isr			;
;Purpose: Replaces the standard transmit register
;full ISR.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_rx_isr:
		movem.l		d0-2/a0-2,-(sp)

		jsr			_tx_empty_isr

		movem.l		(sp)+,d0-2/a0-2
		rte