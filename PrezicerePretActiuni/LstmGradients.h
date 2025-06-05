#pragma once
#include <vector>
class LstmGradients
{
public:
	std::vector<double> grd_ag_p;
	std::vector<double> grd_fg_p;
	std::vector<double> grd_ig_p;
	std::vector<double> grd_og_p;
	std::vector<double> grd_state;

	LstmGradients() {}
	LstmGradients(int nrAsc)
	{
		grd_state.resize(nrAsc);
		grd_fg_p.resize(nrAsc);
		grd_ig_p.resize(nrAsc);
		grd_og_p.resize(nrAsc);
		grd_ag_p.resize(nrAsc);
	}
	void resize(int nrAsc)
	{
		grd_state.resize(nrAsc);
		grd_fg_p.resize(nrAsc);
		grd_ig_p.resize(nrAsc);
		grd_og_p.resize(nrAsc);
		grd_ag_p.resize(nrAsc);
	};
};

