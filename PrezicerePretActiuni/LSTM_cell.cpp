#include "LSTM_cell.h"
//executa intrarea si intoarce un output
std::vector<double> LSTM_cell::ForwardPass(std::vector<double> x)
{
	double temp_forget;
	double temp_input;
	double temp_output;
	double temp_activare;
	for (int i = 0; i < num_unit_ascuns; i++)
	{
		temp_forget = 0;
		temp_input = 0;
		temp_output = 0;
		temp_activare = 0;
		for (int k = 0; k < num_intrari; k++)
		{
			temp_forget += Wf[i][k] * x[k];
			temp_input += Wi[i][k] * x[k];
			temp_output += Wo[i][k] * x[k];
			temp_activare += Wa[i][k] * x[k];
		}
		for (int n = 0; n < num_unit_ascuns; n++)
		{
			temp_forget += Uf[i][n] * cell_gates->out[n];
			temp_input += Ui[i][n] * cell_gates->out[n];
			temp_output += Uo[i][n] * cell_gates->out[n];
			temp_activare += Ua[i][n] * cell_gates->out[n];
		}
		temp_forget += bf[i];
		temp_input += bi[i];
		temp_output += bo[i];
		temp_activare += ba[i];

		cell_gates->fg[i] = sigmoid->Output(temp_forget);
		cell_gates->ig[i] = sigmoid->Output(temp_input);
		cell_gates->og[i] = sigmoid->Output(temp_output);
		cell_gates->ag[i] = tanh->Output(temp_activare);



		cell_gates->state[i] = cell_gates->fg[i] * cell_gates->state[i] + cell_gates->ig[i] * cell_gates->ag[i];
		cell_gates->out[i] = cell_gates->og[i] * tanh->Output(cell_gates->state[i]);

	}
	return cell_gates->out;
}

/*
* Luam din input care e un vector cu toate valorile actiunii de la inchiderea bursei pentru fiecare zi.
* Il vom imparti in 80% vor fi date pentru antrenament, 20% pentru testat.
* Noi antrenam AI sa poata estima valoarea actiunii luand in calcul evolutia pretului in ultimele x zile. Aceste x zile alcatuiesc o fereastra de antrenament.
* Putem sa suprapunem ferestrele sau nu. Alegem sa suprapunem 50% din fereastra (TRAINING_WINDOW_OVERLAY). Putem sa imbunatatim acest parametru.
*/
void LSTM_cell::TrainLSTM(std::vector<std::vector<double>> x, std::vector<std::vector<double>> expected, int window_size, int lambda)
{
	std::vector<Cell*> gates;
	std::vector<Gradient*> grd_gates;
	std::vector<std::vector<double>> grd_Wa;
	std::vector<std::vector<double>> grd_Wf;
	std::vector<std::vector<double>> grd_Wi;
	std::vector<std::vector<double>> grd_Wo;

	std::vector<std::vector<double>> grd_Ua;
	std::vector<std::vector<double>> grd_Uf;
	std::vector<std::vector<double>> grd_Ui;
	std::vector<std::vector<double>> grd_Uo;

	std::vector<double> grd_ba;
	std::vector<double> grd_bf;
	std::vector<double> grd_bi;
	std::vector<double> grd_bo;

	std::vector<double> delta_out;

	Cell* gate_post = new Cell(num_unit_ascuns);
	Cell* gate_ante;

	int T=window_size;
	//aranjam ferestrele de antrenament.
	//TODO: ales aici de unde sa inceapa fereastra, refacut in jos algoritmu luand in calcul acest lucru, plus conditii de stop.
	//intai facem forward un T numar de pasi
	gates.resize(T);
	grd_gates.resize(T);
	for (int t = 0; t < T; t++)
	{
		(void)ForwardPass(x[t]);
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			gates[t]->ag[i] = cell_gates->ag[i];
			gates[t]->fg[i] = cell_gates->fg[i];
			gates[t]->ig[i] = cell_gates->ig[i];
			gates[t]->og[i] = cell_gates->og[i];
			gates[t]->state[i] = cell_gates->state[i];
			gates[t]->out[i] = cell_gates->out[i];
		}
	}
	//apoi calculam grd inapoi.
	delta_out.resize(num_unit_ascuns);
	for (int i = 0; i < num_unit_ascuns; i++)
	{
		delta_out[i] = 0;
	}
	
	for (int t = T-1; t >=0; t--)
	{
		//TODO: rescriem BackwardPass ca sa scrie in argument nu in return value pentru delta_out.
		//TODO: ai grija de pointeri.
		if (t == 0) 
		{
			gate_ante = new Cell(num_unit_ascuns);
		}
		else
		{
			gate_ante = gates[t - 1];
		}
		delta_out = BackwardPass(grd_gates[t], expected[t], gates[t], gate_ante, gate_post, delta_out);
		gate_post = gates[t];
		
	}
	//calculam gradient pentru weights
	//formula e grd_wa = Sum ( grd_a * x ). grd_a e un vector de dimensiune num_unit_ascuns, x e un vector de dimensiune num_intrari 
	for (int t = 0; t < T; t++)
	{
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			for (int j = 0; i < num_intrari; j++)
			{
				grd_Wa[i][j] += grd_gates[t]->grd_ag[i] * x[t][j];
				grd_Wf[i][j] += grd_gates[t]->grd_fg[i] * x[t][j];
				grd_Wi[i][j] += grd_gates[t]->grd_ig[i] * x[t][j];
				grd_Wo[i][j] += grd_gates[t]->grd_og[i] * x[t][j];
			}
			grd_ba[i] += grd_gates[t]->grd_ag[i];
			grd_bf[i] += grd_gates[t]->grd_fg[i];
			grd_bi[i] += grd_gates[t]->grd_ig[i];
			grd_bo[i] += grd_gates[t]->grd_og[i];
		}
	}

	for (int t = 0; t < T-1; t++)
	{
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			for (int j = 0; i < num_unit_ascuns; j++)
			{
				grd_Ua[i][j] += grd_gates[t+1]->grd_ag[i] * gates[t]->out[j];
				grd_Uf[i][j] += grd_gates[t+1]->grd_fg[i] * gates[t]->out[j];
				grd_Ui[i][j] += grd_gates[t+1]->grd_ig[i] * gates[t]->out[j];
				grd_Uo[i][j] += grd_gates[t+1]->grd_og[i] * gates[t]->out[j];
			}
		}
	}
	//ajustam cu viteza de invatare - lambda
	for (int i = 0; i < num_unit_ascuns; i++)
	{
		for (int j = 0; i < num_intrari; j++)
		{
			Wa[i][j] -= lambda * grd_Wa[i][j];
			Wf[i][j] -= lambda * grd_Wf[i][j];
			Wi[i][j] -= lambda * grd_Wi[i][j];
			Wo[i][j] -= lambda * grd_Wo[i][j];
		}
		for(int j = 0; j< num_unit_ascuns; j++)
		{
			Ua[i][j] -= lambda * grd_Ua[i][j];
			Uf[i][j] -= lambda * grd_Uf[i][j];
			Ui[i][j] -= lambda * grd_Ui[i][j];
			Uo[i][j] -= lambda * grd_Uo[i][j];
		}
		ba[i] -= lambda * grd_ba[i];
		bf[i] -= lambda * grd_bf[i];
		bi[i] -= lambda * grd_bi[i];
		bo[i] -= lambda * grd_bo[i];

	}
}

//functia de calcul al erorilor prin porti si a erorii aparute la iesirea fiecarui pas.
// folosim https://medium.com/@aidangomez/let-s-do-this-f9b699de31d9
// t_post e in loc de t+1, t_ante e in loc de t-1
// TODO: codu poate fi imbunatatit ca lizibilitate daca face o clasa matrice cu operatii de inmultire si adunare.. nu stiu cat de mult ajuta si la executie
std::vector<double> LSTM_cell::BackwardPass(Gradient * out_grd_gates, std::vector<double> expected, Cell* cell,  Cell* cell_ante, Cell* cell_post, std::vector<double> delta_out_post)
{
	std::vector<double> delta;
	std::vector<double> grd_out;
	std::vector<double> grd_state;
	std::vector<double> grd_ag;
	std::vector<double> grd_ig;
	std::vector<double> grd_fg;
	std::vector<double> grd_og;
	std::vector<double> grd_out_ante;
#ifdef STACKED_LSTM
//lasam pentru extindere si la cazul de LSTM stacked.
	std::vector<double> grd_x;
#endif
	Gradient* grd_gates = new Gradient(num_unit_ascuns);

	for (int i = 0; i < num_unit_ascuns; i++)
	{
		//delta este diferenta dintre iesirea din LSTM si valoarea corecta din vectorul de invatare.
		delta[i] = cell->out[i] - expected[i];
		//grd_out este suma dintre delta si delta_out calculat din pasul viitor.
		grd_out[i] = delta[i] + delta_out_post[i];
		//TODO: reverifica calculele la grd.
		grd_state[i] = grd_out[i] * cell->og[i] * (1 - tanh->Output(cell->state[i])* tanh->Output(cell->state[i]))+cell_post->state[i]*cell_post->fg[i];
		grd_ag[i] = grd_state[i] * cell->ig[i] * (1 - cell->ag[i] * cell->ag[i]);
		grd_ig[i] = grd_state[i] * cell->ag[i] * cell->ig[i] * (1 - cell->ig[i]);
		grd_fg[i] = grd_state[i] * cell_ante->state[i] * cell->fg[i] * (1 - cell->fg[i]);
		grd_og[i] = grd_out[i] * tanh->Output(cell->state[i]) * cell->og[i] * (1 - cell->og[i]);
		grd_out_ante[i] = 0;
		for (int j = 0; j < num_unit_ascuns; j++)
		{
			grd_out_ante[i]+= grd_ag[i] * Ua[i][j] + grd_ig[i] * Wi[i][j] + grd_fg[i] * Wf[i][j] + grd_og[i] * Wo[i][j];
		}
	}
	
#ifdef STACKED_LSTM 
	for (int i = 0; i < num_intrari; i++)
	{
		grd_x[i] = 0;
		for (int j = 0; j < num_unit_ascuns; j++)
		{
			grd_x[i] += grd_ag[j] * Wa[j][i] + grd_ig[j] * Wi[j][i] + grd_fg[j] * Wf[j][i] + grd_og[j] * Wo[j][i];
		}
		
	}
#endif

	out_grd_gates->grd_ag = grd_ag;
	out_grd_gates->grd_fg = grd_fg;
	out_grd_gates->grd_ig = grd_ig;
	out_grd_gates->grd_og = grd_og;
	out_grd_gates->grd_state = grd_state;
	return grd_out_ante;
}