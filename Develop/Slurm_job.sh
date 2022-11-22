#!/bin/bash
#SBATCH --nodelist=renyi
#SBATCH --ntasks=1
#SBATCH --job-name=forward_800
#SBATCH --output=800_forward_measurement.%j.out

#SBATCH --time=10:00:00



#module purge

echo "helo from slurm"
echo "init modules"
source /usr/share/Modules/init/bash


cd /home/schmidtl/Onlabprojekt/Develop/
source ../start 15.3

source /home/schmidtl/Onlabprojekt/py_venv/bin/activate
which python3

export CUDA_VISIBLE_DEVICES=1



pwd

nvidia-smi

python3 ./performance_test.py

echo "done"