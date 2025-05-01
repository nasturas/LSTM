#pragma once
#include <vector>

class Interpolare
{
private:
	size_t dimVector = 200;
	//vectorY e un vector de 200 de elemente cu valorile functiei pentru interpolare.
	//presupunem ca pasul x al valorilor de intrare e 0.1 de la -10 la 10
	double* vectorY;
	static std::vector<double> vectorSigmoid;
	static std::vector<double> vectorTanh;
	static Interpolare* sigmoid;
	static Interpolare* tanh;

	Interpolare() { vectorY = nullptr; }
	Interpolare(double* vectorOut) { vectorY = vectorOut; }
	Interpolare(const Interpolare&) = delete;
	Interpolare& operator=(const Interpolare&) = delete;
public:
	static Interpolare* getSigmoid(); 
	static Interpolare* getTanh();
	double Output(double x);
	double sigmoid_derivata(double x) { return x * (1 - x); }
};

