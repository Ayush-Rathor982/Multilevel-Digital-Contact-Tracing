# import os
# import matplotlib.pyplot as plt
# import networkx as nx
# import numpy as np
# import powerlaw

# fig, ax = plt.subplots(1, 1, figsize=(10, 8))

# for q in [5,10,20,50,80,150]:
#     # Read the data
#     with open('/home/student/Desktop/new_project2/independency/Q'+str(q)+'_out_population10000/Contact_Graph_info.txt', 'r') as file:
#         lines = file.readlines()

#     # Create a graph
#     G = nx.Graph()
#     for line in lines:
#         # Parse the line and extract vertices
#         parts = line.split()
#         vertices = (int(parts[0]), int(parts[1]))
        
#         # Add edges to the graph
#         if vertices[1] != -9999:
#             G.add_edge(vertices[0], vertices[1])

#     # Calculate degrees97
#     degrees = dict(G.degree())

#     data = np.array([G.degree(n) for n in G.nodes()])

#     # Degree Distribution Plot
#     degree_freq = nx.degree_histogram(G)
#     degrees = range(len(degree_freq))
#     average_degree = sum(degrees[i] * degree_freq[i] for i in range(len(degrees))) / G.number_of_nodes()
#     # ax.bar(degrees, degree_freq, width=0.8, color='b')
#     ax.plot(degrees, degree_freq, marker='o',label=r'$\langle q \rangle$ : 'f'{q}')
#     # ax.set_yscale('log')
#     # ax.set_xscale('log')
# ax.set_xlabel('$q_{i}$', fontweight='bold', fontsize=15, labelpad=15,)
# ax.set_ylabel(r'$f_{i}$', fontweight='bold', fontsize=21, labelpad=25, rotation=0)
# # ax.set_title('Degree Distribution')
# # ax.text(0.98, 0.89, f"Average Degree: {average_degree:.2f}", transform=ax.transAxes, ha='right', va='top', fontsize=16, fontweight='bold')


# # Set the tick labels on the x-axis and y-axis to bold
# for tick in ax.xaxis.get_major_ticks():
#     tick.label.set_fontsize(11)
#     tick.label.set_fontweight('bold')
# for tick in ax.yaxis.get_major_ticks():
#     tick.label.set_fontsize(11)
#     tick.label.set_fontweight('bold')

# # Add legend
# plt.legend()

# # plt.yscale('log')
# # plt.show()
# plt.savefig("/home/student/Desktop/new_project2/independency/vary_Q_degree_distribution_plot.png")



import os
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import powerlaw

fig, ax = plt.subplots(1, 1, figsize=(10, 8))

color = ['#041E42','#00008B','#0047AB','#0039a6','#007FFF','#7CB9E8']

for i,q in enumerate([5,10,20,50,80,150]):
    # Read the data
    with open('/home/student/Desktop/new_project2/independency/Q'+str(q)+'_out_population10000/Contact_Graph_info.txt', 'r') as file:
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

    data = np.array([G.degree(n) for n in G.nodes()])

    # Degree Distribution Plot
    degree_freq = nx.degree_histogram(G)
    degrees = range(len(degree_freq))
    
    # Filter out degrees with zero frequency
    non_zero_degrees = [d for d, freq in zip(degrees, degree_freq) if freq != 0]
    non_zero_degree_freq = [freq for freq in degree_freq if freq != 0]
    
    total_frequency = 0;
    for frequency in non_zero_degree_freq:
       total_frequency += frequency

    non_zero_degree_prob = [freq/total_frequency for freq in non_zero_degree_freq]
    
    ax.plot(non_zero_degrees, non_zero_degree_prob, marker='o', color=color[i], label=r'$\langle q \rangle$ : 'f'{q}')

ax.set_xlabel('$q_{i}$', fontweight='bold', fontsize=15, labelpad=15,)
ax.set_ylabel(r'$f_{i}$', fontweight='bold', fontsize=21, labelpad=25, rotation=0)

# Add legend
plt.legend()

# Set the tick labels on the x-axis and y-axis to bold
for tick in ax.xaxis.get_major_ticks():
    tick.label.set_fontsize(11)
    tick.label.set_fontweight('bold')
for tick in ax.yaxis.get_major_ticks():
    tick.label.set_fontsize(11)
    tick.label.set_fontweight('bold')

plt.show()
# plt.savefig("/home/student/Desktop/new_project2/independency/vary_Q_degree_distribution_plot.png")
