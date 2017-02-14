#include <stddef.h>

#pragma linkage(__aopen, OS)
void *__aopen();
#pragma linkage(__aread, OS)
int __aread();
#pragma linkage(__awrite, OS)
int __awrite();
#pragma linkage(__aclose, OS)
void __aclose();
#pragma linkage(__getclk, OS)
unsigned int __getclk();
#pragma linkage(__gettz, OS)
int __gettz();
#pragma linkage(__getm, OS)
void *__getm();
#pragma linkage(__freem, OS)
void __freem();

#pragma linkage(__dynal, OS)
int __dynal();

#pragma linkage(__idcams, OS)
int __idcams();

#pragma linkage(__system, OS)
#ifdef MUSIC
int __system(int len, const char *command);
int __textlc(void);
int __svc99(void *rb);
#else
int __system();
#endif
