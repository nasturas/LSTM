#include "Test_Vector.h"

int Test_Vector::get_Dim_Test()
{
	return this->test.size();
}

int Test_Vector::get_Dim_Rezultat()
{
	return this->rezultat.size();
}

void Test_Vector::assign_Test_Elem(std::vector<double>::iterator prim, std::vector<double>::iterator ultim)
{
	this->test.assign(prim, ultim);
}

void Test_Vector::assign_Rezultat_Elem(std::vector<double>::iterator prim, std::vector<double>::iterator ultim)
{
	this->rezultat.assign(prim, ultim);
}

double Test_Vector::get_Test_Elem(int idx)
{
	return test[idx];
}

double Test_Vector::get_Rezultat_Elem(int idx)
{
	return rezultat[idx];
}

vector<double> Test_Vector::get_Test_Vector()
{
	return test;
}

vector<double> Test_Vector::get_Rezultat_Vector()
{
	return rezultat;
}
