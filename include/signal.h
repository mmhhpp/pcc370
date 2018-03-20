/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  signal.h - signal header file.                                   */
/*                                                                   */
/*********************************************************************/

#ifndef __SIGNAL_INCLUDED
#define __SIGNAL_INCLUDED

typedef int sig_atomic_t;

__sigdfl();
__sigerr();
__sigign();

#define SIG_DFL __sigdfl
#define SIG_ERR __sigerr
#define SIG_IGN __sigign

#define SIGABRT 1
#define SIGFPE 2
#define SIGILL 3
#define SIGINT 4
#define SIGSEGV 5
#define SIGTERM 6
#define SIGQUIT 7
#define SIGHUP  1

(*signal())();
int raise();

#endif
