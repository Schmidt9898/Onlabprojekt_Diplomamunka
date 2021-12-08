import matplotlib.pyplot as plt

class Colum:
    def __init__(self,name,init_value):
        self.value=float(init_value)
        self.N=1
    def avg(self):
        return self.value/self.N
    def add(self,val):
        self.value+=float(val)
        self.N+=1

class Meres:
    def __init__(self,name_,path):
        self.name=name_
        self.Colums={}
        self.Read_meres(path)

    def Read_meres(self,path):
        lines=[]
        try:
            with open(path) as f:
                lines = f.readlines()
        except IOError:
            print("File: "+ path +" not accessible.")
        finally:
            f.close()

        
        #make colums lines
        for i in range(len(lines)):
            if lines[i]=="END\n":#skip these rows
                continue
            #print(lines[i])
            temp = lines[i].split(':')
            if temp[0] in self.Colums.keys():
                self.Colums[temp[0]].add(temp[1])
            else:
                self.Colums[temp[0]]=Colum(temp[0],temp[1])
        print(len(self.Colums))

    def Plot(self):
        labels=list(self.Colums.keys())
        sections = list(self.Colums.values())
        values=[]
        for i in range(len(sections)):
            values.append(sections[i].avg())
        

        print(values)
        fig,ax = plt.subplots(figsize = (8,6))
        plt.title(self.name)
        plt.ylabel("Time in secounds")
        plt.xlabel("Section")
        plt.bar(labels,values)
#[:-1]
        for index,data in enumerate(values):
            plt.text(x=index-0.3 , y =data+0.01 , s=f"{round(data,2)}" , fontdict=dict(fontsize=20)  )      
        plt.savefig('képek/'+self.name+'.png')
        #plt.show()
    


#Main starts here
import grafok  # ez lefogja generálni a képet

M_32_t=Meres("32 magos CPU","meresek/32_thread_viscoacoustic_cpu_parallel.meres.txt")
M_32_t.Plot()
M_1_t=Meres("1 magos CPU","meresek/1_thread_viscoacoustic_cpu_parallel.meres.txt")
M_1_t.Plot()
M_8_t=Meres("8 magos CPU","meresek/8_thread_viscoacoustic_cpu_parallel.meres.txt")
M_8_t.Plot()


M_cpu=Meres("Szekvenciális lefutás","meresek/viscoacoustic_cpu.txt")
M_cpu.Plot()
M_cpu_simd=Meres("Szekvenciális vektorizált lefutás","meresek/viscoacoustic_cpu_simd.txt")
M_cpu_simd.Plot()

#gpus
M_gpu_o=Meres("Tesla K80 GPU","meresek/viscoacoustic_gpu_old.meres.txt")
M_gpu_o.Plot()
M_gpu_v100=Meres("Tesla V100 GPU","meresek/viscoacoustic_gpu_v100.meres.txt")
M_gpu_v100.Plot()


labels= ["Cpu","Cpu_simd","8 szál","32 szál","K80 gpu","V100 gpu"]
values= [M_cpu.Colums["Time loop"].avg(),M_cpu_simd.Colums["Time loop"].avg(),M_8_t.Colums["Time loop"].avg(),M_32_t.Colums["Time loop"].avg(),M_gpu_o.Colums["Time loop"].avg(),M_gpu_v100.Colums["Time loop"].avg()]


print(values)
fig,ax = plt.subplots(figsize = (8,6))
plt.title("Time loops")
plt.ylabel("Time in secounds")
plt.xlabel("Device")
plt.bar(labels,values,color=['blue', 'royalblue', 'green', 'lime', 'darkred','red'])

for index,data in enumerate(values):
    plt.text(x=index-0.3 , y =data+0.01 , s=f"{round(data,2)}" , fontdict=dict(fontsize=20)  )      
plt.savefig('képek/Timeloop.png')

#labels= ["Cpu","Cpu_simd","8 thread","32 thread","gpu","v100 gpu"]
labels= ["Cpu","Cpu_simd","8 szál","32 szál","K80 gpu","V100 gpu"]
values= [M_cpu.Colums["Forward"].avg(),M_cpu_simd.Colums["Forward"].avg(),M_8_t.Colums["Forward"].avg(),M_32_t.Colums["Forward"].avg(),M_gpu_o.Colums["Forward"].avg(),M_gpu_v100.Colums["Forward"].avg()]


print(values)
fig,ax = plt.subplots(figsize = (8,6))
plt.title("Forward")
plt.ylabel("Time in secounds")
plt.xlabel("Device")
plt.bar(labels,values,color=['blue', 'royalblue', 'green', 'lime', 'darkred','red'])

for index,data in enumerate(values):
    plt.text(x=index-0.3 , y =data+0.01 , s=f"{round(data,2)}" , fontdict=dict(fontsize=20)  )      
plt.savefig('képek/Forward.png')





plt.show()