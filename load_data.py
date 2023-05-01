import numpy as np
import pandas as pd
import os
import h5py

def load_data(network_name):
	hyper_parameters = ["REACTION","FEAR","FEEDBACK"]
	hyper_par_values = [["soften","cut"],
						["fixed","uni","asyLow","asyHigh","bi","corr","anticorr"],
						["short","long","shortlong","neighbours","all"]]

	data_list = []
	for i,rea in enumerate(hyper_par_values[0]):
		data_fear = []
		for j,fea in enumerate(hyper_par_values[1]):
			data_feedback = []
			for k,fee in enumerate(hyper_par_values[2]):
				hpv = [rea,fea,fee]
				simulation_name = "_".join([hyper_parameters[i]+hpv[i] for i in range(len(hyper_parameters))]) + ".h5"
				filepath = os.path.join("output", network_name, simulation_name)
				h5file = h5py.File(filepath, 'r')
				df_run = {i : pd.DataFrame.from_records(np.array(h5file.get(str(i))), index="t") for i in range(100)}
				h5file.close()
				data_feedback.append(pd.concat(df_run, axis=1, join="outer"))
				print(f"Loading dataset: {(i*7*5+j*5+k)*100//(2*7*5)}%", flush=True, end="\r")
			data_feedback = pd.concat(data_feedback, keys=hyper_par_values[2], copy=False)
			data_fear.append(data_feedback)
		data_fear = pd.concat(data_fear, keys=hyper_par_values[1], copy=False)
		data_list.append(data_fear)
	data = pd.concat(data_list, keys=hyper_par_values[0], copy=False)
	data.rename_axis(index=["reaction","fear","feedback","t"], inplace=True)

	simulation_name_noFeedback = "_".join([hyper_parameters[i]+"none" for i in range(len(hyper_parameters))]) + ".h5"
	filepath = os.path.join("output", network_name, simulation_name_noFeedback)
	h5file = h5py.File(filepath, 'r')
	data_none = pd.concat({i : pd.DataFrame.from_records(np.array(h5file.get(str(i))), index="t") for i in range(100)}, axis=1, join="outer")
	h5file.close()
	data_none.index = pd.MultiIndex.from_product([["none"],["none"],["none"],data_none.index], names=["reaction","fear","feedback","t"])

	data = pd.concat([data_none,data])
	data.rename_axis(columns=["run","compartment"], inplace=True)

	print("Loading dataset: Completed!")

	return data
