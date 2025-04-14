#pragma once
class EnvironmentData
{
private:
	int numar_maxim_antrenari;
	int procent_precizie_antrenament;
	static EnvironmentData* instance;
	EnvironmentData(int val, int proc) : numar_maxim_antrenari(val), procent_precizie_antrenament(proc) {}
public:
	EnvironmentData(const EnvironmentData&) = delete;
	EnvironmentData& operator=(const EnvironmentData&) = delete;

	static EnvironmentData* getInstance(int val, int proc) 
	{
		if (!instance)
		{
			instance = new EnvironmentData(val, proc);
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
};

