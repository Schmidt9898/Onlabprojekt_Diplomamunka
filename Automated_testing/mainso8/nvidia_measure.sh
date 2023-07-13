#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --gres=gpu:0
#SBATCH --job-name=MEASURE
#SBATCH --output=openmp_800_nvidia_measurement.%j.out
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

python3 ./measure.py fornaiv
touch naive_is_done
python3 ./measure.py forblocked
touch blocked_is_done
python3 ./measure.py fortilled
touch tilled_is_done
python3 ./measure.py forblocktilled
touch forblocktilled_is_done

date

echo "done"