#ifndef utilities_h
#define utilities_h

#include <vector>
#include <numeric>


template<typename T>
std::vector<size_t> argsort(const std::vector<T> & v, bool ascending=true){
	std::vector<size_t> indices(v.size());
	std::iota(indices.begin(), indices.end(), 0);
	auto sorting = [&v](size_t idx1, size_t idx2){ return v[idx1] < v[idx2]; };
	std::sort(indices.begin(), indices.end(), sorting);
	if (ascending)	{	return indices;													}
	else 			{	return std::vector<size_t>(indices.rbegin(), indices.rend());	}
}


#endif