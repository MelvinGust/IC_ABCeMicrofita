import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import math
import statistics   
from mpl_toolkits.axes_grid1 import make_axes_locatable

# Define Add Colorbar function
def add_colorbar(mappable):
    last_axes = plt.gca()
    ax = mappable.axes
    fig = ax.figure
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)
    cbar = fig.colorbar(mappable, cax=cax)
    cbar.ax.tick_params(labelsize = 6)
    plt.sca(last_axes)
    return cbar

# PRECISO AVALIAR POR SEPARADO:
# 6: helical valley por ser com 3 entradas
def roots(z):
    return 1 / np.abs(z**6 - 1)

def helicalvalley(x, y, z):
    thetafunc = [0]*x
    for x_val in x:
        if (x_val>=0):
            thetafunc.append(((1.0)/(2.0*np.pi))*math.atan(y/x))
        else:
            thetafunc.append(((1.0)/(2.0*np.pi))*(math.atan(y/x)+np.pi))
    return (100)*((z-(10)*thetafunc)**2+(np.sqrt(x**2+y**2)-1)**2)+z**2

# Generate x and y values for the functions
x_1 = np.linspace(-10, 10, 50)
y_1 = np.linspace(-10, 10, 50)
z_hv = np.linspace(-10, 35, 50)
X_1, Y_1, Z_hv = np.meshgrid(x_1, y_1, z_hv)
# Z_hvgrid = np.outer(z_hv.T, z_hv)
x_2 = np.linspace(-2, 2, 100)
y_2 = np.linspace(-2, 2, 100)
X_2, Y_2 = np.meshgrid(x_2, y_2)
Z_roots = X_2 + 1j * Y_2


# Choose the functions you'll be evaluating
Z_1 = helicalvalley(x_1, y_1, z_hv)
Z_2 = roots(Z_roots)

# Create subplots
fig, axs = plt.subplot_mosaic([['a)', 'b)'], ['d)', 'd)'], ['d)','d)']], layout='tight')

# fourth dimention - colormap
color_dimension = Z_1 # change to desired fourth dimension
minn, maxx = color_dimension.min(), color_dimension.max()
norm = matplotlib.colors.Normalize(minn, maxx)
m = plt.cm.ScalarMappable(norm=norm, cmap='viridis')
m.set_array([])
fcolors = m.to_rgba(color_dimension)

fig1 = axs['a)'].plot_surface(X_1, Y_1, Z_hv, rstride=1, cstride=1, facecolors=fcolors, vmin=minn, vmax=maxx, shade=False)
axs['a)'].set_title('Função HelicalValley', fontsize = 7)
axs['a)'].tick_params(labelsize=7)
axs['a)'].set_xlabel('x', fontsize = 7)
axs['a)'].set_ylabel('y', fontsize = 7)
axs['a)'].set_zlabel('z', fontsize = 7)


fig2 = axs['b)'].contourf(X_2, Y_2, np.abs(Z_2), cmap='viridis', levels=50)
axs['b)'].set_title('Função Roots (|g(z)|)', fontsize = 7)
axs['b)'].tick_params(labelsize=7)
axs['b)'].set_xlabel('Re(z)', fontsize = 7)
axs['b)'].set_ylabel('Im(z)', fontsize = 7)
add_colorbar(fig2)

# Read points from the .txt file
file_path1 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/plots/ABCperformance6.txt"
file_path2 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/plots/ABCperformance29.txt"
max_iterations = 200
point_list1 = [[] for _ in range(max_iterations)] # Create list with 100 elements.
point_list2 = [[] for _ in range(max_iterations)]
stdev_points1 = []
stdev_points2 = []
mean_points1 = []
mean_points2 = []
function6_mean = 0 # HelicalValley: TO-DO
function29_mean = 1 #

with open(file_path1, 'r') as file1, open(file_path2, 'r') as file2:
    for line1, line2 in zip(file1, file2):
        x1, y1 = map(lambda v: int(v) if v.isdigit() else float(v), line1.strip().split())
        x2, y2 = map(lambda v: int(v) if v.isdigit() else float(v), line2.strip().split())
        # Lambda function declares value as an int if v.isdigit() returns true.
        point_list1[x1-1].append(y1)
        point_list2[x2-1].append(y2)

# MUDAR AQUI TAMBEM
for s1, s2 in zip(point_list1, point_list2):
    stdev_points1.append(statistics.stdev(s1, xbar = function6_mean))
    stdev_points2.append(statistics.stdev(s2, xbar = function29_mean))

x_coordinate = []
for x in range(len(stdev_points1)):
    x_coordinate.append(x)

# MUDAR OS LABELS
# Plot the Roots function on the right subplot, taking the full height
axs['d)'].plot(x_coordinate, stdev_points1, color = 'red', label = "Função HelicalValley" )
axs['d)'].plot(x_coordinate, stdev_points2, color = 'blue', linestyle = '--', label = "Função Roots")
axs['d)'].set_title('(σ x c) com numLaços = 100')
axs['d)'].set_xlabel('Ciclos de forrageamento (c)')
axs['d)'].set_ylabel('Desvio Padrão (σ)')
axs['d)'].legend()   

# plt.tight_layout()
plt.show()