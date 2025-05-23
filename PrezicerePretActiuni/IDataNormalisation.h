#pragma once
#include <vector>
using namespace std;

class IDataNormalisation
{
public:
	virtual vector<double> Normalise(vector<double> data_set) const = 0;
	virtual double Denormalise(vector<double> data_set, double val) const = 0;
};

