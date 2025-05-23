#pragma once
#include "ILossFunction.h"
class MeanSquareError :
    public ILossFunction
{
public:
    double GetLoss(vector<double> y_true, vector<double> y_pred) const override;
};

