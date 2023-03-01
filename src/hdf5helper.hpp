#ifndef hdf5helper_h
#define hdf5helper_h

#include <H5Cpp.h>


template<typename T>
auto getPredTypeFromCppVariable(T){}
template<>
auto getPredTypeFromCppVariable<char>(char)						{	return H5::PredType::C_S1;	}
template<>
auto getPredTypeFromCppVariable<int>(int)						{	return H5::PredType::NATIVE_INT;	}
template<>
auto getPredTypeFromCppVariable<long>(long)						{	return H5::PredType::NATIVE_LONG;	}
template<>
auto getPredTypeFromCppVariable<unsigned>(unsigned)				{	return H5::PredType::NATIVE_UINT;	}
template<>
auto getPredTypeFromCppVariable<unsigned long>(unsigned long)	{	return H5::PredType::NATIVE_ULONG;	}
template<>
auto getPredTypeFromCppVariable<float>(float)					{	return H5::PredType::NATIVE_FLOAT;	}
template<>
auto getPredTypeFromCppVariable<double>(double)					{	return H5::PredType::NATIVE_DOUBLE;	}


#endif