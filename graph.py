import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.colors as col

plt.rcParams["animation.convert_path"] = "/usr/bin/convert"

fname = "1649176165_30_0.01_200"
data = np.loadtxt(fname+".txt",skiprows=0) #FICHERO DE MATRICES

color_dict = {0: '#2D4263', 1: '#C84B31'}
label_dict = {0: 'off', 1: 'on'}
imax = max(label_dict)
imin = min(label_dict)
cmap = col.ListedColormap(color_dict.values())

dim = int(len(data[0,:])) #DIMENSION DE LA MATRIZ
iter = int(len(data[:,0])/(dim)) #NUMERO DE ITERACIONES

#MATRIZ
grid = np.zeros((dim,dim))

def update(k):
    global grid
    newGrid = grid.copy()
    for i in range(dim):
        for j in range(dim):
            newGrid[i][j] = data[i+k*dim][j]
    mat.set_data(grid)
    grid = newGrid
    return [mat]

skip = 1

fig, ax = plt.subplots()
mat = ax.matshow(grid,cmap=cmap,interpolation='nearest',vmin=imin,vmax=imax)
ani = animation.FuncAnimation(fig, update, frames=np.arange(0, iter, skip), interval=1, repeat=False)

plt.title("Modelo de Ising $T = 0.01$")

plt.show()
ani.save(fname+".gif", writer=animation.PillowWriter(fps=30))

exit()

#ACTUALIZAMOS GRAFICA
def update(k):
    for i in range(dim):
        for j in range(dim):
            grid[i, j] = data[i+k*dim, j] #OBTENEMOS LOS VALORES NUEVOS PARA LA MATRIZ
    ax.matshow(grid,cmap=cmap,interpolation='nearest',vmin=imin, vmax=imax) #MOSTRAMOS LA MATRIZ COMO GRAFICA
    plt.savefig("/home/domi/cphys/ising/data/temp/"+"frame_"+str(k)+".png")
    return

skip = 1 #NUMERO DE DATOS A OMITIR EN CADA FRAME DEL GIF

fig, ax = plt.subplots()
ani = animation.FuncAnimation(fig, update, frames=np.arange(0, iter, skip), interval=1, repeat=False)

plt.title("Modelo de Ising")

plt.show()
#ani.save("/home/domi/cphys/ising/data/"+fname+".gif", writer=animation.FFMpegWriter)