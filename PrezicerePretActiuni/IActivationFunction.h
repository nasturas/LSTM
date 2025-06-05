#pragma once
class IActivationFunction
{
public:
	virtual double Output(double x) const = 0;
	virtual double Derivate(double x) const = 0;
};

