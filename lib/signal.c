/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  signal.c - implementation of stuff in signal.h                   */
/*                                                                   */
/*********************************************************************/
#define void int
#include "signal.h"
#include "stdlib.h"
#include "stddef.h"

static void (*handlers[])() = {
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl };

void __sigdfl();
void __sigerr();
void __sigign();

#define SIG_DFL __sigdfl
#define SIG_ERR __sigerr
#define SIG_IGN __sigign

/* __PDPCLIB_API__ void (*signal(int sig, void (*func)(int)))(int) */
__PDPCLIB_API__ void (*signal(sig, func))() int sig; void (*func)();
{
    handlers[sig] = func;
    return (func);
}


__PDPCLIB_API__ int raise(sig) int sig;
{
    (handlers[sig])(sig);
    return (0);
}

__PDPCLIB_API__ void __sigdfl(sig) int sig;
{
    handlers[sig] = SIG_DFL;
    if (sig == SIGABRT)
    {
        exit(EXIT_FAILURE);
    }
    return;
}

__PDPCLIB_API__ void __sigerr(sig) int sig;
{
    (void)sig;
    return;
}

__PDPCLIB_API__ void __sigign(sig) int sig;
{
    (void)sig;
    return;
}
