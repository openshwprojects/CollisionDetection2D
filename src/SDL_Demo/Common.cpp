#include "Common.h"

float RandomFloatRange(float min, float max) {
    float f = ((float)rand()) / RAND_MAX;
    f = f * (max - min) + min;
    return f;
}
