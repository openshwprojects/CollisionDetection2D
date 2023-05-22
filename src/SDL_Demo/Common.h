#ifndef __COMMON_H__
#define __COMMON_H__

#include <math.h>

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



