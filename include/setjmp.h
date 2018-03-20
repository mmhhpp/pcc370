/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  setjmp.h - setjmp header file.                                   */
/*                                                                   */
/*********************************************************************/

#ifndef __SETJMP_INCLUDED
#define __SETJMP_INCLUDED

typedef struct {
#if defined(__MVS__) || defined(__CMS__) || defined(__VSE__)
    int regs[15];
#else
#if defined(__WIN32__) || defined(__32BIT__) || defined(__OS2__) \
    || defined(__gnu_linux__)
    int ebx;
    int ecx;
    int edx;
    int edi;
    int esi;
    int esp;
    int ebp;
    int retaddr;
#endif
#endif
    int retval_;
} jmp_buf[1];

#ifdef ibm
int __setj();
#define setjmp(x) (__setj(x))
longjmp();
#else
int __setj(jmp_buf env);
#define setjmp(x) (__setj(x))
void longjmp(jmp_buf env, int val);
#endif

#endif
