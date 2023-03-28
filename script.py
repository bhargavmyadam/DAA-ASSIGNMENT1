import matplotlib.pylab as plt
import sys

def generate_polygon(filename):
    with open(filename+".txt","r") as f:
        str = f.readline()
        m,n = str.split(" ")
        vertex_list = []
        for i in range(int(m)):
            x,y = (f.readline()).split(" ")
            x = float(x)
            y = float(y)
            vertex_list.append((x,y))
        edge_list = []
        for i in range(int(n)):
            u,v,x,y = (f.readline()).split(" ")
            u = float(u)
            v = float(v)
            x = float(x)
            y = float(y)
            edge_list.append(((u,v),(x,y)))

    figure, axis = plt.subplots()
    X_list = []
    Y_list = []
    for v1 in vertex_list:
        x1,y1 = v1
        X_list.append(x1)
        Y_list.append(y1)
    plt.plot(X_list,Y_list,'o',color = 'red')
    for v1,v2 in edge_list:
        x1,y1 = v1
        x2,y2 = v2
        axis.plot([x1,x2],[y1,y2],color = 'blue')
    axis.set_xlim(min([x[0] for x in vertex_list])-3,max([x[0] for x in vertex_list])+3)
    axis.set_ylim(min([y[1] for y in vertex_list])-3,max([y[1] for y in vertex_list])+3)  
    plt.savefig(filename + '.png')


generate_polygon("output1")
generate_polygon("output2")
generate_polygon("output3")