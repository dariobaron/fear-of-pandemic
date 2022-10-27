#include <string> 
#include <vector>
#include <highfive/H5File.hpp>


struct Recorder{
    double time; int S; int E;
    int I; int R; int D;
    Recorder() : time(0), S(0), E(0), I(0), R(0), D(0) {}
    Recorder(double t, const vector<int> & SEIRD) : time(t) {
        S = SEIRD[0]; E = SEIRD[1]; I = SEIRD[2]; R = SEIRD[3]; D = SEIRD[4];
    }
};



HighFive::CompoundType create_compound() {
    return {
        {"time", HighFive::AtomicType<double>{}}, {"S", HighFive::AtomicType<int>{}},
        {"E", HighFive::AtomicType<int>{}}, {"I", HighFive::AtomicType<int>{}},
        {"R", HighFive::AtomicType<int>{}}, {"D", HighFive::AtomicType<int>{}},
    };
}

HIGHFIVE_REGISTER_TYPE(Recorder, create_compound)

void record_to_hdf5(const string filename, const string dataset_name, const vector<double> & time, const vector<vector<int>> & SEIRD){


    vector<Recorder> records(time.size());
    for (auto i = 0; i < time.size(); ++i){
        records[i] = Recorder(time[i], SEIRD[i]);
    }

    HighFive::File file(filename, HighFive::File::Truncate);

    auto t1 = create_compound();
    t1.commit(file, "Recorder");

    auto dataset = file.createDataSet(dataset_name, records);

}