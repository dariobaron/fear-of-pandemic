#ifndef distribution_h
#define distribution_h

#include <iostream>
#include <iomanip>

using namespace std;


template<typename Distr,typename Engine>
class Distribution{
protected:
	Distr distribution_;
	Engine * random_engine_;

public:
	Distribution(Distr distribution, Engine * random_engine) : distribution_(distribution),
	random_engine_(random_engine) {};
	Distribution() {};
	~Distribution(){};
	auto operator()(){
		return distribution_(*random_engine_);
	};
};


#endif