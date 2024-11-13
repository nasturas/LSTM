#pragma once
#include <random>
#include <cmath>

class XavierNormalised
{
public:
	XavierNormalised(int num_input, int num_output)
	{
		std::random_device rd;
		if (num_input + num_output != 0)
		{
			varianta = sqrt(6.0 / (num_input + num_output));
		}
		else
		{
			varianta = 1;
		}
		generator = std::mt19937(rd()); 
		distributie = std::normal_distribution<>(0, varianta);
	}

	double generateNumber();
	//TODO: ar tb sa intoarca un pointer la un obiect sau sa fie prin move semantics?
	std::vector<double> generateVector(int size);
	std::vector<std::vector<double>> generateMatrix(int size_r, int size_c);
private:
	double varianta; 
	std::mt19937 generator; 
	std::normal_distribution<> distributie;
};

