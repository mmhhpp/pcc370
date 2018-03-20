/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  string.h - string header file.                                   */
/*                                                                   */
/*********************************************************************/

#ifndef __STRING_INCLUDED
#define __STRING_INCLUDED
#define void int
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

#ifndef NULL
#define NULL (0)
#endif
void *memcpy();
void *memmove();
char *strcpy();
char *strncpy();
char *strcat();
char *strncat();
int memcmp();
int strcmp();
int strcoll();
int strncmp();
size_t strxfrm();
void *memchr();
char *strchr();
size_t strcspn();
char *strpbrk();
char *strrchr();
size_t strspn();
char *strstr();
char *strtok();
void *memset();
char *strerror();
size_t strlen();

#endif
