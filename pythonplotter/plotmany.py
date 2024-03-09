import matplotlib.pyplot as plt
import numpy as np


# Read points from the .txt file
file_pathv3 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/pythonplotter/ABCv3.txt"
file_pathv4 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/pythonplotter/ABCv4.txt"
file_pathv5 = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/pythonplotter/ABCv5.txt"
pointsv3 = []
pointsv4 = []
pointsv5 = []

with open(file_pathv3, 'r') as file1:
    for line in file1:
        x, y = map(float, line.strip().split())
        pointsv3.append((x, y))
        
with open(file_pathv4, 'r') as file2:
    for line in file2:
        x, y = map(float, line.strip().split())
        pointsv4.append((x, y))
        
with open(file_pathv5, 'r') as file3:
    for line in file3:
        x, y = map(float, line.strip().split())
        pointsv5.append((x, y))


# Connect points with lines
first = True
for i in range(len(pointsv3) - 1):
    if first:
        first = False
        plt.plot([pointsv3[i][0], pointsv3[i+1][0]], [pointsv3[i][1], pointsv3[i+1][1]], color='magenta', label='V3')
        plt.plot([pointsv4[i][0], pointsv4[i+1][0]], [pointsv4[i][1], pointsv4[i+1][1]], color='blue', label='V4')
        plt.plot([pointsv5[i][0], pointsv5[i+1][0]], [pointsv5[i][1], pointsv5[i+1][1]], color='red', label='V5')
    else:
        plt.plot([pointsv3[i][0], pointsv3[i+1][0]], [pointsv3[i][1], pointsv3[i+1][1]], color='magenta')
        plt.plot([pointsv4[i][0], pointsv4[i+1][0]], [pointsv4[i][1], pointsv4[i+1][1]], color='blue')
        plt.plot([pointsv5[i][0], pointsv5[i+1][0]], [pointsv5[i][1], pointsv5[i+1][1]], color='red')

plt.legend(loc="upper right")

# Set labels and title
plt.xlabel('Ciclos de forrageamento')
plt.ylabel('Eixo Z')
plt.title('Comparação da performance da V3, V4 e V5')

# Show the plot
plt.grid(True)
plt.show()
