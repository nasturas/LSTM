#pragma once
#include <vector>
#include "Interpolare.h"
#include "XavierNormalised.h"
#include "Test_Vector.h"
class LSTM_cell
{
private:
	//numarul de unitati ascunse este si numarul de iesiri.
	int num_intrari, num_unit_ascuns;
	class Cell
	{
		
	public:
		Cell() {}
		Cell(int nrAsc) { 
			fg.resize(nrAsc);
			ig.resize(nrAsc);
			og.resize(nrAsc);
			out.resize(nrAsc);
			ag.resize(nrAsc);
			state.resize(nrAsc);

			for (int i = 0; i < nrAsc; i++)
			{
				fg[i] = 0;
				ig[i] = 0;
				og[i] = 0;
				out[i] = 0;
				ag[i] = 0;
				state[i] = 0;
			}
		}

		~Cell() {}

	
		//o coloana de num_unit_ascuns randuri, sau num_unit_ascuns x 1
		std::vector<double> fg;
		std::vector<double> ig;
		std::vector<double> og;
		std::vector<double> out;
		std::vector<double> ag;
		std::vector<double> state;

	};
	class Gradient
	{
	public:
		Gradient(){}
		Gradient(int nrAsc)
		{
			grd_state.resize(nrAsc);
			grd_fg.resize(nrAsc);
			grd_ig.resize(nrAsc);
			grd_og.resize(nrAsc);
			grd_ag.resize(nrAsc);
		};

		std::vector<double> grd_ag;
		std::vector<double> grd_fg;
		std::vector<double> grd_ig;
		std::vector<double> grd_og;
		std::vector<double> grd_state;
	};
	Cell* cell_gates;
	// matrici cu num_unit_ascuns randuri si num_intrari coloane. sau num_unit_ascuns x num_intrari
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

	Interpolare* sigmoid;
	Interpolare* tanh;
	XavierNormalised* num_generator;
public:
	LSTM_cell() 
	{
		num_intrari = 0;
		num_unit_ascuns = 0;

		num_generator = new XavierNormalised(0,0);
		sigmoid = Interpolare::getSigmoid(); tanh = Interpolare::getTanh();
		cell_gates = new Cell();
	}
	LSTM_cell(int nrIn, int nrAsc) : num_intrari(nrIn), num_unit_ascuns(nrAsc)
	{
		cell_gates = new Cell(nrAsc);
	
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
		
		sigmoid = Interpolare::getSigmoid(); tanh = Interpolare::getTanh();
	}
	//TODO: de adaugat o initializare cu weights din vreun fisier salvat local ceva.


	std::vector<double> ForwardPass(std::vector<double> input);
	std::vector<double> BackwardPass(Gradient* out_grd_gates, std::vector<double> expected, const Cell* const cell, const Cell* const cell_ante, const Cell* const cell_post, std::vector<double> delta_out_post);
	void TrainLSTM(std::vector<Test_Vector> test_vect, double lambda);
	// Functie care primeste un vector de valori, si il imparte in training set si test set dupa regula 70%-30%
	void PrepareTraining(std::vector<double> in_set, std::vector<Test_Vector>* training_set, std::vector<Test_Vector>* test_set, int stride);
	void Train(std::vector<double> in_set, int stride, double lambda);
	double TestLSTM(std::vector<Test_Vector>* test_set);
};


