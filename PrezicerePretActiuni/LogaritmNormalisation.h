#pragma once
#include "IDataNormalisation.h"
class LogaritmNormalisation :
    public IDataNormalisation
{
public:
    vector<double> Normalise(vector<double> data_set) const override;
    double Denormalise(vector<double> data_set, double val) const override;
};

