#include "LSTM_cell.h"
//executa intrarea si intoarce un output
std::vector<double> LSTM_cell::ForwardPass(std::vector<double> input)
{
	//forget gate, input, output, cell input se calc in acelasi timp si la fel
	double temp_forget;
	double temp_input;
	double temp_output;
	double temp_cell_input;
	for (int i = 0; i < num_unit_ascuns; i++)
	{
		temp_forget = 0;
		temp_input = 0;
		temp_output = 0;
		temp_cell_input = 0;
		for (int k = 0; k < num_intrari; k++)
		{
			temp_forget += forget_input_weight[i][k] * input[k];
			temp_input += input_weight[i][k] * input[k];
			temp_output += output_input_weight[i][k] * input[k];
			temp_cell_input += cell_input_weight[i][k] * input[k];
		}
		for (int n = 0; n < num_unit_ascuns; n++)
		{
			temp_forget += forget_hidden_weight[i][n] * cell_gates->output_gate[n];
			temp_input += input_hidden_weight[i][n] * cell_gates->output_gate[n];
			temp_output += output_hidden_weight[i][n] * cell_gates->output_gate[n];
			temp_cell_input += cell_input_hidden_weight[i][n] * cell_gates->output_gate[n];
		}
		temp_forget += bias_forget[i];
		temp_input += bias_input[i];
		temp_output += bias_output[i];
		temp_cell_input += bias_cell_input[i];

		cell_gates->forget_gate[i] = sigmoid->Output(temp_forget);
		cell_gates->input_gate[i] = sigmoid->Output(temp_input);
		cell_gates->output_gate_activation[i] = sigmoid->Output(temp_output);
		cell_gates->cell_input[i] = tanh->Output(temp_cell_input);


		//acum produs elemente de pe aceasi pozitie
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			cell_gates->cell_state[i] = cell_gates->forget_gate[i] * cell_gates->cell_state[i] + cell_gates->input_gate[i] * cell_gates->cell_input[i];
			cell_gates->output_gate[i] = cell_gates->output_gate_activation[i] * tanh->Output(cell_gates->cell_state[i]);
		}

		return cell_gates->output_gate;
	}
}

/*
* Luam din input care e un vector cu toate valorile actiunii de la inchiderea bursei pentru fiecare zi.
* Il vom imparti in 80% vor fi date pentru antrenament, 20% pentru testat.
* Noi antrenam AI sa poata estima valoarea actiunii luand in calcul evolutia pretului in ultimele x zile. Aceste x zile alcatuiesc o fereastra de antrenament.
* Putem sa suprapunem ferestrele sau nu. Alegem sa suprapunem 50% din fereastra (TRAINING_WINDOW_OVERLAY). Putem sa imbunatatim acest parametru.
*/
void LSTM_cell::TrainLSTM(std::vector<double> input, int window_size)
{
	std::vector<Cell*> cell_states;

	//aranjam ferestrele de antrenament.

	//intai facem forward

	//apoi calculam grd inapoi.

}

//functia de baza pentru invatare
// folosim https://medium.com/@aidangomez/let-s-do-this-f9b699de31d9
// t_post e in loc de t+1, t_ante e in loc de t-1
// codu poate fi imbunatatit ca lizibilitate daca face o clasa matrice cu operatii de inmultire si adunare.. nu stiu cat de mult ajuta si la executie
std::vector<double> LSTM_cell::BackwardPass(Gradient * out_grd_gates,Cell* cell_state_t, std::vector<double> input_T, std::vector<double> expected, std::vector<double> delta_out_T, std::vector<double> grd_state_t_post, std::vector<double> f_t_post, std::vector<double> cell_state_ante)
{
	std::vector<double> delta_T;
	std::vector<double> delta_out_T_1;
	std::vector<double> grd_out_T;
	std::vector<double> grd_state_t;
	std::vector<double> grd_cell_input_t;
	std::vector<double> grd_input;
	std::vector<double> grd_output_activation;
	std::vector<double> grd_input;
	std::vector<double> grd_out_ante;

	Gradient* grd_gates = new Gradient(num_unit_ascuns);

	for (int i = 0; i < num_unit_ascuns; i++)
	{
		delta_T[i] = cell_state_t->output_gate[i] - expected[i];
		delta_out_T_1[i] = 0;
		grd_out_T[i] = delta_T[i] + delta_out_T[i];
		grd_state_t[i] = grd_out_T[i] * cell_state_t->output_gate[i] * (1 - tanh->Output(cell_state_t->cell_state[i])* tanh->Output(cell_state_t->cell_state[i]))+grd_state_t_post[i]*f_t_post[i];
		grd_cell_input_t[i] = grd_state_t[i] * input_T[i] * (1 - cell_state_t->cell_input[i] * cell_state_t->cell_input[i]);
		grd_input[i] = grd_state_t[i] * cell_state_t->cell_input[i] * input_T[i] * (1 - input_T[i]);
		grd_gates->grd_forget_gate[i] = grd_state_t[i] * cell_state_ante[i] * cell_state_t->forget_gate[i] * (1 - cell_state_t->forget_gate[i]);
		grd_output_activation[i] = grd_out_T[i] * tanh->Output(cell_state_t->cell_state[i]) * cell_state_t->output_gate_activation[i] * (1 - cell_state_t->output_gate_activation[i]);
		grd_out_ante[i] = 0;
		for (int j = 0; j < num_unit_ascuns; j++)
		{
			grd_out_ante[i]+= grd_cell_input_t[i] * cell_input_hidden_weight[i][j] + grd_input[i] * input_weight[i][j] + grd_gates->grd_forget_gate[i] * forget_input_weight[i][j] + grd_output_activation[i] * output_input_weight[i][j];
		}
	}
	for (int i = 0; i < num_intrari; i++)
	{
		grd_input[i] = 0;
		for (int j = 0; j < num_unit_ascuns; j++)
		{
			grd_input[i] += grd_cell_input_t[j] * cell_input_weight[j][i] + grd_input[j] * input_weight[j][i] + grd_gates->grd_forget_gate[j] * forget_input_weight[j][i] + grd_output_activation[j] * output_input_weight[j][i];
		}
		
	}
	out_grd_gates = grd_gates;
	return grd_out_ante;
}