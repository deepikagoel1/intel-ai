
#Remove old files 
rm -rf setupenv.sh.*

#Create conda env: 
conda create -n stock-tensorflow python matplotlib ipykernel psutil pandas gitpython

#Activate the created conda env: 
source activate stock-tensorflow

#Install stock Tensorflow with a specific version:
pip install tensorflow==2.3.0 cxxfilt 


#Deactivate conda env:
conda deactivate

#Register the kernel to Jupyter NB: 
~/.conda/envs/stock-tensorflow/bin/python -m ipykernel install --user --name=stock-tensorflow


#Source oneAPI environment variables: 
source /opt/intel/inteloneapi/setvars.sh

#Create conda env: 
conda create --name intel-tensorflow --clone tensorflow

#Activate the created conda env: 
source activate intel-tensorflow

#Install extra needed package:
pip install cxxfilt matplotlib ipykernel psutil pandas gitpython

#Deactivate conda env: 
conda deactivate

#Register the kernel to Jupyter NB: 
~/.conda/envs/intel-tensorflow/bin/python -m ipykernel install --user --name=intel-tensorflow
