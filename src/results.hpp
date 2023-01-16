#ifndef results_h
#define results_h

#include <fstream>
#include <vector>
#include "epidemic.hpp"


class Results{

protected:
	std::vector<double> t_;
	std::vector<unsigned int> S_;
	std::vector<unsigned int> E_;
	std::vector<unsigned int> I_;
	std::vector<unsigned int> R_;
	std::vector<unsigned int> D_;

public:
	Results(const Epidemic & epidemic) : t_(epidemic.gett()), S_(epidemic.getS()),
	E_(epidemic.getE()), I_(epidemic.getI()), R_(epidemic.getR()), D_(epidemic.getD()) {};
	~Results() {};
	
	void writeData(std::ostream & os){
		os << "t S E I R D" << std::endl;
		for (auto i = 0; i < t_.size(); ++i){
			os << t_[i] << " " << S_[i] << " " << E_[i] << " ";
			os << I_[i] << " " << R_[i] << " " << D_[i] << std::endl;
		}
	}	
};

#endif