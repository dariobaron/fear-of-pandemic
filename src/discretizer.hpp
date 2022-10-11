#ifndef discretizer_h
#define discretizer_h

class Discretizer{

protected:
	double timestep_;
	double beginning_;
	double t_;

public:
	Discretizer() : timestep_(1), beginning_(0), t_(0) {};
	Discretizer(double dt, double beg=0) : timestep_(dt), beginning_(beg), t_(beg) {};
	
	~Discretizer() {};

	double operator()(){
		t_ += timestep_;
		return t_;
	}
};

#endif