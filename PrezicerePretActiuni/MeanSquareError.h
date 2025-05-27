#pragma once
#include "ILossFunction.h"
class MeanSquareError :
    public ILossFunction
{
public:
    double GetLoss(vector<double> y_true, vector<double> y_pred) const override;
    double GetLoss(vector<vector<double>> y_true, vector<vector<double>> y_pred) const override;
};

