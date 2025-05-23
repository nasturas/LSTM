// PrezicerePretActiuni.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "LSTM_cell.h"
#include "XavierNormalised.h"
#include "Test_Vector.h"
#include "EnvironmentData.h"
#include "LSTM_cell.h"
using namespace std;

EnvironmentData* envData = EnvironmentData::getInstance(200, 95, 0.2, DataNormalisationStyle::Logaritm, LossFunctionStyle::MSE);
int main()
{
	
	LSTM_cell* lstm_test = new LSTM_cell(2, 1);
	vector<double> vector_de_date = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	vector<double> vector_normalisat(vector_de_date.size(), 0.0);
	vector_normalisat = envData->getNormalisation()->Normalise(vector_de_date);
	vector<Test_Vector> training_set, test_set;
	try {
		lstm_test->PrepareTraining(vector_normalisat, &training_set, &test_set, 2);
		for (Test_Vector t : training_set)
		{
			for (double d : t.get_Test_Vector())
				cout << d << " ";
			cout << " - ";
			for (double d : t.get_Rezultat_Vector())
				cout << d << " ";
			cout << endl;
		}
		cout << "miu" << endl;
		for (Test_Vector t : test_set)
		{
			for (double d : t.get_Test_Vector())
				cout << d << " ";
			cout << " - ";
			for (double d : t.get_Rezultat_Vector())
				cout << d << " ";
			cout << endl;
		}
	}
	catch(const invalid_argument& e)
	{ 
		cout << "Invalid arg: " << e.what() << endl;
	}
	
	//lstm_test->ForwardPass(training_set[0].get_Test_Vector());
	//lstm_test->TrainLSTM(training_set, envData->getLamda());
	//cout << "Eroare e: "<< lstm_test->TestLSTM(&test_set);

	lstm_test->Train(vector_normalisat, 2, envData->getLamda());
	cout<<"haha: "<< envData->getNormalisation()->Denormalise(vector_de_date,lstm_test->ForwardPass(envData->getNormalisation()->Normalise({11.0,12.0}))[0]);
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
