#include <stddef.h>

void *__aopen();
int __aread();
int __awrite();
void __aclose();
unsigned int __getclk();
int __gettz();
void *__getm();
void __freem();

int __dynal();

int __idcams();

#ifdef MUSIC
int __system(int len, const char *command);
int __textlc(void);
int __svc99(void *rb);
#else
int __system();
#endif
