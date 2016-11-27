#include <osbind.h>
#include <stdio.h>

#include "system.h"
#include "serial.h"
#include "types.h"

#define TX_EMPTY_VECTOR 74
#define RX_FULL_VECTOR 76

#define ENABLE_TX_RX 0x01
#define CNTRL_STATUS 0x08
#define ENABLE_USART_INTERRUPTS 0x14

#define CLEAR_RX_PENDING 0xEF
#define CLEAR_TX_PENDING 0xFB

#define BUFFER_SIZE 512
 
void rx_isr();
void tx_isr();

void tx_empty_isr();
void rx_full_isr();
 
volatile UINT8 * const TIMER_C_DATA			= 0xFFFFFA23;
volatile UINT8 * const TIMER_D_DATA			= 0xFFFFFA25;

volatile UINT8 * const USART_CNTRL			= 0xFFFFFA29;
volatile UINT8 * const RX_STATUS			= 0xFFFFFA2B;
volatile UINT8 * const TX_STATUS 			= 0xFFFFFA2D;

volatile UINT8 * const ISR_A_IN_SERVICE	  	= 0xFFFFFA0F;

volatile UINT8 * const INTERRUPT_ENABLE_B	= 0xFFFFFA09;
volatile UINT8 * const INTERRUPT_MASK_B		= 0xFFFFFA15;

/* xxxxxxx.
   | | |||________ Parity odd/even
   | | ||_________ Parity enable/disable
   | | |__________ Protocol A
   | |____________ Protocol B
   |______________ Clock divide by 16 off/on

Protocol A							 Protocol B
Value	Stop	Start	Format		 Value	 Data
00		0		0		Synchronous	 00		 8
01		1		1		Asynchronous 01		 7
10		1		1.5		Asynchronous 10		 6
11		1		2		Asynchronous 11		 5
*/
volatile UINT8 * const IRQ_ENABLE_A 	= 0xFFFFFA07;
volatile UINT8 * const IRQ_MASK_A 		= 0xFFFFFA13;


/*
volatile UINT8 * const INTERRUPT_ENABLE_B	= 0xFFFFFA09;
volatile UINT8 * const INTERRUPT_MASK_B		= 0xFFFFFA15;
*/

volatile UINT8 * const USART_DATA			= 0xFFFFFA2F;


char output_buffer[BUFFER_SIZE];
UINT16 out_head = 0;
UINT16 out_tail = 0;
UINT16 out_fill = 0;

char input_buffer[BUFFER_SIZE];
UINT16 in_head = 0;
UINT16 in_tail = 0;
UINT16 in_fill = 0;

Vector old_transmit_empty;
Vector old_receive_full;

/*
Initializes the serial interface and its associated buffers, and installs 
the needed ISRs 
*/
void serial_setup()
{
	UINT32 old_ssp = Super (1);
	
	if (old_ssp == 0)
		old_ssp = Super(0);
		
	old_transmit_empty = install_vector (TX_EMPTY_VECTOR, tx_isr);
	old_receive_full = install_vector (RX_FULL_VECTOR, rx_isr);

	*RX_STATUS |= ENABLE_TX_RX;
	*TX_STATUS |= ENABLE_TX_RX;
	
	*USART_CNTRL = CNTRL_STATUS;
	*IRQ_ENABLE_A |= ENABLE_USART_INTERRUPTS;
	*IRQ_MASK_A |= ENABLE_USART_INTERRUPTS;
			
	if (old_ssp != -1)
		Super(old_ssp);
}

/*
*/
void serial_teardown()
{
	UINT32 old_ssp = Super (1);
	
	if (old_ssp == 0)
		old_ssp = Super(0);
		
	*IRQ_ENABLE_A &= ~ENABLE_USART_INTERRUPTS;
	*IRQ_MASK_A &= ~ENABLE_USART_INTERRUPTS;
	
	*RX_STATUS &= ~ENABLE_TX_RX;
	*TX_STATUS &= ~ENABLE_TX_RX;

	install_vector (TX_EMPTY_VECTOR, old_transmit_empty);
	install_vector (RX_FULL_VECTOR, old_receive_full);
	
	if (old_ssp != -1)
		Super(old_ssp);
}

/* Enqueues size bytes from buff in the serial output buffer.  This
function is non-blocking, meaning that it returns without waiting for
the data to be transmitted.  Transmission continues (in the background).

The number of bytes actually enqueued is returned (this may be less than
size if there was insufficient space in the buffer) */
int serial_write(const char *buff, unsigned int size)
{
	UINT16 i;
	UINT32 old_ssp;
	/* Remember to mask here */
	for (i = 0; i < size && out_fill < BUFFER_SIZE; i++)
	{
		out_fill++;
		output_buffer[out_tail] = buff[i];
		out_tail = (out_tail + 1) & (BUFFER_SIZE - 1);
	}
	
	return i;
}
/*Like serial_write, except that it dequeues up to size bytes from the 
serial input buffer and stores them in buff.  The number of bytes actually
dequeued is returned*/
int serial_read(char *buff, unsigned int size)
{
	UINT16 i;
	for (i = 0; i < size && in_fill > 0; i++)
	{
		buff[i] = input_buffer[in_head];
		in_head = (in_head + 1) & (BUFFER_SIZE - 1);
		in_fill--;
	}
	
	return i;	
}


/* returns the number of bytes which 
can be successfully written at the time of call. */
int serial_can_write()
{
	return (int)((BUFFER_SIZE - out_tail) + out_head);
}

/*Like serial_can_write, but for reading */
int serial_can_read()
{
	return (int)(in_fill);
}

/*Returns 0 if the serial output buffer is empty, returns non-0 otherwise.
In other words, this is used to test if all previously written bytes have
been transmitted or not */
int serial_tx_pending()
{
	return out_fill;
}

/*The ISR which is invoked whenever the USART's Tx register becomes empty */
void tx_empty_isr()
{
	UINT32 old_ssp = Super(1);
	
	if (out_fill > 0)
	{
		if (old_ssp == 0)
			old_ssp = Super(0);
	
		out_fill--;
		*USART_DATA = output_buffer[out_head];
		out_head = (out_head + 1) & (BUFFER_SIZE - 1);

		*ISR_A_IN_SERVICE &= CLEAR_TX_PENDING;
		
		if (old_ssp != -1)
			Super(old_ssp);
	}
}

/*The ISR which is invoked whenever the USART's Rx register becomes full.*/

void rx_full_isr()
{
	UINT32 old_ssp = Super(1);
	
	if (in_fill < BUFFER_SIZE)
	{
		if (old_ssp == 0)
			old_ssp = Super(0);
			
		in_fill++;
		input_buffer[in_tail] = *USART_DATA;
		in_tail = (in_tail + 1) & (BUFFER_SIZE - 1);
		
		*ISR_A_IN_SERVICE &= CLEAR_RX_PENDING;
		
		if (old_ssp != -1)
			Super(old_ssp);
	}
}
 
 
