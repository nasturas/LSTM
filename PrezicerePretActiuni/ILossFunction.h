#pragma once
#include "Test_Vector.h"
using namespace std;

class ILossFunction
{
public:
	virtual double GetLoss(vector<double> y_true, vector<double> y_pred) const = 0;
};

