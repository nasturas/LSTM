#pragma once
class EnvironmentData
{
private:
	int numar_maxim_antrenari;
	int procent_precizie_antrenament;
	double lambda;
	static EnvironmentData* instance;
	EnvironmentData(int val, int proc, double lam) : numar_maxim_antrenari(val), procent_precizie_antrenament(proc), lambda(lam){}
public:
	EnvironmentData(const EnvironmentData&) = delete;
	EnvironmentData& operator=(const EnvironmentData&) = delete;

	static EnvironmentData* getInstance(int val, int proc, double lambda) 
	{
		if (instance==nullptr)
		{
			instance = new EnvironmentData(val, proc, lambda);
		}
		return instance;
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
};

