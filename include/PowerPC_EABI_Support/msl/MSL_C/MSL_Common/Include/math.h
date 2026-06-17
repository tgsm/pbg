#ifndef _MSL_MATH_H
#define _MSL_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

inline double fabs(double x) { return __fabs(x); }
inline float fabsf(float x) { return __fabsf(x); }
double pow(double x, double y);
double floor(double x);
double sin(double x);
inline float sinf(float x) { return sin(x); }
double cos(double x);
inline float cosf(float x) { return cos(x); }
double atan2(double y, double x);

#ifdef __cplusplus
}
#endif

#endif
