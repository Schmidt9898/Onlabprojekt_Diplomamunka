#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --gres=gpu:0
#SBATCH --job-name=cuda_800
#SBATCH --output=cuda_800_measurement.%j.out

#SBATCH --time=11:00:00


#module purge
echo "Cuda_800_measurement"
echo "init modules"

date

source /usr/share/Modules/init/bash

source /home/schmidtl/Onlabprojekt/start 15.3

source /home/schmidtl/Onlabprojekt/py_venv/bin/activate
which python3


cd /home/schmidtl/Onlabprojekt/Automated_testing/cuda_test



export CUDA_VISIBLE_DEVICES=0

pwd

nvidia-smi

export SM_VAL='sm_80'

python3 ./execute.py

date

echo "done"