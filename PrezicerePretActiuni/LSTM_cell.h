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
			forget_gate.reserve(nrAsc);
			input_gate.reserve(nrAsc);
			output_gate_activation.reserve(nrAsc);
			output_gate.reserve(nrAsc);
			cell_input.reserve(nrAsc);
			cell_state.reserve(nrAsc);
		}

		~Cell() {}

	
		//o coloana de num_unit_ascuns randuri, sau nume_unit_ascuns x 1
		std::vector<double> forget_gate;
		std::vector<double> input_gate;
		std::vector<double> output_gate_activation;
		std::vector<double> output_gate;
		std::vector<double> cell_input;
		std::vector<double> cell_state;

	};
	class Gradient
	{
	public:
		Gradient(){}
		Gradient(int nrAsc)
		{
			grd_cell_state.reserve(nrAsc);
			grd_forget_gate.reserve(nrAsc);
			grd_input_gate.reserve(nrAsc);
			grd_output_gate_activation.reserve(nrAsc);
		};


		std::vector<double> grd_forget_gate;
		std::vector<double> grd_input_gate;
		std::vector<double> grd_output_gate_activation;
		std::vector<double> grd_cell_state;
	};
	Cell* cell_gates;
	// matrici cu num_unit_ascuns randuri si num_intrari coloane. sau num_unit_ascuns x num_intrari
	std::vector<std::vector<double>> forget_input_weight; 
	std::vector<std::vector<double>> input_weight; 
	std::vector<std::vector<double>> output_input_weight; 
	std::vector<std::vector<double>> cell_input_weight; 
	// matrici de dimensiune num_unit_ascuns x num_unit_ascuns 
	std::vector<std::vector<double>> forget_hidden_weight; 
	std::vector<std::vector<double>> input_hidden_weight; 
	std::vector<std::vector<double>> output_hidden_weight; 
	std::vector<std::vector<double>> cell_input_hidden_weight;
	// o coloana de num_unit_ascuns randuri
	std::vector<double> bias_forget;
	std::vector<double> bias_input;
	std::vector<double> bias_output;
	std::vector<double> bias_cell_input;

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
		forget_input_weight.reserve(nrAsc); 
		input_weight.reserve(nrAsc); 
		output_input_weight.reserve(nrAsc); 
		cell_input_weight.reserve(nrAsc); 

		forget_hidden_weight.reserve(nrAsc); 
		input_hidden_weight.reserve(nrAsc); 
		output_hidden_weight.reserve(nrAsc); 
		cell_input_hidden_weight.reserve(nrAsc);
		for (int i = 0; i < nrAsc; i++)
		{

			forget_input_weight[i].reserve(nrIn);
			input_weight[i].reserve(nrIn);
			output_input_weight[i].reserve(nrIn);
			cell_input_weight[i].reserve(nrIn);

			forget_hidden_weight[i].reserve(nrAsc);
			input_hidden_weight[i].reserve(nrAsc);
			output_hidden_weight[i].reserve(nrAsc);
			cell_input_hidden_weight[i].reserve(nrAsc);
		}
		bias_forget.reserve(nrAsc);
		bias_input.reserve(nrAsc);
		bias_output.reserve(nrAsc);
		bias_cell_input.reserve(nrAsc);

		sigmoid = Interpolare::getSigmoid(); tanh = Interpolare::getTanh();
	}
	std::vector<double> ForwardPass(std::vector<double> input);
	std::vector<double> BackwardPass(Gradient* out_grd_gates, Cell* cell_state_t, std::vector<double> input_T, std::vector<double> expected, std::vector<double> delta_out_T, std::vector<double> grd_state_t_post, std::vector<double> f_t_post, std::vector<double> cell_state_ante);
	void TrainLSTM(std::vector<double> input, int window_size);
};

