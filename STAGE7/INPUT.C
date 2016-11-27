#include <osbind.h>

char get_input()
{
	return (char)(Cnecin() >> 16);
}

int input_available()
{
	return Cconis();
}