import matplotlib.pyplot as plt
import numpy as np


# Read points from the .txt file
file_path = "c:/Users/Melvin/Documents/GitHub/IC_ABCeMicrofita/pythonplotter/ABCperformance.txt"
points = []

with open(file_path, 'r') as file:
    for line in file:
        x, y = map(float, line.strip().split())
        points.append((x, y))

# Extract x and y coordinates from the points
x_values = [point[0] for point in points]
y_values = [point[1] for point in points]

# Plot the points
plt.scatter(x_values, y_values, color='black')

# Connect points with lines
for i in range(len(points) - 1):
    plt.plot([points[i][0], points[i+1][0]], [points[i][1], points[i+1][1]], color='blue')

# Set labels and title
plt.xlabel('Number of foraging cycles')
plt.ylabel('Value of the dependent variable')
plt.title('Optimization Scatter Plot')

# Show the plot
plt.grid(True)
plt.show()
