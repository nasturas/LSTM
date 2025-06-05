#include "LSTMLayer.h"
#include "EnvironmentData.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;
//executa intrarea si intoarce un output
std::vector<double> LSTMLayer::ForwardPass(std::vector<double> x)
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
		for (int k = 0; k < num_feature; k++)
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

		cell_gates->fg_p[i] = temp_forget;
		cell_gates->ig_p[i] = temp_input;
		cell_gates->og_p[i] = temp_output;
		cell_gates->ag_p[i] = temp_activare;

		cell_gates->fg[i] = sigmoid->Output(temp_forget);
		cell_gates->ig[i] = sigmoid->Output(temp_input);
		cell_gates->og[i] = sigmoid->Output(temp_output);
		cell_gates->ag[i] = tanh->Output(temp_activare);


			
		cell_gates->state[i] = cell_gates->fg[i] * cell_gates->state[i] + cell_gates->ig[i] * cell_gates->ag[i];
		if (cell_gates->state[i] > 0)
			cell_gates->out[i] = cell_gates->og[i] * tanh->Output(cell_gates->state[i]);
		else
			cell_gates->out[i] = 0;

	}
	return cell_gates->out;
}

/*
* Luam din input care e un vector cu toate valorile actiunii de la inchiderea bursei pentru fiecare zi.
* Il vom imparti in 80% vor fi date pentru antrenament, 20% pentru testat.
* Noi antrenam AI sa poata estima valoarea actiunii luand in calcul evolutia pretului in ultimele x zile. Aceste x zile alcatuiesc o fereastra de antrenament.
* Putem sa suprapunem ferestrele sau nu. Alegem sa suprapunem 50% din fereastra (TRAINING_WINDOW_OVERLAY). Putem sa imbunatatim acest parametru.
* 
* Aici e doar o trecere pentru o epoca.
*/
void LSTMLayer::TrainLSTM(Test_Vector test_vect, double lambda)
{

	std::vector<std::vector<double>> grd_Wa(num_unit_ascuns, std::vector<double>(num_feature, 0.0));
	std::vector<std::vector<double>> grd_Wf(num_unit_ascuns, std::vector<double>(num_feature, 0.0));
	std::vector<std::vector<double>> grd_Wi(num_unit_ascuns, std::vector<double>(num_feature, 0.0));
	std::vector<std::vector<double>> grd_Wo(num_unit_ascuns, std::vector<double>(num_feature, 0.0));

	std::vector<std::vector<double>> grd_Ua(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));
	std::vector<std::vector<double>> grd_Uf(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));
	std::vector<std::vector<double>> grd_Ui(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));
	std::vector<std::vector<double>> grd_Uo(num_unit_ascuns, std::vector<double>(num_unit_ascuns, 0.0));

	std::vector<double> grd_ba(num_unit_ascuns, 0.0);
	std::vector<double> grd_bf(num_unit_ascuns, 0.0);
	std::vector<double> grd_bi(num_unit_ascuns, 0.0);
	std::vector<double> grd_bo(num_unit_ascuns, 0.0);

	const ILossFunction* lossFunction = EnvironmentData::getInstance(0,0,1,DataNormalisationStyle::Logaritm,LossFunctionStyle::MSE)->GetLossFunction();

	vector<vector<double>> delta_loss(num_intrari);

	int T = (int)test_vect.get_Dim_Test();
	
	//intai facem forward un T numar de pasi
	gates = new vector<LSTMCell>;
	gates->resize(T);
	for (int t = 0; t < T; t++)
	{
		(void)ForwardPass(test_vect.get_Test_Vector()[t]);
		(*gates)[t].resize(num_unit_ascuns);
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			(*gates)[t].ag[i] = cell_gates->ag[i];
			(*gates)[t].fg[i] = cell_gates->fg[i];
			(*gates)[t].ig[i] = cell_gates->ig[i];
			(*gates)[t].og[i] = cell_gates->og[i];
			(*gates)[t].state[i] = cell_gates->state[i];
			(*gates)[t].out[i] = cell_gates->out[i];
		}
		delta_loss[t] = lossFunction->GetLossDerivate(test_vect.get_Rezultat_Vector(), cell_gates->out);
		
	}

	vector<LstmGradients> grd_gates = BackwardPass(delta_loss);
	//calculam gradient pentru weights
	//formula e grd_wa = Sum ( grd_a * x ). grd_a e un vector de dimensiune num_unit_ascuns, x e un vector de dimensiune num_feature 
	for (int t = 0; t < T; t++)
	{
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			for (int j = 0; j < num_feature; j++)
			{
				grd_Wa[i][j] += grd_gates[t].grd_ag_p[i] * test_vect.get_Test_Elem(t)[j];
				grd_Wf[i][j] += grd_gates[t].grd_fg_p[i] * test_vect.get_Test_Elem(t)[j];
				grd_Wi[i][j] += grd_gates[t].grd_ig_p[i] * test_vect.get_Test_Elem(t)[j];
				grd_Wo[i][j] += grd_gates[t].grd_og_p[i] * test_vect.get_Test_Elem(t)[j];
			}
			grd_ba[i] += grd_gates[t].grd_ag_p[i];
			grd_bf[i] += grd_gates[t].grd_fg_p[i];
			grd_bi[i] += grd_gates[t].grd_ig_p[i];
			grd_bo[i] += grd_gates[t].grd_og_p[i];
		}
	}

	for (int t = 0; t < T-1; t++)
	{
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			for (int j = 0; j < num_unit_ascuns; j++)
			{
				grd_Ua[i][j] += grd_gates[t+1].grd_ag_p[i] * (*gates)[t].out[j];
				grd_Uf[i][j] += grd_gates[t+1].grd_fg_p[i] * (*gates)[t].out[j];
				grd_Ui[i][j] += grd_gates[t+1].grd_ig_p[i] * (*gates)[t].out[j];
				grd_Uo[i][j] += grd_gates[t+1].grd_og_p[i] * (*gates)[t].out[j];
			}
		}
	}
	//ajustam cu viteza de invatare - lambda
	for (int i = 0; i < num_unit_ascuns; i++)
	{
		for (int j = 0; j < num_feature; j++)
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
void LSTMLayer::PrepareTraining(std::vector<std::vector<double>> in_set, std::vector<Test_Vector>* training_set, std::vector<Test_Vector>* test_set, int stride)
{
	Test_Vector tmp_set;
	//verificam daca marimea setului e destul de mare.
	if (num_intrari*2+num_unit_ascuns > in_set.size() * 30 / 100)
	{
		throw std::invalid_argument("Marimea setului de date este prea mica! Incearca sa dai un set de date de cel putin "+ std::to_string((num_feature * 2 + num_unit_ascuns)*10/3+1));
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
void LSTMLayer::Train(vector<vector<double>> in_set, int stride, double lambda)
{
	std::vector<Test_Vector> train_set;
	std::vector<Test_Vector> test_set;
	double error = 0;
	double eroare_tinta = 0;
	int num_antrenari = 0;
	EnvironmentData* env_data = EnvironmentData::getInstance(0,0,0, DataNormalisationStyle::Logaritm, LossFunctionStyle::MSE);

	try 
	{
		PrepareTraining(in_set, &train_set, &test_set, stride);
		eroare_tinta = CalcEroareTinta(&test_set);
		cout << "eroarea tinta e " << std::fixed << std::setprecision(5) << eroare_tinta << endl;
		do {
			for (Test_Vector tv : train_set)
			{
				TrainLSTM(tv, lambda);
			}
			//acum testam daca aceasta epoca de antrenament a fost indeajuns.
			error = TestLSTM(&test_set);
			num_antrenari++;
			cout << "epoca:"<<num_antrenari<<"loss: "<< error << endl;
		} while (num_antrenari < env_data->getNumarMaximAntrenari()  && error > eroare_tinta);

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
double LSTMLayer::TestLSTM(std::vector<Test_Vector>* test_set)
{
	double error = 0.0;

	vector<vector<double>> predicted_out;
	vector<vector<double>> true_out;
	const ILossFunction* lossFunction = EnvironmentData::getInstance(0,0,0,DataNormalisationStyle::Logaritm,LossFunctionStyle::MSE)->GetLossFunction();
	vector<double> out;
	for(Test_Vector v : *test_set)
	{
		for(int t=0; t<num_intrari;t++)
			out = ForwardPass(v.get_Test_Vector()[t]);
		predicted_out.push_back(out);
		true_out.push_back(v.get_Rezultat_Vector());
	}
	error = lossFunction->GetLoss(true_out, predicted_out);

	return error;
}

double LSTMLayer::CalcEroareTinta(std::vector<Test_Vector>* test_set)
{
	double error = 0.0;
	EnvironmentData* envData = EnvironmentData::getInstance(0, 0,0, DataNormalisationStyle::Logaritm, LossFunctionStyle::MSE);
	double procent_error = (100 - envData->getProcentPrecizieAntrenament()) / 100.0;
	for (Test_Vector tc : (*test_set))
	{
		for (double r : tc.get_Rezultat_Vector())
		{
			error += r * procent_error*r * procent_error;
		}
	}
	error = error / ((*test_set).size() * (*test_set)[0].get_Dim_Rezultat());

	return error;
}

vector<LstmGradients> LSTMLayer::BackwardPass(vector<vector<double>> &deltaLoss)
{

	vector<LstmGradients> gradients(num_intrari);
	vector<double> grd_out(num_unit_ascuns, 0.0);

#ifdef STACKED_LSTM
	//lasam pentru extindere si la cazul de LSTM stacked.
	std::vector<double> grd_x;
#endif
	
	for (int t = num_intrari-1; t >=0; t--)
	{
		gradients[t].resize(num_unit_ascuns);
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			grd_out[i] += deltaLoss[t][i];
		}
		if (t < num_intrari-1)
		{
			//transpusa matricei U.
			for (int j = 0; j < num_unit_ascuns; j++)
			{
				for (int i = 0; i < num_unit_ascuns; i++)
				{
					grd_out[i] += Ua[j][i] * sigmoid ->Derivate((*gates)[t + 1].ag_p[j]) * gradients[t + 1].grd_ag_p[j];
					grd_out[i] += Ui[j][i] * sigmoid->Derivate((*gates)[t + 1].ig_p[j]) * gradients[t+1].grd_ig_p[j];
					grd_out[i] += Uf[j][i] * sigmoid->Derivate((*gates)[t + 1].fg_p[j]) * gradients[t+1].grd_fg_p[j];
					grd_out[i] += Uo[j][i] * sigmoid->Derivate((*gates)[t + 1].og_p[j]) * gradients[t+1].grd_og_p[j];
				}
			}
		}
		
		for (int i = 0; i < num_unit_ascuns; i++)
		{
			gradients[t].grd_og_p[i] = grd_out[i] * tanh->Output((*gates)[t].state[i]) * sigmoid->Derivate((*gates)[t].og_p[i]);
			gradients[t].grd_state[i] = grd_out[i] * (*gates)[t].og[i] * tanh->Derivate((*gates)[t].state[i]);
			if (t > 0)
				gradients[t].grd_fg_p[i] = gradients[t].grd_state[i] * (*gates)[t - 1].state[i] * sigmoid->Derivate((*gates)[t].fg_p[i]);
			else
				gradients[t].grd_fg_p[i] = 0;
			gradients[t].grd_ig_p[i] = gradients[t].grd_state[i] * (*gates)[t].ag[i] * sigmoid->Derivate((*gates)[t].ig_p[i]);
			gradients[t].grd_ag_p[i] = gradients[t].grd_state[i] * (*gates)[t].ig[i] * tanh->Derivate((*gates)[t].ag_p[i]);
		}
	}
	
	return gradients;
}
