#pragma once
#include <vector>
class LSTMCell
{

public:

	LSTMCell() {}
	LSTMCell(int nrAsc) {
		fg.resize(nrAsc);
		ig.resize(nrAsc);
		og.resize(nrAsc);
		out.resize(nrAsc);
		ag.resize(nrAsc);
		state.resize(nrAsc);
		fg_p.resize(nrAsc);
		ig_p.resize(nrAsc);
		og_p.resize(nrAsc);
		ag_p.resize(nrAsc);

		for (int i = 0; i < nrAsc; i++)
		{
			fg[i] = 0;
			ig[i] = 0;
			og[i] = 0;
			out[i] = 0;
			ag[i] = 0;
			state[i] = 0;
			fg_p[i] = 0;
			ig_p[i] = 0;
			og_p[i] = 0;
			ag_p[i] = 0;
		}
	}

	~LSTMCell() {}
	void resize(int nrAsc)
	{
		fg.resize(nrAsc);
		ig.resize(nrAsc);
		og.resize(nrAsc);
		out.resize(nrAsc);
		ag.resize(nrAsc);
		state.resize(nrAsc);
		fg_p.resize(nrAsc);
		ig_p.resize(nrAsc);
		og_p.resize(nrAsc);
		ag_p.resize(nrAsc);

		for (int i = 0; i < nrAsc; i++)
		{
			fg[i] = 0;
			ig[i] = 0;
			og[i] = 0;
			out[i] = 0;
			ag[i] = 0;
			state[i] = 0;
			fg_p[i] = 0;
			ig_p[i] = 0;
			og_p[i] = 0;
			ag_p[i] = 0;
		}
	}

	//o coloana de num_unit_ascuns randuri, sau num_unit_ascuns x 1
	std::vector<double> fg;
	std::vector<double> ig;
	std::vector<double> og;
	std::vector<double> out;
	std::vector<double> ag;
	std::vector<double> state;
	std::vector<double> fg_p;
	std::vector<double> ig_p;
	std::vector<double> og_p;
	std::vector<double> ag_p;

};
