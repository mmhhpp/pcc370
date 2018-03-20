/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  stdlib.h - stdlib header file                                    */
/*                                                                   */
/*********************************************************************/

#ifndef __STDLIB_INCLUDED
#define __STDLIB_INCLUDED

#define void int
#define const

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
#if (defined(__OS2__) || defined(__32BIT__) || defined(__MVS__) \
    || defined(__CMS__) || defined(__VSE__))
typedef unsigned long size_t;
#else
#if (defined(__MSDOS__) || defined(__DOS__) || defined(__POWERC) \
    || defined(__WIN32__) || defined(__gnu_linux__))
typedef unsigned int size_t;
#endif
#endif
#endif
#ifndef __WCHAR_T_DEFINED
#define __WCHAR_T_DEFINED
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#endif
typedef char wchar_t;
#endif
typedef struct { int quot; int rem; } div_t;
typedef struct { long l_quot; long l_rem; } ldiv_t;

#ifndef NULL
#define NULL (0)
#endif
#define EXIT_SUCCESS 0
#if defined(__MVS__) || defined(__CMS__) || defined(__VSE__)
#define EXIT_FAILURE 12
#else
#define EXIT_FAILURE 1
#endif
#define RAND_MAX 32767
#define MB_CUR_MAX 1
#define __NATEXIT 32

char *malloc();
char *calloc();
char *realloc();
void free();
void abort();
void exit();
void qsort();
void srand();
int rand();
double atof();
double strtod();
int atoi();
long int atol();
long int strtol();
unsigned long int strtoul();
int mblen();
int mbtowc();
int wctomb();
size_t mbstowcs();
size_t wcstombs();
int abs();
div_t div();
long int labs();
ldiv_t ldiv();
int atexit();
char *getenv();
int system();
void *bsearch();

#endif
