// PrezicerePretActiuni.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "XavierNormalised.h"
#include "Test_Vector.h"
#include "EnvironmentData.h"
#include "LSTMLayer.h"
#include "FileHandler.h"
#include "nlohmann/json.hpp"

using namespace std;

vector<vector<double>> readInputJson(const std::string& json)
{
	nlohmann::json jsonData = nlohmann::json::parse(json);
	std::vector<std::vector<double>> vec;
	
	for (auto it = jsonData["features"].begin(); it != jsonData["features"].end(); ++it) {
		vec.push_back(it.value().get<std::vector<double>>());
	}
	return vec;
}

int main(int argc, char* argv[])
{
	FileHandler fileHandler;
	
	std::string configFile, inputFile, forwardFile;
	int num_intrari=0;

	for (int i = 1; i < argc; ++i) 
	{
		std::string arg = argv[i];

		if (arg.rfind("-c=", 0) == 0) 
		{
			configFile = arg.substr(3);  
		}
		else if (arg.rfind("-t=", 0) == 0) 
		{
			inputFile = arg.substr(3);  
		}
		else if (arg.rfind("-i=", 0) == 0)
		{
			try
			{
				num_intrari = stoi(arg.substr(3));
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Eroare: Nu este un numar valid! " << e.what() << std::endl;
			}
			catch (const std::out_of_range& e) {
				std::cerr << "Eroare: Numarul este prea mare! " << e.what() << std::endl;
			}
		}
		else if (arg.rfind("-forward=", 0) == 0)
		{
			forwardFile = arg.substr(9);
		}
	}

	if (configFile.empty()) 
	{
		std::cerr << "Eroare: Trebuie sa furnizezi un fisier de configurare.\n";
		return 1;
	}
	if (num_intrari == 0)
	{
		std::cerr << "Eroare: argumentul pentru -i nu e valid";
		return 1;
	}
	if (forwardFile.empty() && inputFile.empty())
	{
		std::cerr << "Eroare: Trebuie sa furnizezi un fisier de estimare sau un fisier de intrare.\n";
		return 1;
	}

	
	EnvironmentData* envData = EnvironmentData::fromJson(fileHandler.readFromFile(configFile));

	if (!inputFile.empty())
	{
		vector<vector<double>> vec = readInputJson(fileHandler.readFromFile(inputFile));

		//avem output sau hidden size 1 pentru ca estimam doar pentru o actiune.
		LSTMLayer* lstm_test = new LSTMLayer((int)vec.size(), 1, num_intrari);

		vector<vector<double>> matrixNormalizat(vec.size());
		for (int i = 0; i < vec.size(); i++)
			matrixNormalizat[i] = envData->getNormalisation()->Normalise(vec[i]);

		vector<vector<double>> data_in(vec[0].size(), std::vector<double>(vec.size(), 0.0));
		for (int j = 0; j < vec.size(); j++)
			for (int i = 0; i < vec[0].size(); i++)
				data_in[i][j] = matrixNormalizat[j][i];
		vector<Test_Vector> training_set, test_set;

		int stride = num_intrari / 2;
		if (stride < 2) stride = 2;
		try
		{
			lstm_test->Train(data_in, stride, envData->getLamda());
			vector<double> myvar = lstm_test->ForwardPass({ 12,13 });
			cout << envData->getNormalisation()->Denormalise({ 12,13 }, myvar[0]);
			FileHandler fileHandler;
			fileHandler.writeToFile("LSTMWeights.json", lstm_test->toJson());

		}
		catch (const invalid_argument e)
		{
			cout << "Invalid arg: " << e.what() << endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Eroare: " << e.what() << std::endl;
		}
	}
	else if (!forwardFile.empty())
	{
		vector<vector<double>> vec = readInputJson(fileHandler.readFromFile(forwardFile));
		if (vec[IDX_PREDICTED_FEATURE].size() != num_intrari)
		{
		std::cerr << "Valorile oferite nu au " << num_intrari << " elemente. Reconfigurati sau oferiti numarul corect de valori!" << endl;
			return 1;
		}
		LSTMLayer* layer = LSTMLayer::fromJson(fileHandler.readFromFile("LSTMWeights.json"));
		vector<vector<double>> matrixNormalizat(vec.size());
		for (int i = 0; i < vec.size(); i++)
			matrixNormalizat[i] = envData->getNormalisation()->Normalise(vec[i]);
		vector<vector<double>> data_in(vec[0].size(), std::vector<double>(vec.size(), 0.0));
		for (int j = 0; j < vec.size(); j++)
			for (int i = 0; i < vec[0].size(); i++)
				data_in[i][j] = matrixNormalizat[j][i];
		double out = 0;
		//merge si cu mai multe hidden states dar avem pt actiuni doar 1
		for(int i=0; i<vec[0].size(); i++)
			out = layer->ForwardPass(data_in[i])[0];
		nlohmann::json jsonData;
		jsonData["rezultat"] = envData->getNormalisation()->Denormalise(vec[IDX_PREDICTED_FEATURE],out);
		fileHandler.writeToFile("output.json",jsonData.dump(4));
	}
	return 0;
}