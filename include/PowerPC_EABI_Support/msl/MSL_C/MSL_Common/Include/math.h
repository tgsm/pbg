#ifndef _MSL_MATH_H
#define _MSL_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

inline double fabs(double x) { return __fabs(x); }
double pow(double x, double y);
double floor(double x);

#ifdef __cplusplus
}
#endif

#endif
