#pragma once
#include "IDataNormalisation.h"
#include "LogaritmNormalisation.h"
#include "ZScoreNormalisation.h"
#include "MeanSquareError.h"
#define IDX_PREDICTED_FEATURE 0

/// <summary>
/// Tipuri de normalisari
/// dupa http://www.mirlabs.org/ijcisim/regular_papers_2014/IJCISIM_24.pdf
/// </summary>
enum DataNormalisationStyle
{
	Logaritm=0,
	ZScore=1,
	RolingAverage=2
};

enum LossFunctionStyle
{
	MSE,
	RMSE
};

class EnvironmentData
{
private:
	int numar_maxim_antrenari;
	int procent_precizie_antrenament;
	double lambda;
	DataNormalisationStyle normalisationStyle;
	IDataNormalisation* normalisation;
	LossFunctionStyle lossFunctionStyle;
	ILossFunction* lossFunction;
	static EnvironmentData* instance;

	EnvironmentData(int val, int proc, double lam, DataNormalisationStyle normStyle, LossFunctionStyle lossFStyle) : numar_maxim_antrenari(val), procent_precizie_antrenament(proc), lambda(lam), normalisationStyle(normStyle), lossFunctionStyle(lossFStyle)
	{ 
		switch (normStyle)
		{
		case DataNormalisationStyle::Logaritm:
		{
			normalisation = new LogaritmNormalisation();
			break;
		}
		case DataNormalisationStyle::ZScore:
		{
			normalisation = new ZScoreNormalisation();
		}
		default:
			normalisation = nullptr;
			break;
		}
		switch (lossFStyle)
		{
		case MSE:
		{
			lossFunction = new MeanSquareError();
			break;
		}
		case RMSE:
			lossFunction = nullptr;
		default:
			lossFunction = new MeanSquareError();
			break;
		}
	}
public:
	EnvironmentData(const EnvironmentData&) = delete;
	EnvironmentData& operator=(const EnvironmentData&) = delete;

	static EnvironmentData* getInstance(int val, int proc, double lambda, DataNormalisationStyle normStyle, LossFunctionStyle lossFStyle) 
	{
		if (instance==nullptr)
		{
			instance = new EnvironmentData(val, proc, lambda, normStyle, lossFStyle);
		}
		return instance;
	}

	const DataNormalisationStyle getNormalisationStyle()
	{
		return normalisationStyle;
	}

	const int getNumarMaximAntrenari()  
	{
		return numar_maxim_antrenari;
	}

	const int getProcentPrecizieAntrenament()
	{
		return procent_precizie_antrenament;
	}

	const double getLamda() { return lambda; }
	
	const IDataNormalisation* getNormalisation()
	{
		return normalisation;
	}

	const ILossFunction* GetLossFunction()
	{
		return lossFunction;
	}
};

