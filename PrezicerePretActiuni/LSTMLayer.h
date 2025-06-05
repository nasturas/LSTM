#pragma once
#include <vector>
#include "XavierNormalised.h"
#include "Test_Vector.h"
#include "LSTMCell.h"
#include "GateGradient.h"
#include "LstmGradients.h"
#include "Sigmoid.h"
#include "Tanh.h"

class LSTMLayer
{
private:
	//numarul de unitati ascunse este si numarul de iesiri.
	int num_feature, num_unit_ascuns, num_intrari;
	LSTMCell* cell_gates;
	// matrici cu num_unit_ascuns randuri si num_feature coloane. sau num_unit_ascuns x num_feature
	std::vector<std::vector<double>> Wf; 
	std::vector<std::vector<double>> Wi; 
	std::vector<std::vector<double>> Wo; 
	std::vector<std::vector<double>> Wa; 
	// matrici de dimensiune num_unit_ascuns x num_unit_ascuns 
	std::vector<std::vector<double>> Uf; 
	std::vector<std::vector<double>> Ui; 
	std::vector<std::vector<double>> Uo; 
	std::vector<std::vector<double>> Ua;
	// o coloana de num_unit_ascuns randuri
	std::vector<double> bf;
	std::vector<double> bi;
	std::vector<double> bo;
	std::vector<double> ba;

	vector<LSTMCell>* gates;

	IActivationFunction* sigmoid;
	IActivationFunction* tanh;
	XavierNormalised* num_generator;

	
	double CalcEroareTinta(std::vector<Test_Vector>* test_set);
	
	
	
public:
	LSTMLayer() 
	{
		num_feature = 0;
		num_unit_ascuns = 0;
		num_intrari = 0;
		gates = nullptr;

		num_generator = new XavierNormalised(0,0);
		sigmoid = new Sigmoid(); tanh = new Tanh();
		cell_gates = new LSTMCell();
	}
	LSTMLayer(int nrFe, int nrAsc, int nrIn) : num_feature(nrFe), num_unit_ascuns(nrAsc), num_intrari(nrIn)
	{
		cell_gates = new LSTMCell(nrAsc);
		gates = nullptr;
		num_generator = new XavierNormalised(nrIn, nrAsc);
		//initializam ponderile la porti cu generatorul de numere ales. Initial folosim doar Xavier normalizat
		//https://machinelearningmastery.com/weight-initialization-for-deep-learning-neural-networks/
		// https://arxiv.org/abs/1912.10454 - "On the Initialization of Long Short-Term Memory Networks" de M. Ghazi et al.
		//TODO: de creat diferite functii de initializare pentru LSTM, xavier, normalised xavier, orthogonal initialization. we will use normalised xavier.

		Wf = num_generator->generateMatrix(nrAsc, nrIn);
		Wi = num_generator->generateMatrix(nrAsc, nrIn);
		Wo = num_generator->generateMatrix(nrAsc, nrIn);
		Wa = num_generator->generateMatrix(nrAsc, nrIn);


		Uf = num_generator->generateMatrix(nrAsc, nrAsc);
		Ui = num_generator->generateMatrix(nrAsc, nrAsc);
		Uo = num_generator->generateMatrix(nrAsc, nrAsc);
		Ua = num_generator->generateMatrix(nrAsc, nrAsc);

		bf = num_generator->generateVector(nrAsc);
		bi = num_generator->generateVector(nrAsc);
		bo = num_generator->generateVector(nrAsc);
		ba = num_generator->generateVector(nrAsc);
		
		sigmoid = new Sigmoid(); tanh = new Tanh();
	}
	


	std::vector<double> ForwardPass(std::vector<double> input);
	void Train(vector<vector<double>> in_set, int stride, double lambda);
	// Functie care primeste un vector de valori, si il imparte in training set si test set dupa regula 70%-30%
	void PrepareTraining(std::vector<std::vector<double>> in_set, std::vector<Test_Vector>* training_set, std::vector<Test_Vector>* test_set, int stride);

	void TrainLSTM(Test_Vector test_vect, double lambda);
	double TestLSTM(std::vector<Test_Vector>* test_set);
	vector<LstmGradients> BackwardPass(vector<vector<double>>& deltaLoss);
}; 


