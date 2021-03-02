import matplotlib.pyplot as plt


class Section:
    def __init__(self,t):
        self.t=float(t)
        self.n=1
    def avg(self):
        return self.t/self.n
    def add(self,val):
        self.t+=float(val)
        self.n+=1


def plot_this(filename):
    with open(filename) as f:
        lines = f.readlines()
    sections={}
    for i in range(len(lines)):
        if lines[i]=="END\n":
            continue
        #print(lines[i])
        temp = lines[i].split(':')
        if temp[0] in sections.keys():
            sections[temp[0]].add(temp[1])
        else:
            sections[temp[0]]=Section(temp[1])

    labels=list(sections.keys())
    sections = list(sections.values())
    values=[]
    for i in range(len(sections)):
        values.append(sections[i].avg())
        

    print(values[:-1])
    plt.bar(labels[:-1],values[:-1])





print("Diagram megjelenito.")

plot_this("..\\viscoacoustic_meres\\base.txt")
plot_this("..\\viscoacoustic_cpu_parallel\\cpu_parallel.txt")
    #sections[temp[0]]=float(temp[1])
#plt.bar([1,2,3,4],[1,2,3,4])
plt.show()
