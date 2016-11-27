#ifndef SERIAL_H
#define SERIAL_H
 
void serial_setup();
void serial_teardown();
int serial_write(const char *buff, unsigned int size);
int serial_read(char *buff, unsigned int size);

int serial_can_write();
int serial_can_read();

int serial_tx_pending();


#endif