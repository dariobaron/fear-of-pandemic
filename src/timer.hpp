#ifndef timer_h
#define timer_h

#include <chrono>
#include <ctime>
#include <vector>

namespace chrono = std::chrono;

template<typename UoM = chrono::seconds, typename ClockType = chrono::steady_clock>
class Timer{
public:
	Timer() : starting_point(ClockType::now()), laps({starting_point}) {}
	void restart(){
		starting_point = ClockType::now();
		laps = {starting_point};
	}
	auto lap(){
		auto newlap = ClockType::now();
		auto interval = (chrono::duration_cast<UoM>( newlap - laps.back() )).count();
		laps.push_back(newlap);
		return interval;
	}
	auto stop(){
		end_point = ClockType::now();
		laps.push_back(end_point);
		return (chrono::duration_cast<UoM>( end_point - starting_point )).count();
	}
protected:
	chrono::time_point<ClockType> starting_point;
	chrono::time_point<ClockType> end_point;
	std::vector<chrono::time_point<ClockType>> laps;
};


#endif