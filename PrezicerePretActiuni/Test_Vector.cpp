#include "Test_Vector.h"

size_t Test_Vector::get_Dim_Test()
{
	return this->test.size();
}

size_t Test_Vector::get_Dim_Rezultat()
{
	return this->rezultat.size();
}

void Test_Vector::assign_Test_Elem(std::vector<std::vector<double>>::iterator prim, std::vector<std::vector<double>>::iterator ultim)
{
	this->test.assign(prim, ultim);
}

void Test_Vector::assign_Rezultat_Elem(std::vector<std::vector<double>>::iterator prim, std::vector<std::vector<double>>::iterator ultim)
{
	this->rezultat.clear();
	for (auto it = prim; it != ultim; ++it)
	{
		this->rezultat.push_back((*it)[0]);
	}
}

vector<double> Test_Vector::get_Test_Elem(int idx)
{
	return test[idx];
}

double Test_Vector::get_Rezultat_Elem(int idx)
{
	return rezultat[idx];
}

vector<vector<double>> Test_Vector::get_Test_Vector()
{
	return test;
}

vector<double> Test_Vector::get_Rezultat_Vector()
{
	return rezultat;
}
