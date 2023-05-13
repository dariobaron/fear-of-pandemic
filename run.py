import os

os.chdir("src")
os.system("make")
os.chdir("..")

from params import *

output_dir = input_dir.replace("input", "output")
if not os.path.exists(output_dir):
	os.makedirs(output_dir)

simulations = [[reaction,fear,feedback] for reaction in reaction_type for fear in fear_distribution for feedback in feedback_type]

from multiprocessing import Pool, cpu_count, Lock
import subprocess

def process(sim_params):
	logfile = output_dir + "_".join(sim_params) + ".log"
	command = "src/simulation.x " + input_dir + " " + " ".join(sim_params)
	with open(logfile, 'w') as logs:
		exitstatus = subprocess.run(command, shell=True, stderr=logs)
	lock.acquire()
	if exitstatus.returncode == 0:
		print("Simulation " + "_".join(sim_params) + " completed!")
	else:
		print("Simulation " + "_".join(sim_params) + " encountered an ERROR!")
	lock.release()

lock = Lock()

N_cpu = cpu_count() - 1
with Pool(N_cpu) as pool:
	pool.map(process, simulations)