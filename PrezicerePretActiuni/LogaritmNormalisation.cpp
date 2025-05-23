#include "LogaritmNormalisation.h"
#include <cmath>

vector<double> LogaritmNormalisation::Normalise(vector<double> data_set) const
{
	vector<double> rtn(data_set.size(), 0.0);

	for (int i = 1; i < data_set.size(); i++)
	{
		rtn[i] = log(data_set[i] / data_set[i - 1]);
		if (rtn[i] < -1.0)
			rtn[i] = -1.0;
		if (rtn[i] > 1.0)
			rtn[i] = 1.0;
	}
	return rtn;
}
double LogaritmNormalisation::Denormalise(vector<double> data_set, double val) const
{
	double rtn = 0;
	rtn = exp(val) * data_set[data_set.size() - 1];
	return rtn;
}