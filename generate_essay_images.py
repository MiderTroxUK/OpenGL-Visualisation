import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
from matplotlib import colormaps as cm

# 1. Displacement Map / FEA Mesh Visualization
def plot_displacement_mesh(filename):
    fig = plt.figure(figsize=(10, 5))
    
    # Original Mesh (Left)
    ax1 = fig.add_subplot(121, projection='3d')
    x = np.linspace(-5, 5, 20)
    y = np.linspace(-5, 5, 20)
    X, Y = np.meshgrid(x, y)
    Z = np.zeros_like(X)
    ax1.plot_wireframe(X, Y, Z, color='gray', alpha=0.5)
    ax1.set_title("Original Mesh (Flat)", fontsize=10)
    ax1.set_axis_off()

    # Displaced Mesh (Right)
    ax2 = fig.add_subplot(122, projection='3d')
    # Create simple heat/displacement function (e.g., Gaussian stress)
    R = np.sqrt(X**2 + Y**2)
    Z_disp = 2.0 * np.exp(-0.1 * R**2) * np.sin(0.5*X) # Deform based on "stress"
    
    # Plot wireframe deformed
    ax2.plot_wireframe(X, Y, Z_disp, color='b', alpha=0.6)
    # Overlay heatmap surface for clarity (Vertex Shader applies colors too!)
    surf = ax2.plot_surface(X, Y, Z_disp, cmap='coolwarm', alpha=0.3, antialiased=False)
    
    ax2.set_title("Vertex Shader Displacement (FEA Step)", fontsize=10)
    ax2.set_axis_off()
    
    plt.tight_layout()
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()

# 2. Volume Rendering Ray Casting Diagram
def plot_ray_casting(filename):
    fig, ax = plt.subplots(figsize=(8, 5))
    
    # Define limits
    ax.set_xlim(-2, 12)
    ax.set_ylim(-2, 8)
    
    # Draw Volume (Cube Projection / 2D Grid)
    grid_x_start = 2
    grid_y_start = 0
    grid_size = 6
    cells = 6
    step = grid_size / cells
    
    # Draw Grid of "Voxels"
    for i in range(cells):
        for j in range(cells):
            rect = patches.Rectangle((grid_x_start + i*step, grid_y_start + j*step), step, step, linewidth=1, edgecolor='lightgray', facecolor='none')
            ax.add_patch(rect)
    
    # Add Volume Label
    ax.text(grid_x_start + 1, grid_y_start + grid_size + 0.5, "Volumetric Data (Voxels)", fontsize=10, fontweight='bold')

    # Draw Eye
    eye_pos = (-1, 3)
    ax.plot(eye_pos[0], eye_pos[1], 'ko', markersize=8)
    ax.text(eye_pos[0]-0.8, eye_pos[1]+0.5, "Camera/Eye", fontsize=10)
    
    # Draw Screen Plane (Vertical Line)
    screen_x = 1
    ax.plot([screen_x, screen_x], [0, 6], 'k-', linewidth=2)
    ax.text(screen_x-0.5, 6.2, "Screen Plane", fontsize=10, rotation=90)

    # Draw Ray
    ray_start = eye_pos
    # Ray goes through volume
    ray_end = (10, 5) # Slight angle
    ax.arrow(ray_start[0], ray_start[1], ray_end[0]-ray_start[0], ray_end[1]-ray_start[1], head_width=0.3, head_length=0.5, fc='r', ec='r', linestyle='--')
    ax.text(5, 4.2, "Ray Marching Path", color='red', fontsize=10)

    # Draw Sampling Points
    num_samples = 8
    # Parameterize line: P(t) = P0 + t*Dir
    # Simple discrete points for illustration
    # approximate path in grid coords
    # y = m*x + c. m = (5-3)/(10--1) = 2/11 = 0.18
    # y - 3 = 0.18 * (x - (-1)) => y = 0.18x + 3.18
    
    for x_samp in np.linspace(grid_x_start+0.5, grid_x_start+grid_size-0.5, num_samples):
        y_samp = 0.18 * x_samp + 3.18
        if grid_y_start <= y_samp <= grid_y_start+grid_size:
            ax.plot(x_samp, y_samp, 'yo', markeredgecolor='k', markersize=6, label='Sample Point' if x_samp==np.linspace(grid_x_start+0.5, grid_x_start+grid_size-0.5, num_samples)[0] else "")

    ax.legend(loc='lower right')
    ax.set_axis_off()
    plt.tight_layout()
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    plt.close()

# 3. CFD Streamlines
def plot_cfd_streamlines(filename):
    # Setup grid
    Y, X = np.mgrid[-3:3:100j, -5:10:100j]
    
    # Define flow around a cylinder (Potential Flow Theory approximation)
    # U = U_inf * (1 - R^2/r^2 * cos(2theta)) ... simplified
    # Just use simple stream function for flow past a cylinder
    U_inf = 1
    R_cyl = 1.0
    
    # Convert to polar
    R = np.sqrt(X**2 + Y**2)
    Theta = np.arctan2(Y, X)
    
    # Mask inside cylinder
    outside_mask = R >= R_cyl
    
    # Radial and tangential velocities
    Vr = U_inf * (1 - (R_cyl/R)**2) * np.cos(Theta)
    Vt = -U_inf * (1 + (R_cyl/R)**2) * np.sin(Theta)
    
    # Back to Cartesian
    Vx = Vr * np.cos(Theta) - Vt * np.sin(Theta)
    Vy = Vr * np.sin(Theta) + Vt * np.cos(Theta)
    
    # Apply mask
    Vx[~outside_mask] = 0
    Vy[~outside_mask] = 0
    
    # Add some "turbulence" or wake behind (conceptually)
    # Simple modification for visual effect behind cylinder (x>0, |y|<R)
    wake_mask = (X > 0.5) & (np.abs(Y) < 1.2)
    Vx[wake_mask] *= 0.5 + 0.2*np.sin(5*Y[wake_mask]*X[wake_mask]) # perturb
    
    speed = np.sqrt(Vx**2 + Vy**2)
    
    fig, ax = plt.subplots(figsize=(10, 5), facecolor='black')
    ax.set_facecolor('black')
    
    # Cylinder patch
    cyl = patches.Circle((0,0), radius=R_cyl, fc='gray', ec='white', zorder=10)
    ax.add_patch(cyl)
    
    # Streamplot
    strm = ax.streamplot(X, Y, Vx, Vy, color=speed, linewidth=1.5, cmap='plasma', density=1.5, arrowsize=1.2)
    
    # Colorbar
    cbar = fig.colorbar(strm.lines, ax=ax)
    cbar.set_label('Velocity Magnitude (m/s)', color='white')
    cbar.ax.yaxis.set_tick_params(color='white')
    plt.setp(plt.getp(cbar.ax.axes, 'yticklabels'), color='white')
    
    ax.set_title("CFD Visualization: Airflow Velocity Field (GPU Compute)", color='white', fontsize=12)
    ax.set_xlim(-4, 8)
    ax.set_ylim(-3, 3)
    ax.set_aspect('equal')
    ax.set_axis_off()
    
    plt.savefig(filename, dpi=150, bbox_inches='tight', facecolor='black')
    plt.close()

if __name__ == "__main__":
    print("Generating images...")
    try:
        plot_displacement_mesh("displacement_fea_diagram.png")
        print("Generated displacement_fea_diagram.png")
        plot_ray_casting("ray_casting_diagram.png")
        print("Generated ray_casting_diagram.png")
        plot_cfd_streamlines("cfd_streamlines_vis.png")
        print("Generated cfd_streamlines_vis.png")
        print("Success")
    except Exception as e:
        print(f"Error: {e}")
