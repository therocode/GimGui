#include <helpers/closeenough.hpp>
#include <cmath>

bool closeEnough(float a, float b)
{
    return fabs(a - b) < 0.00001;
}
