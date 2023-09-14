#!/bin/bash
echo "hello from intel"
echo $(pwd)
source /home/u192928/python_env/bin/activate
cd /home/u192928/Onlabprojekt_Diplomamunka/Automated_testing/mainso8
python ./measure.py forblocked
echo $(pwd)