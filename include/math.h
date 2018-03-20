/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  math.h - math header file.                                       */
/*                                                                   */
/*********************************************************************/

#ifndef __MATH_INCLUDED
#define __MATH_INCLUDED

/*
  Some constants - at present these are only defined for IBM
*/
#if defined(__MVS__) || defined (__CMS__) || defined(__VSE__)
/*
 This is about the nearest thing we can get to inf wthout
*/
#define HUGE_VAL 10E72

#else

#define HUGE_VAL 9.999999999999999999999E72

#endif

double ceil();
double fabs();
double floor();
double fmod();

/* unimplemented: */


double atan2();
double frexp();
double ldexp();
double modf();

/*
  Implemented by d.wade - April 2006
*/

double pow();
double sqrt();
double acos();
double asin();
double atan();
double cos();
double sin();
double tan();
double cosh();
double sinh();
double tanh();
double exp();
double log();
double log10();


#ifdef __WATCOMC__
#pragma intrinsic(cos, sin, tan, exp, log, log10, sqrt)
#endif

#ifdef __IBMC__
double _Builtin __fabs(double x);
#define fabs(x) (__fabs((x)))
double _Builtin __fsin(double x);
#define sin(x) (__fsin((x)))
double _Builtin __fcos(double x);
#define cos(x) (__fcos((x)))
double _Builtin __fptan(double x);
#define tan(x) (__fptan((x)))
double _Builtin __fpatan(double x);
#define atan(x) (__fpatan((x)))
double _Builtin __facos(double x);
#define acos(x) (__facos((x)))
double _Builtin __fasin(double x);
#define asin(x) (__fasin((x)))
double _Builtin __fsqrt(double x);
#define sqrt(x) (__fsqrt((x)))
#endif

#ifdef __BORLANDC__
double __sin__(double x);
#define sin(x) (__sin__((x)))
double __cos__(double x);
#define cos(x) (__cos__((x)))
#endif

#endif

