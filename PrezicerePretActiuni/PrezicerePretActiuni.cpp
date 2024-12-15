// PrezicerePretActiuni.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "LSTM_cell.h"
#include "XavierNormalised.h"
int main()
{
	std::vector<int> vec;
	vec.resize(2);
	std::vector<int> c2;
	c2.resize(2);
	vec[0] = 1;
	vec[1] = 2;
	c2[0] = 123;
	c2[1] = 124434;

	c2 = vec;

	std::cout << c2[0]<<","<<c2[1];

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
