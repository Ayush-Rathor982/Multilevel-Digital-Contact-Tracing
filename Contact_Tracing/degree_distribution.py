import os
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

# Read the data
with open('/home/student/Desktop/new_project2/Q10_D10_N1000/Contact_Graph_info/day2.txt', 'r') as file:    # Give the path of the Contact_Graph_info's file containing information about the contact graph, from which the degree distribution is to be found
    lines = file.readlines()

# Create a graph
G = nx.Graph()
for line in lines:
    # Parse the line and extract vertices
    parts = line.split()
    vertices = (int(parts[0]), int(parts[1]))
    
    # Add edges to the graph
    if vertices[1] != -9999:
        G.add_edge(vertices[0], vertices[1])

# Calculate degrees
degrees = dict(G.degree())

# Extract degree sequence for plotting
data = np.array([G.degree(n) for n in G.nodes()])

# Create a figure and axis for plotting
fig, ax = plt.subplots(1, 1, figsize=(10, 8))

# Degree Distribution Plot

# Compute the degree frequency histogram
degree_freq = nx.degree_histogram(G)
degrees = range(len(degree_freq))

# Calculate average degree
average_degree = sum(degrees[i] * degree_freq[i] for i in range(len(degrees))) / G.number_of_nodes()

# Plot the degree distribution
ax.plot(degrees, degree_freq, marker='o')

# Set the scale of axes (optional)
# ax.set_yscale('log')
# ax.set_xscale('log')

# Set labels for the axes and title for the plot
ax.set_xlabel('Degree', fontweight='bold', fontsize=15)
ax.set_ylabel('f', fontweight='bold', fontsize=24, rotation=0)
# ax.set_title('Degree Distribution')

# Add text for average degree
ax.text(0.98, 0.89, f"Average Degree: {average_degree:.2f}", transform=ax.transAxes, ha='right', va='top', fontsize=16, fontweight='bold')

# Set the tick labels on the x-axis and y-axis to bold
for tick in ax.xaxis.get_major_ticks():
    tick.label.set_fontsize(13)
    tick.label.set_fontweight('bold')
for tick in ax.yaxis.get_major_ticks():
    tick.label.set_fontsize(13)
    tick.label.set_fontweight('bold')

# Display the plot
plt.show()
# Save the plot as an image (optional)
# plt.savefig("/home/student/Desktop/new_project2/Q10_D10_N1000.png")
