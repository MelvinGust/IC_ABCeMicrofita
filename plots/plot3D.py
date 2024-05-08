import matplotlib.pyplot as plt
import numpy as np
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
# 6: helical valley por ser com 3 entradas.
def ackley(x, y): #1
    return -20 * np.exp(-0.2 * np.sqrt(0.5 * (x**2 + y**2))) - \
           np.exp(0.5 * (np.cos(2 * np.pi * x) + np.cos(2 * np.pi * y))) + 20 + np.exp(1)

def sphere(x, y): #2
    return x**2 + y**2

def rosenbrock(x, y): #3
    return (1 - x)**2 + 100 * (y - x**2)**2

def rastrigin(x, y): #4
    return 20 + x**2 + y**2 - 10 * (np.cos(2 * np.pi * x) + np.cos(2 * np.pi * y))

def griewank(x, y): #5
    return (x**2 + y**2) / 4000 - np.cos(x) * np.cos(y / np.sqrt(2)) + 1

def helical_valley(x, y): #6 AVALIAR SEPARADO
    t = np.arctan2(y, x)
    z = np.linspace(0, 4, 100)
    return np.array([(x - 10 * t) ** 2 + (y - 10 * z) ** 2 for z in z]).min()

def beale(x, y): #7
    return (1.5 - x + x*y)**2 + (2.25 - x + x*y**2)**2 + (2.625 - x + x*y**3)**2

def bird(x, y): #8
    return np.sin(x) * np.exp((1 - np.cos(y))**2) + np.cos(y) * np.exp((1 - np.sin(x))**2) + (x - y)**2

def booth(x, y): #9
    return (x + 2*y - 7)**2 + (2*x + y - 5)**2

def bukin6(x, y): #10
    return 100 * np.sqrt(np.abs(y - 0.01 * x**2)) + 0.01 * np.abs(x + 10)

def carromtable(x, y): #11
    return (-1/30)*(np.cos(x)**2)*(np.cos(y)**2)*(np.exp(2*np.abs(1-np.sqrt(x**2+y**2)/np.pi)))

def chichinadze(x, y): #12
    return x**2 - 12*x + 11 + 10*np.cos(np.pi*x/2) + 8*np.sin(5*np.pi*x/2) - 0.2*np.sqrt(5) * np.exp(-((y - 0.5)**2)/2)

def crossintray(x, y): #13
    return -0.0001 * (np.abs(np.sin(x) * np.sin(y) * np.exp(np.abs(100 - np.sqrt(x**2 + y**2) / np.pi))) + 1)**0.1

def crosslegtable(x, y): #14
    return (-1)/(np.abs(np.sin(x) * np.sin(y) * np.exp(np.abs(100 - np.sqrt(x**2 + y**2) / np.pi)))+1)**0.1

def crownedcross(x, y): #15
    return 0.0001 * (np.abs(np.sin(x) * np.sin(y) * np.exp(np.abs(100 - np.sqrt(x**2 + y**2) / np.pi))) + 1)**0.1

def cube(x,y): #16
    return 100*(y-x**3)**2+(1+x)**2

def easom(x, y): #17
    return -np.cos(x) * np.cos(y) * np.exp(-(x - np.pi)**2 - (y - np.pi)**2)

def eggholder(x, y): #18
    return -(y + 47) * np.sin(np.sqrt(np.abs(y + x / 2 + 47))) - x * np.sin(np.sqrt(np.abs(x - (y + 47))))

def giunta(x, y): #19
    return 0.6+np.sin((16/15)*x-1)+np.sin((16/15)*y-1)+np.sin((16/15)*x-1)**2+np.sin((16/15)*y-1)**2+(1/50)*np.sin(4*((16/15)*x-1))+(1/50)*np.sin(4*((16/15)*x-1))

def goldsteinprice(x, y): #20
    term1 = 1 + (x + y + 1)**2 * (19 - 14*x + 3*x**2 - 14*y + 6*x*y + 3*y**2)
    term2 = 30 + (2*x - 3*y)**2 * (18 - 32*x + 12*x**2 + 48*y - 36*x*y + 27*y**2)
    return term1 * term2

def himmelblau(x, y): #21
    return (x**2 + y - 11)**2 + (x + y**2 - 7)**2

def holdertable(x, y): #22
    term1 = np.abs(1 - np.sqrt(x**2 + y**2) / np.pi)
    term2 = np.sin(x) * np.cos(y) * np.exp(term1)
    return -np.abs(term2)

def leon(x, y): #23
    return 100*(y-x**2)**2 + (1-x)**2   

def levy13(x, y): #24
    return np.sin(3*np.pi*x)**2 + (x-1)**2 * (1 + np.sin(3*np.pi*y)**2) + (y-1)**2 * (1 + np.sin(2*np.pi*y)**2)

def matyas(x, y): #25
    return 0.26 * (x**2 + y**2) - 0.48 * x * y

def mccormick(x, y): #26
    return np.sin(x + y) + (x - y)**2 - 1.5*x + 2.5*y + 1

def schaffer1(x, y): #27
    numerator = np.sin(x**2 - y**2) - 0.5
    denominator = (1 + 0.001*(x**2 + y**2))**2
    return 0.5 + numerator / denominator

def schaffer4(x, y): #28
    numerator = np.cos(np.sin(np.abs(x**2 - y**2)))**2 - 0.5
    denominator = (1 + 0.001*(x**2 + y**2))**2
    return 0.5 + numerator / denominator



# Generate x and y values for the functions
x_1 = np.linspace(-1.5, 4, 400)
y_1 = np.linspace(-3, 4, 400)
X_1, Y_1 = np.meshgrid(x_1, y_1)
x_2 = np.linspace(-100, 100, 1000)
y_2 = np.linspace(-100, 100, 1000)
X_2, Y_2 = np.meshgrid(x_2, y_2)
x_3 = np.linspace(-100, 100, 1000)
y_3 = np.linspace(-100, 100, 1000)
X_3, Y_3 = np.meshgrid(x_3, y_3)

# Choose the functions you'll be evaluating
Z_1 = mccormick(X_1, Y_1)
Z_2 = schaffer1(X_2, Y_2)
Z_3 = schaffer4(X_3, Y_3)

# Create subplots
fig, axs = plt.subplot_mosaic([['a)', 'b)', 'c)'], ['d)', 'd)','d)'], ['d)','d)','d)']], layout='tight')

# Plot the first function on the left top subplot
fig1 = axs['a)'].contourf(X_1, Y_1, Z_1, cmap='viridis', levels=50)
axs['a)'].set_title('Função McCormick', fontsize = 7)
axs['a)'].tick_params(labelsize=7)
axs['a)'].set_xlabel('x', fontsize = 7)
axs['a)'].set_ylabel('y', fontsize = 7)
add_colorbar(fig1)

# Plot the second function on the left middle subplot
fig2 = axs['b)'].contourf(X_2, Y_2, Z_2, cmap='viridis', levels=50)
axs['b)'].set_title('Função Schaffer1', fontsize = 7)
axs['b)'].tick_params(labelsize=7)
axs['b)'].set_xlabel('x', fontsize = 7)
axs['b)'].set_ylabel('y', fontsize = 7)
add_colorbar(fig2)

# Plot the third function on the left bottom subplot
fig3 = axs['c)'].contourf(X_3, Y_3, Z_3, cmap='viridis', levels=50)
axs['c)'].set_title('Função Schaffer4', fontsize = 7)
axs['c)'].tick_params(labelsize=7)
axs['c)'].set_xlabel('x', fontsize = 7)
axs['c)'].set_ylabel('y', fontsize = 7)
add_colorbar(fig3)

# Read points from the .txt file #TINHA 26, 27 E 28
file_path1 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/plots/ABCperformance10.txt"
file_path2 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/plots/ABCperformance14.txt"
file_path3 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/plots/ABCperformance28.txt"
max_iterations = 200
point_list1 = [[] for _ in range(max_iterations)] # Create list with 100 elements.
point_list2 = [[] for _ in range(max_iterations)]
point_list3 = [[] for _ in range(max_iterations)]
stdev_points1 = []
stdev_points2 = []
stdev_points3 = []
mean_points1 = []
mean_points2 = []
mean_points3 = []
function1_mean = 0 # DONE
function2_mean = 0 # DONE
function3_mean = 0 # DONE
function4_mean = 0 # DONE
function5_mean = 0 # DONE
function6_mean = 0 # HelicalValley: TO-DO
function7_mean = 0 # DONE
function8_mean = -106.764537 # DONE
function9_mean = 0 # DONE
function10_mean = 0 # Bukin6: DONE e DIFICIL
function11_mean = -24.1568 # DONE
function12_mean = -42.944387 # DONE
function13_mean = -2.06261 # DONE
function14_mean = -1 # Cross-Leg Table: DONE e DIFICIL
function15_mean = 0.0001 # Crowned-Cross: DONE e DIFICIL
function16_mean = 0 # DONE
function17_mean = -1 # DONE
function18_mean = -959.6407 # DONE
function19_mean = 0.06447 # DONE
function20_mean = 3 # DONE
function21_mean = 0 # DONE
function22_mean = -19.2085 # DONE
function23_mean = 0 # DONE
function24_mean = 0 # DONE
function25_mean = 0 # DONE
function26_mean = -1.9133 # DONE
function27_mean = 0 # DONE
function28_mean = 0.292579 # Schaffer4: DONE e DIFICIL
function29_mean = 1 #

with open(file_path1, 'r') as file1, open(file_path2, 'r') as file2, open(file_path3, 'r') as file3:
    for line1, line2, line3 in zip(file1, file2, file3):
        x1, y1 = map(lambda v: int(v) if v.isdigit() else float(v), line1.strip().split())
        x2, y2 = map(lambda v: int(v) if v.isdigit() else float(v), line2.strip().split())
        x3, y3 = map(lambda v: int(v) if v.isdigit() else float(v), line3.strip().split())
        # Lambda function declares value as an int if v.isdigit() returns true.
        point_list1[x1-1].append(y1)
        point_list2[x2-1].append(y2)
        point_list3[x3-1].append(y3)

# MUDAR AQUI TAMBEM
for s1, s2, s3 in zip(point_list1, point_list2, point_list3):
    stdev_points1.append(statistics.stdev(s1, xbar = function26_mean))
    stdev_points2.append(statistics.stdev(s2, xbar = function27_mean))
    stdev_points3.append(statistics.stdev(s3, xbar = function28_mean))

x_coordinate = []
for x in range(len(stdev_points1)):
    x_coordinate.append(x)

for s1, s2, s3 in zip(stdev_points1, stdev_points2, stdev_points3):
    print(s1, s2, s3, sep='---')
# MUDAR OS LABELS
# Plot the Roots function on the right subplot, taking the full height
axs['d)'].plot(x_coordinate, stdev_points1, color = 'red', label = "Função McCormick" )
axs['d)'].plot(x_coordinate, stdev_points2, color = 'blue', linestyle = '--', label = "Função Schaffer1")
axs['d)'].plot(x_coordinate, stdev_points3, color='purple', linestyle = ':', label = "Função Schaffer4")
axs['d)'].set_title('(σ x c) com numLaços = 100')
axs['d)'].set_xlabel('Ciclos de forrageamento (c)')
axs['d)'].set_ylabel('Desvio Padrão (σ)')
axs['d)'].legend()   

# plt.tight_layout()
plt.show()