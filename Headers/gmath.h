#pragma once
#ifndef GMATH_H
#define GMATH_H

template<typename T>
T Clamp(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
#endif