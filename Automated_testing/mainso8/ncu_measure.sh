#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --gres=gpu:0
#SBATCH --job-name=NCU_MEAS
#SBATCH --output=ncu_measurement.%j.out
#SBATCH --time=11:00:00

#module purge

date

source /usr/share/Modules/init/bash


cd /home/schmidtl/Onlabprojekt

source init_workspace

which python3

cd /home/schmidtl/Onlabprojekt/Automated_testing/mainso8


export CUDA_VISIBLE_DEVICES=0

pwd

nvidia-smi

python3 profile.py

date

echo "done"