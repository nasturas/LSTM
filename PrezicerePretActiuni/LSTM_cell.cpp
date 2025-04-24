#include "LSTM_cell.h"
#include "Test_Vector.h"
#include "EnvironmentData.h"
#include <string>
#include <iostream>
using namespace std;
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

//functia de calcul al erorilor prin porti si a erorii aparute la iesirea fiecarui pas.
// folosim https://medium.com/@aidangomez/let-s-do-this-f9b699de31d9
// t_post e in loc de t+1, t_ante e in loc de t-1
// TODO: codu poate fi imbunatatit ca lizibilitate daca facem o clasa matrice cu operatii de inmultire si adunare.. nu stiu cat de mult ajuta si la executie
std::vector<double> LSTM_cell::BackwardPass(Gradient* out_grd_gates, std::vector<double> expected, const Cell* const cell, const Cell* const cell_ante, const Cell* const cell_post, std::vector<double> delta_out_post)
{
	std::vector<double> delta(num_unit_ascuns, 0.0);
	std::vector<double> grd_out(num_unit_ascuns, 0.0);
	std::vector<double> grd_state(num_unit_ascuns, 0.0);
	std::vector<double> grd_ag(num_unit_ascuns, 0.0);
	std::vector<double> grd_ig(num_unit_ascuns, 0.0);
	std::vector<double> grd_fg(num_unit_ascuns, 0.0);
	std::vector<double> grd_og(num_unit_ascuns, 0.0);
	std::vector<double> grd_out_ante(num_unit_ascuns, 0.0);
#ifdef STACKED_LSTM
	//lasam pentru extindere si la cazul de LSTM stacked.
	std::vector<double> grd_x;
#endif


	for (int i = 0; i < num_unit_ascuns; i++)
	{
		//delta este diferenta dintre iesirea din LSTM si valoarea corecta din vectorul de invatare.
		delta[i] = cell->out[i] - expected[i];
		//grd_out este suma dintre delta si delta_out calculat din pasul viitor.
		grd_out[i] = delta[i] + delta_out_post[i];
		
		grd_state[i] = grd_out[i] * cell->og[i] * (1 - tanh->Output(cell->state[i]) * tanh->Output(cell->state[i])) + cell_post->state[i] * cell_post->fg[i];
		grd_ag[i] = grd_state[i] * cell->ig[i] * (1 - cell->ag[i] * cell->ag[i]);
		grd_ig[i] = grd_state[i] * cell->ag[i] * cell->ig[i] * (1 - cell->ig[i]);
		grd_fg[i] = grd_state[i] * cell_ante->state[i] * cell->fg[i] * (1 - cell->fg[i]);
		grd_og[i] = grd_out[i] * tanh->Output(cell->state[i]) * cell->og[i] * (1 - cell->og[i]);
		grd_out_ante[i] = 0;
		for (int j = 0; j < num_unit_ascuns; j++)
		{
			grd_out_ante[i] += grd_ag[i] * Ua[i][j] + grd_ig[i] * Ui[i][j] + grd_fg[i] * Uf[i][j] + grd_og[i] * Uo[i][j];
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


/*
* Luam din input care e un vector cu toate valorile actiunii de la inchiderea bursei pentru fiecare zi.
* Il vom imparti in 80% vor fi date pentru antrenament, 20% pentru testat.
* Noi antrenam AI sa poata estima valoarea actiunii luand in calcul evolutia pretului in ultimele x zile. Aceste x zile alcatuiesc o fereastra de antrenament.
* Putem sa suprapunem ferestrele sau nu. Alegem sa suprapunem 50% din fereastra (TRAINING_WINDOW_OVERLAY). Putem sa imbunatatim acest parametru.
* 
* Aici e doar o trecere pentru o epoca.
*/
void LSTM_cell::TrainLSTM(std::vector<Test_Vector> test_vect, double lambda)
{
	std::vector<Cell> gates;
	std::vector<Gradient> grd_gates;
	std::vector<std::vector<double>> grd_Wa(num_unit_ascuns, std::vector<double>(num_intrari, 0.0));
	std::vector<std::vector<double>> grd_Wf(num_unit_ascuns, std::vector<double>(num_intrari, 0.0));
	std::vector<std::vector<double>> grd_Wi(num_unit_ascuns, std::vector<double>(num_intrari, 0.0));
	std::vector<std::vector<double>> grd_Wo(num_unit_ascuns, std::vector<double>(num_intrari, 0.0));

	std::vector<std::vector<double>> grd_Ua(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));
	std::vector<std::vector<double>> grd_Uf(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));
	std::vector<std::vector<double>> grd_Ui(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));
	std::vector<std::vector<double>> grd_Uo(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));

	std::vector<double> grd_ba(num_unit_ascuns, 0.0);
	std::vector<double> grd_bf(num_unit_ascuns, 0.0);
	std::vector<double> grd_bi(num_unit_ascuns, 0.0);
	std::vector<double> grd_bo(num_unit_ascuns, 0.0);

	std::vector<double> delta_out;

	Cell* gate_post = new Cell(num_unit_ascuns);
	Cell* gate_ante;
	
	int T = (int)test_vect.size();
	
	//intai facem forward un T numar de pasi
	gates.resize(T);
	grd_gates.resize(T);
	for (int t = 0; t < T; t++)
	{
		(void)ForwardPass(test_vect[t].get_Test_Vector());
		gates[t].resize(num_unit_ascuns);
		grd_gates[t].resize(num_unit_ascuns);
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			gates[t].ag[i] = cell_gates->ag[i];
			gates[t].fg[i] = cell_gates->fg[i];
			gates[t].ig[i] = cell_gates->ig[i];
			gates[t].og[i] = cell_gates->og[i];
			gates[t].state[i] = cell_gates->state[i];
			gates[t].out[i] = cell_gates->out[i];
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
		if (t == 0) 
		{
			gate_ante = new Cell(num_unit_ascuns);
		}
		else
		{
			gate_ante = &gates[t - 1];
		}
		delta_out = BackwardPass(&grd_gates[t], test_vect[t].get_Rezultat_Vector(), &gates[t], gate_ante, gate_post, delta_out);
		
		if (t == 0)
		{
			delete gate_ante;
		}
		else
		{
			if (t == T - 1)
			{
				delete gate_post;
			}
		}
		gate_post = &gates[t];
		
	}
	//calculam gradient pentru weights
	//formula e grd_wa = Sum ( grd_a * x ). grd_a e un vector de dimensiune num_unit_ascuns, x e un vector de dimensiune num_intrari 
	for (int t = 0; t < T; t++)
	{
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			for (int j = 0; j < num_intrari; j++)
			{
				grd_Wa[i][j] += grd_gates[t].grd_ag[i] * test_vect[t].get_Test_Elem(j);
				grd_Wf[i][j] += grd_gates[t].grd_fg[i] * test_vect[t].get_Test_Elem(j);
				grd_Wi[i][j] += grd_gates[t].grd_ig[i] * test_vect[t].get_Test_Elem(j);
				grd_Wo[i][j] += grd_gates[t].grd_og[i] * test_vect[t].get_Test_Elem(j);
			}
			grd_ba[i] += grd_gates[t].grd_ag[i];
			grd_bf[i] += grd_gates[t].grd_fg[i];
			grd_bi[i] += grd_gates[t].grd_ig[i];
			grd_bo[i] += grd_gates[t].grd_og[i];
		}
	}

	for (int t = 0; t < T-1; t++)
	{
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			for (int j = 0; j < num_unit_ascuns; j++)
			{
				grd_Ua[i][j] += grd_gates[t+1].grd_ag[i] * gates[t].out[j];
				grd_Uf[i][j] += grd_gates[t+1].grd_fg[i] * gates[t].out[j];
				grd_Ui[i][j] += grd_gates[t+1].grd_ig[i] * gates[t].out[j];
				grd_Uo[i][j] += grd_gates[t+1].grd_og[i] * gates[t].out[j];
			}
		}
	}
	//ajustam cu viteza de invatare - lambda
	for (int i = 0; i < num_unit_ascuns; i++)
	{
		for (int j = 0; j < num_intrari; j++)
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

/**
* @brief Functie care primeste un vector de valori, si il imparte in training set si test set dupa regula 70%-30%
*/
void LSTM_cell::PrepareTraining(std::vector<double> in_set, std::vector<Test_Vector>* training_set, std::vector<Test_Vector>* test_set, int stride)
{
	Test_Vector tmp_set;
	//verificam daca marimea setului e destul de mare.
	if (num_intrari*2+num_unit_ascuns > in_set.size() * 30 / 100)
	{
		throw std::invalid_argument("Marimea setului de date este prea mica! Incearca sa dai un set de date de cel putin "+ std::to_string((num_intrari * 2 + num_unit_ascuns)*10/3+1));
	}
	
	// calculam numarul de elemente care ar fi 70%
	int num_elem_70_procent = (int)in_set.size() * 70 / 100;
	// sliding window size e numarul_intrari
	// vrem sa vedem care e indexul mai apropiat de acest 70% dupa ce il impartitm.
	int num_vector_antr = (int)(num_elem_70_procent - num_intrari) / stride+1;
	// avem un set de numar de intrari elemente, si fiidnca se suprapun se avanseaza cum stride de acum. se scade 1 ca indexu porneste de la 0.
	int idx_ultim = (num_vector_antr - 1) * stride + num_intrari - 1;

	
	//cum aflu care e indexul de pornire la test set, ca multiplu de num_intrari.
	int idx_test_vector_start = idx_ultim + num_unit_ascuns;

	// populam vectorii de iesire. (scadem cu 1 ca sa transformam numaru in index pornit de la 0)
	for (int i = 0; i + num_intrari + num_unit_ascuns-1 <= idx_ultim; i += stride)
	{
		//punem un vector de test cu iesire cu tot.
		tmp_set.assign_Test_Elem(in_set.begin() + i, in_set.begin() + i + num_intrari);
		tmp_set.assign_Rezultat_Elem(in_set.begin() + i + num_intrari, in_set.begin() + i + num_intrari + num_unit_ascuns);
		training_set->push_back(tmp_set);
	}

	for (int i = idx_test_vector_start; i + num_intrari + num_unit_ascuns < in_set.size(); i += stride)
	{
		tmp_set.assign_Test_Elem(in_set.begin() + i, in_set.begin() + i + num_intrari);
		tmp_set.assign_Rezultat_Elem(in_set.begin() + i + num_intrari, in_set.begin() + i + num_intrari + num_unit_ascuns);
		test_set->push_back(tmp_set);
	}

}

/**
* @brief Functie care antreneaza reteaua
* antrenamentul se opreste cand numarul de antrenament atingi un numar maxim - masura de siguranta ca nu rulam la infinit
* sau cand aproximarea e mai buna decat 95% din valoarea dorita.
*/
void LSTM_cell::Train(std::vector<double> in_set, int stride, double lambda)
{
	std::vector<Test_Vector> train_set;
	std::vector<Test_Vector> test_set;
	double error = 0;
	double eroare_tinta = 0;
	int num_antrenari = 0;
	EnvironmentData* env_data = EnvironmentData::getInstance(0,0,0);

	try {
		PrepareTraining(in_set, &train_set, &test_set, stride);
		eroare_tinta = CalcEroareTinta(&test_set);
		cout << "eroarea tinta e " << eroare_tinta << endl;
		do {
			//while(i < nr_max_epoc && error scade - sau nu creste de 3 ori? consec) 
			TrainLSTM(train_set, lambda);

			//acum testam daca aceasta epoca de antrenament a fost indeajuns.
			error = TestLSTM(&test_set);
			num_antrenari++;
			cout << error << endl;
		} while (num_antrenari < env_data->getNumarMaximAntrenari() && error > eroare_tinta);

		cout << "Am iesit din antrenament cu eroarea " << error<<" dupa numar de antrenament: "<<num_antrenari<<endl;

	}
	catch (const invalid_argument e)
	{
		throw;
	}
}

/**
* @brief Functie care testeaza daca dupa o epoca de antrenament celula LSTM merge si pentru setul de test
* returneaza eroare ca double
* eroarea totala e media sumelor patratelor erorilor fiecarui set. 
*/
double LSTM_cell::TestLSTM(std::vector<Test_Vector>* test_set)
{
	double error = 0.0;
	double error_pas = 0.0;
	double err_i = 0.0;
	
	for(Test_Vector v : *test_set)
	{
		vector<double> out = ForwardPass(v.get_Test_Vector());
		//facem media patratului erorii pe iesiri
		for (int i=0; i < v.get_Dim_Rezultat(); i++)
		{
			err_i = v.get_Rezultat_Elem(i) - out[i];
			error_pas += err_i * err_i;
		}
		error_pas /= v.get_Dim_Rezultat();
		error_pas = sqrt(error_pas);
			//adaugam eroarea pasului la eroarea generala
		error += error_pas * error_pas;
	}
	error /= test_set->size();
	error = sqrt(error);
	
	return error;
}

double LSTM_cell::CalcEroareTinta(std::vector<Test_Vector>* test_set)
{
	double error = 0.0;
	double error_pas = 0.0;
	double err_i = 0.0;
	EnvironmentData* envData = EnvironmentData::getInstance(0, 0,0);

	for (Test_Vector v : *test_set)
	{
		//calculam eroarea pentru o estimare care ar fi procent_precizie_antrenament% din valoarea reala
		for (int i = 0; i < v.get_Dim_Rezultat(); i++)
		{
			err_i = v.get_Rezultat_Elem(i) * envData->getProcentPrecizieAntrenament() / 100;
			error_pas += err_i * err_i;
		}
		error_pas /= v.get_Dim_Rezultat();
		error_pas = sqrt(error_pas);
		//adaugam eroarea pasului la eroarea generala
		error += error_pas * error_pas;
	}
	error /= test_set->size();
	error = sqrt(error);

	return error;
}