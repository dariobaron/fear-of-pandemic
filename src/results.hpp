#ifndef results_h
#define results_h

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <H5Cpp.h>
#include "epidemic.hpp"
#include "hdf5helper.hpp"


class Results{

protected:
	H5::H5File file_;

public:
	Results(H5std_string filepath) : file_(filepath, H5F_ACC_TRUNC) {};
	~Results(){	file_.close();	};
	
	void createDataset(std::string name, const Epidemic & epidemic){
		std::vector<Epidemic::Record> data = epidemic.getRecords();
		// dataspace
		hsize_t dimension[] = {data.size()};
		H5::DataSpace dataspace(1, dimension);
		// datatype
		H5::CompType record_type(sizeof(Epidemic::Record));
		record_type.insertMember("t", HOFFSET(Epidemic::Record, t), H5::PredType::NATIVE_DOUBLE);
		record_type.insertMember("S", HOFFSET(Epidemic::Record, S), H5::PredType::NATIVE_UINT);
		record_type.insertMember("E", HOFFSET(Epidemic::Record, E), H5::PredType::NATIVE_UINT);
		record_type.insertMember("I", HOFFSET(Epidemic::Record, I), H5::PredType::NATIVE_UINT);
		record_type.insertMember("R", HOFFSET(Epidemic::Record, R), H5::PredType::NATIVE_UINT);
		record_type.insertMember("D", HOFFSET(Epidemic::Record, D), H5::PredType::NATIVE_UINT);
		// dataset
		H5::DataSet dataset = file_.createDataSet(name, record_type, dataspace);
		// writing data
		dataset.write(data.data(), record_type);
		// closing the dataset buffer
		dataset.close();
	};

	template<typename T>
	void setAttribute(std::string name, T attribute_value){
		auto attribute_type = getPredTypeFromCppVariable(attribute_value);
		H5::DataSpace attribute_space(H5S_SCALAR);
		H5::Attribute attribute = file_.createAttribute(name, attribute_type, attribute_space);
		attribute.write(attribute_type, &attribute_value);
	}

};


#endif