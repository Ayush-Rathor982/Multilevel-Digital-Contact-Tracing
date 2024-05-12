'''
First generates data as in folder Dependency and independency by variying dependency in dataGenerate.h file. Then run List_Data.py script to generate file Q_vs_Listsize.txt which contain(q, list size).
This helps to understand how array and list size vary with dependency.
'''


import matplotlib.pyplot as plt

# Set the figure size
plt.figure(figsize=(10, 9))

# Adjusting some plot settings using rcParams
plt.rcParams["figure.figsize"] = [17.50, 13.50]  # Custom figure size
plt.rcParams["figure.autolayout"] = True  # Adjust layout automatically
plt.rcParams["axes.edgecolor"] = "black"  # Set color for the edges of axes
plt.rcParams["axes.linewidth"] = 3.50  # Set linewidth of the axes

# Iterate over two modes: 'independency' and 'dependency'
for mode in ['independency', 'dependency'] :
    if mode == 'independency' :
        # Read data from the file for independency mode
        data_file = "/home/student/Desktop/new_project2/independency/Q_vs_Listsize.txt"

        q_values = []
        size_values = []

        # Extract data from the file
        with open(data_file, 'r') as file:
            for line in file:
                parts = line.strip().split()
                q_values.append(int(parts[0]))
                size_values.append(int(parts[1]))

        # Calculate percentage values
        per_values = [(size * 100) / (10000 * q) for size, q in zip(size_values, q_values)]

        print(size_values)
        
        # Plotting for independency mode
        plt.plot(q_values, per_values, marker='o', linestyle='-', label=f'independency', linewidth=2.5)

    elif mode == 'dependency':
        # Iterate over different dependency percentages
        for dependency in [10,20,30,40,50]:

            # Read data from the file for each dependency percentage
            data_file = "/home/student/Desktop/new_project2/Dependency/dependency_"+str(dependency)+"per/Q_vs_Listsize.txt"

            q_values = []
            size_values = []

            # Extract data from the file
            with open(data_file, 'r') as file:
                for line in file:
                    parts = line.strip().split()
                    q_values.append(int(parts[0]))
                    size_values.append(int(parts[1]))

            # Calculate percentage values
            per_values = [(size * 100) / (10000 * q) for size, q in zip(size_values, q_values)]

            print(size_values)
            
            # Plotting for each dependency percentage
            plt.plot(q_values, per_values, marker='o', linestyle='-', label=f'Dependency {dependency}%', linewidth=2.5)

# Set the x-axis label
plt.xlabel(r'$\langle q \rangle$', fontweight='bold', fontsize=22)
# Set the y-axis label
plt.ylabel('List Size(%)', fontweight='bold', fontsize=22)
plt.xticks(fontsize=20, fontweight='bold')  # Set x-axis tick font properties
plt.yticks(fontsize=20, fontweight='bold')  # Set y-axis tick font properties
plt.legend(fontsize=20)  # Add a legend with specified font size
plt.grid(False)  # Turn off the grid
plt.show()  # Show the plot

