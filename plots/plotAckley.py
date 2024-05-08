import numpy as np
import matplotlib.pyplot as plt

def add_colorbar(mappable):
    from mpl_toolkits.axes_grid1 import make_axes_locatable
    import matplotlib.pyplot as plt
    last_axes = plt.gca()
    ax = mappable.axes
    fig = ax.figure
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)
    cbar = fig.colorbar(mappable, cax=cax)
    cbar.ax.tick_params(labelsize = 6)
    plt.sca(last_axes)
    return cbar

# Define Ackley function
def ackley(x, y):
    return -20 * np.exp(-0.2 * np.sqrt(0.5 * (x**2 + y**2))) - \
           np.exp(0.5 * (np.cos(2 * np.pi * x) + np.cos(2 * np.pi * y))) + 20 + np.exp(1)

# Define Sphere function
def sphere(x, y):
    return x**2 + y**2

# Define Beale function
def beale(x, y):
    return (1.5 - x + x*y)**2 + (2.25 - x + x*y**2)**2 + (2.625 - x + x*y**3)**2

# Define Roots function
def roots(x, y):
    return np.abs(x) + np.abs(y)

# Generate x and y values
x = np.linspace(-5, 5, 100)
y = np.linspace(-5, 5, 100)
X, Y = np.meshgrid(x, y)

# Calculate function values for each function
Z_ackley = ackley(X, Y)
Z_sphere = sphere(X, Y)
Z_beale = beale(X, Y)
Z_roots = roots(X, Y)

# Create subplots
fig, axs = plt.subplot_mosaic([['a)', 'd)', 'd)','d)'], ['b)', 'd)','d)','d)'], ['c)','d)','d)','d)']], layout='tight')
# (2, 2, figsize=(14, 7))

# Plot the Ackley function on the left top subplot
fig1 = axs['a)'].contourf(X, Y, Z_ackley, cmap='viridis', levels=50)
axs['a)'].set_title('Ackley Function', fontsize = 7)
axs['a)'].tick_params(labelsize=7)
axs['a)'].set_xlabel('x', fontsize = 7)
axs['a)'].set_ylabel('y', fontsize = 7)
add_colorbar(fig1)

# Plot the Sphere function on the left middle subplot
fig2 = axs['b)'].contourf(X, Y, Z_sphere, cmap='viridis', levels=50)
axs['b)'].set_title('Sphere Function', fontsize = 7)
axs['b)'].tick_params(labelsize=7)
axs['b)'].set_xlabel('x', fontsize = 7)
axs['b)'].set_ylabel('y', fontsize = 7)
add_colorbar(fig2)

# Plot the Beale function on the left bottom subplot
fig3 = axs['c)'].contourf(X, Y, Z_beale, cmap='viridis', levels=50)
axs['c)'].set_title('Beale Function', fontsize = 7)
axs['c)'].tick_params(labelsize=7)
axs['c)'].set_xlabel('x', fontsize = 7)
axs['c)'].set_ylabel('y', fontsize = 7)
add_colorbar(fig3)
# Plot the Roots function on the right subplot, taking the full height
axs['d)'].contourf(X, Y, Z_roots, cmap='viridis', levels=20)
axs['d)'].set_title('Roots Function')
axs['d)'].set_xlabel('X')
axs['d)'].set_ylabel('Y')

# plt.tight_layout()
plt.show()
