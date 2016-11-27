 #ifndef SYSTEM_H
 #define SYSTEM_H
 
typedef void (*Vector)();

Vector install_vector(int num, Vector vector);
int super_mask_ipl(int mask);
void initialize();
 
 #endif