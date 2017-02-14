/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  ctype.h - ctype header file.                                     */
/*                                                                   */
/*********************************************************************/

#ifndef __CTYPE_INCLUDED
#define __CTYPE_INCLUDED

extern unsigned short *__isbuf;
extern short *__tolow;
extern short *__toup;

#define isalnum(c) (__isbuf[(c)] & 0x0001)
#define isalpha(c) (__isbuf[(c)] & 0x0002)
#define iscntrl(c) (__isbuf[(c)] & 0x0004)
#define isdigit(c) (__isbuf[(c)] & 0x0008)
#define isgraph(c) (__isbuf[(c)] & 0x0010)
#define islower(c) (__isbuf[(c)] & 0x0020)
#define isprint(c) (__isbuf[(c)] & 0x0040)
#define ispunct(c) (__isbuf[(c)] & 0x0080)
#define isspace(c) (__isbuf[(c)] & 0x0100)
#define isupper(c) (__isbuf[(c)] & 0x0200)
#define isxdigit(c) (__isbuf[(c)] & 0x0400)
#define tolower(c) (__tolow[(c)])
#define toupper(c) (__toup[(c)])

#endif
