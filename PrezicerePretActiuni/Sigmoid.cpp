#include "Sigmoid.h"
#include <cmath>

double Sigmoid::Output(double x) const
{
    return (1/(1+exp(-x)));
}

double Sigmoid::Derivate(double x) const
{
    double s = Output(x);
    return s*(1-s);
}
