#pragma once
#include <vector>
class GateGradient
{
public:
	GateGradient() {}
	GateGradient(int nrAsc)
	{
		grd_state.resize(nrAsc);
		grd_fg.resize(nrAsc);
		grd_ig.resize(nrAsc);
		grd_og.resize(nrAsc);
		grd_ag.resize(nrAsc);
	};
	void resize(int nrAsc)
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
