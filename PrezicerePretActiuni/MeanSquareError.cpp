#include "MeanSquareError.h"

double MeanSquareError::GetLoss(vector<double> y_true, vector<double> y_pred) const
{
	double mean = 0;
	for (int i = 0; i < y_true.size(); i++)
	{
		mean += pow(y_true[i] - y_pred[i],2);
	}
	return   mean/y_true.size();
}