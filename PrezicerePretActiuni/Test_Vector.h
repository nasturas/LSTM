#pragma once

#include <vector>
using namespace std;

class Test_Vector
{

private:

	vector<vector<double>> test;
	vector<double> rezultat;

public:
	Test_Vector() {}
	~Test_Vector() {}

	size_t get_Dim_Test();
	size_t get_Dim_Rezultat();

	void assign_Test_Elem(std::vector<std::vector<double>>::iterator prim, std::vector<std::vector<double>>::iterator ultim);
	void assign_Rezultat_Elem(std::vector<std::vector<double>>::iterator prim, std::vector<std::vector<double>>::iterator ultim);

	vector<double> get_Test_Elem(int idx);
	double get_Rezultat_Elem(int idx);

	vector<vector<double>> get_Test_Vector();
	vector<double> get_Rezultat_Vector();
};

