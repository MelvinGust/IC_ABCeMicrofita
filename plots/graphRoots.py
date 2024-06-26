import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Define the function g(z)
def g(z):
    return 1 / np.abs(z**6 - 1)

# Generate complex values for z
x = np.linspace(-2, 2, 50)
y = np.linspace(-2, 2, 50)
X, Y = np.meshgrid(x, y)
Z = X + 1j * Y

# Compute the modulus of g(z) for each complex value of z
G = g(Z)

# Create a 3D plot
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='cont')

# Plot the modulus of g(z)
surf = ax.plot_surface(X, Y, np.abs(G), cmap='plasma')

# Add labels and title
ax.set_xlabel('Re(z)')
ax.set_ylabel('Im(z)')
ax.set_zlabel('Magnitude da função g(z) (|g(z)|)')
ax.set_title('Mapa de contorno da magnitude de g(z) = 1/|z^6 - 1|')

# Add a color bar
fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5)

# Show the plot
plt.show()