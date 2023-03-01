#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --gres=gpu:0
#SBATCH --job-name=OMPBLOCK
#SBATCH --output=openmp_800_blocked_measurement.%j.out

#SBATCH --time=11:00:00

#module purge
echo "openmp_800_blocked_measurement"
echo "init modules"

date

source /usr/share/Modules/init/bash

source /home/schmidtl/Onlabprojekt/start 15.3

source /home/schmidtl/Onlabprojekt/py_venv/bin/activate
which python3


cd /home/schmidtl/Onlabprojekt/Automated_testing/openmp_block


export CUDA_VISIBLE_DEVICES=0

pwd

nvidia-smi

export SM_VAL='sm_70'

python3 ./execute_block_test.py

date

echo "done"