#include "Tanh.h"
#include <cmath>
double Tanh::Output(double x) const
{
    return ( (exp(x)-exp(-x))/(exp(x)+exp(-x)));
}

double Tanh::Derivate(double x) const
{
    double t = Output(x);
    return ( 1-t*t );
}
