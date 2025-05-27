#include "MeanSquareError.h"

/// Functie care intoarce MSE pentru un sir de estimari scalare
/// </summary>
/// <param name="y_true"></param>
/// <param name="y_pred"></param>
/// <returns></returns>
double MeanSquareError::GetLoss(vector<double> y_true, vector<double> y_pred) const
{
	double mean = 0;
	for (int i = 0; i < y_true.size(); i++)
	{
		mean += pow(y_true[i] - y_pred[i],2);
	}
	return   mean/y_true.size();
}

double MeanSquareError::GetLoss(vector<vector<double>> y_true, vector<vector<double>> y_pred) const
{
	double mean = 0;
	for (int i = 0; i < y_true.size(); i++)
	{
		for(int j=0; j<y_true[0].size(); j++)
			mean += pow(y_true[i][j] - y_pred[i][j], 2);
	}
	return   mean / y_true.size();
}