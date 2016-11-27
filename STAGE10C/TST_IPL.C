#include <osbind.h>

void do_VBL();
void do_get_input();
int mask_ipl(int mask);

int main()
{
	unsigned long old_ssp;
	unsigned long tst_ssp;
	int old_ipl;
	
	tst_ssp = Super(0);
	
	if (Super(1) == 0)
		old_ssp = Super(0);
	
	old_ipl = mask_ipl(7);
	mask_ipl(old_ipl);
	
	if (Super(1) != 0)
		Super(tst_ssp);	

	return 0;
}

void do_VBL(){}
void do_get_input(){}
