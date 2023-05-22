#ifndef __COMMON_H__
#define __COMMON_H__

#include <math.h>

#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288
#endif

#define DEG2RAD(deg) ((deg) * M_PI / 180.0)
#define RAD2DEG(rad) ((rad) * 180.0 / M_PI)

typedef unsigned char byte;

#include <cstdio>
#include <cstdlib>

float RandomFloatRange(float min, float max);

#include <vector>

template<typename T>
class Array : public std::vector<T>
{
public:
};

#endif



