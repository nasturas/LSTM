// PrezicerePretActiuni.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "LSTM_cell.h"
#include "XavierNormalised.h"
#include "Test_Vector.h"

using namespace std;

void functie(vector<Test_Vector>* p, vector<double> vivi)
{
	Test_Vector tv;
	cout << "Marime in functie: " << p->size() << endl;
	int size = 4;
	for (int i = 0; i+ size <= vivi.size(); i = i + 2)
	{
		tv.assign_Test_Elem(vivi.begin() + i, vivi.begin() + size + i);
		//tv.assign_Rezultat_Elem(vivi.begin() + 2 + i, vivi.begin() + 2 +1+ i);
		p->push_back(tv);
	}

}
int main()
{
	vector<double> vivi = { 0,10,20,30,40,50,60,70,80,90 };
	vector<Test_Vector> ptr;


	functie(&ptr, vivi);
	cout << "Marime dupa functie: " << ptr.size() << endl;
	for (Test_Vector v : ptr)
	{
		for (int i=0;i<v.get_Dim_Test();i++)
			cout << v.get_Test_Elem(i) << " ";
		cout << ";" << endl;
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
