#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --gres=gpu:0
#SBATCH --job-name=cudade80
#SBATCH --output=800_CUDA_decomposed4d_measurement.%j.out

#SBATCH --time=11:00:00


#module purge
echo "800_CUDA_decomposed4d_measurement"
echo "init modules"

date

source /usr/share/Modules/init/bash

source /home/schmidtl/Onlabprojekt/start 15.3

source /home/schmidtl/Onlabprojekt/py_venv/bin/activate
which python3


cd /home/schmidtl/Onlabprojekt/Automated_testing/cuda_test_decomposed



export CUDA_VISIBLE_DEVICES=0

pwd

nvidia-smi

export SM_VAL='sm_80'

python3 ./execute.py

date

echo "done"