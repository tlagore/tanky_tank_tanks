#include "system.h"
#include "types.h"
#include <osbind.h>

#define CLEAR 0x00
#define VECTOR_CLEAR 0x07
#define VECTOR_BASE	0x48
#define VECTOR_BASE_MASK 0x4F
#define ENABLE_ACIA 0x40
#define ENABLE_TIMER_D 0x10
#define IO_STATE 0x10
/* monochrome set, rest set to disable */

#define IKBD_STATE 0x82  
/*interrupts enabled, divide clock down 64 */

int mask_ipl(int mask);

volatile UINT8 * const MFP_GP_IO			= 0xFFFFFA01;

volatile UINT8 * const MFP_VECTOR_BASE		= 0xFFFFFA17;
volatile UINT8 * const INTERRUPT_ENABLE_A 	= 0xFFFFFA07;
volatile UINT8 * const INTERRUPT_MASK_A 	= 0xFFFFFA13;
volatile UINT8 * const INTERRUPT_ENABLE_B	= 0xFFFFFA09;
volatile UINT8 * const INTERRUPT_MASK_B		= 0xFFFFFA15;
/* ACIA = bit 6 */

volatile UINT8 * const TIMER_A				= 0xFFFFFA19;
volatile UINT8 * const TIMER_B				= 0xFFFFFA1B;
volatile UINT8 * const TIMERS_CD			= 0xFFFFFA1D;
/*Timer C delay mode bits 6-4, timer D delay mode bits 2-0

Value	Delay (divider)
000	 	Timer stop
001 	4
010 	10
011 	16
100		50
101		64
110		100
111		200
*/

volatile UINT8 * const TIMER_A_DATA 		= 0xFFFFFA1F;
volatile UINT8 * const TIMER_B_DATA			= 0xFFFFFA21;

volatile UINT8 * const ACIA_CONTROL 		= 0xFFFFFC00;  

/*
Name: initialize
Purpose: Initializes the hardware to proper values to ensure that
	necessary interrupts are being sent and handled with the 
	knowledge that TOS will no longer be responding to IRQ's
*/

void initialize()
{
	UINT32 old_ssp = Super (1);
	
	if (old_ssp == 0)
		old_ssp = Super(0);
	
	*ACIA_CONTROL = IKBD_STATE;
		
	*MFP_GP_IO = IO_STATE;
		
	*MFP_VECTOR_BASE &= VECTOR_CLEAR; /* Set vector base */
	*MFP_VECTOR_BASE |= VECTOR_BASE;
		
	*INTERRUPT_ENABLE_A = CLEAR;
	*INTERRUPT_MASK_A = CLEAR;
	/* Disable all interrupts on Register A */
	
	*INTERRUPT_ENABLE_B = CLEAR;
	*INTERRUPT_ENABLE_B |= (ENABLE_ACIA | ENABLE_TIMER_D);
	/* Enable ACIA and timer D interrupts*/
	
	*INTERRUPT_MASK_B = CLEAR;
	*INTERRUPT_MASK_B |= (ENABLE_ACIA | ENABLE_TIMER_D);
	/* Mask everything except ACIA and timer D interrupts */
		
	if (old_ssp != -1)
		Super(old_ssp);
}

/*
Name: super_mask_ipl
Purpose: Checks to see if the process is in user or supervisor mode, 
	to ensure that it is in supervisor mode, then changes the 
	ipl_mask to the given value.
	
Input: 
	int mask - the mask to set the new ipl to (must be between 0 and 7)
	
Output:
	int old_ipl - the value of the old ipl bits before the mask was set
*/
int super_mask_ipl(int mask)
{
	UINT32 old_ssp = Super(1);
	int old_ipl;

	if (old_ssp == 0)
		old_ssp = Super(0);
		
	if (mask > 0 && mask <= 7)
		old_ipl = mask_ipl(mask);

	if (old_ssp != -1)
		Super(old_ssp);
		
	return old_ipl;
}

/*
Name: install_vector
Purpose: Installs a new vector jump address at a given vector number to 
	replace the currently installed ISR with another.
	
Input:
	int num: The vector to install the new vector jump address at
	Vector vector: The vector address to install

*/
Vector install_vector(int num, Vector vector)
{
	Vector orig;
	Vector *vectp = (Vector *)((long)num << 2);
	long old_ssp = Super(1);
	
	if (old_ssp == 0)
		old_ssp = Super(0);

	orig = *vectp;
	*vectp = vector;

	if (old_ssp != -1)
		Super(old_ssp);
		
	return orig;
}
