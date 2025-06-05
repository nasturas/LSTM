// PrezicerePretActiuni.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "LSTM_cell.h"
#include "XavierNormalised.h"
#include "Test_Vector.h"
#include "EnvironmentData.h"
#include "LSTMLayer.h"
using namespace std;

EnvironmentData* envData = EnvironmentData::getInstance(200, 95, 0.1, DataNormalisationStyle::Logaritm, LossFunctionStyle::MSE);
int main()
{
	
	LSTMLayer* lstm_test = new LSTMLayer(1, 1, 2);
	vector<double> vector_de_date = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	vector<double> vector_normalisat(vector_de_date.size(), 0.0);
	vector_normalisat = envData->getNormalisation()->Normalise(vector_de_date);
	vector<vector<double>> data_in(vector_de_date.size(), std::vector<double>(1,0.0));
	for(int i=0; i<vector_de_date.size();i++)
		data_in[i][0] = vector_normalisat[i];
	vector<Test_Vector> training_set, test_set;
	try {
		lstm_test->PrepareTraining(data_in, &training_set, &test_set, 2);
	}
	catch(const invalid_argument& e)
	{ 
		cout << "Invalid arg: " << e.what() << endl;
	}
	
	try 
	{
		lstm_test->Train(data_in, 2, envData->getLamda());
		vector<double> myvar=lstm_test->ForwardPass({ 12,13 });
		cout<<envData->getNormalisation()->Denormalise({ 12,13 },myvar[0]);
	}
	catch (const invalid_argument e)
	{
		cout << "Invalid arg: " << e.what() << endl;
	}
}

//TODO de gasit o metoda de a masura relevant eroarea. 

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
