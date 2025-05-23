#include "ZScoreNormalisation.h"
#include <cmath>
vector<double> ZScoreNormalisation::Normalise(vector<double> data_set) const
{
	vector<double> rtn(data_set.size(), 0.0);
	double tmpMean=0;
	double tmpStandardDeviation = 0;
	//calculam media
	for (int i = 0; i < data_set.size(); i++)
	{
		tmpMean += data_set[i];
	}
	tmpMean /= data_set.size();
	//scadem media si ridicam la patrat
	for (int i = 0; i < data_set.size(); i++)
	{
		tmpStandardDeviation += pow(data_set[i] - tmpMean, 2);
	}
	tmpStandardDeviation = sqrt(tmpStandardDeviation/data_set.size());
	
	//finalu Z = (x-tmpMean)/tmpStandardDeviation
	for (int i = 0; i < data_set.size(); i++)
	{
		rtn[i] = (data_set[i] - tmpMean) / tmpStandardDeviation;
	}

	return rtn;
}

double ZScoreNormalisation::Denormalise(vector<double> data_set, double val) const
{
	double tmpMean = 0;
	double tmpStandardDeviation = 0;
	//calculam media
	for (int i = 0; i < data_set.size(); i++)
	{
		tmpMean += data_set[i];
	}
	tmpMean /= data_set.size();
	//scadem media si ridicam la patrat
	for (int i = 0; i < data_set.size(); i++)
	{
		tmpStandardDeviation += pow(data_set[i] - tmpMean, 2);
	}
	tmpStandardDeviation = sqrt(tmpStandardDeviation / data_set.size());

	return (val * tmpStandardDeviation + tmpMean);
}