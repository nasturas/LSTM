#pragma once
#include <vector>
#include "Interpolare.h"

// cat sa suprapunem din ferestre de antrenament. in procente
#define TRAINING_WINDOW_OVERLAY 50

class LSTM_cell
{
private:
	int num_intrari, num_unit_ascuns;
	class Cell
	{
		
	public:
		Cell() {}
		Cell(int nrAsc) { 
			//TODO: initializeaza tot cu 0.
			fg.reserve(nrAsc);
			ig.reserve(nrAsc);
			og.reserve(nrAsc);
			out.reserve(nrAsc);
			ag.reserve(nrAsc);
			state.reserve(nrAsc);
		}

		~Cell() {}

	
		//o coloana de num_unit_ascuns randuri, sau nume_unit_ascuns x 1
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
			grd_state.reserve(nrAsc);
			grd_fg.reserve(nrAsc);
			grd_ig.reserve(nrAsc);
			grd_og.reserve(nrAsc);
			grd_ag.reserve(nrAsc);
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

public:
	LSTM_cell() 
	{
		num_intrari = 0;
		num_unit_ascuns = 0;
		sigmoid = Interpolare::getSigmoid(); tanh = Interpolare::getTanh();
		cell_gates = new Cell();
	}
	LSTM_cell(int nrIn, int nrAsc) : num_intrari(nrIn), num_unit_ascuns(nrAsc)
	{
		cell_gates = new Cell(nrAsc);
		// la matrici alocam spatiu pentru cate randuri avem nevoie
		Wf.reserve(nrAsc); 
		Wi.reserve(nrAsc); 
		Wo.reserve(nrAsc); 
		Wa.reserve(nrAsc); 

		Uf.reserve(nrAsc); 
		Ui.reserve(nrAsc); 
		Uo.reserve(nrAsc); 
		Ua.reserve(nrAsc);
		for (int i = 0; i < nrAsc; i++)
		{

			Wf[i].reserve(nrIn);
			Wi[i].reserve(nrIn);
			Wo[i].reserve(nrIn);
			Wa[i].reserve(nrIn);

			Uf[i].reserve(nrAsc);
			Ui[i].reserve(nrAsc);
			Uo[i].reserve(nrAsc);
			Ua[i].reserve(nrAsc);
		}
		bf.reserve(nrAsc);
		bi.reserve(nrAsc);
		bo.reserve(nrAsc);
		ba.reserve(nrAsc);
		//TODO: initializeaza weights cu niste valori random. De vazut ce valori bune de init pot fi.

		sigmoid = Interpolare::getSigmoid(); tanh = Interpolare::getTanh();
	}
	std::vector<double> ForwardPass(std::vector<double> input);
	std::vector<double> BackwardPass(Gradient* out_grd_gates, std::vector<double> expected, Cell* cell, Cell* cell_ante, Cell* cell_post, std::vector<double> delta_out_post);
	void TrainLSTM(std::vector<std::vector<double>> x, std::vector<std::vector<double>> expected, int window_size, int lambda);
};


