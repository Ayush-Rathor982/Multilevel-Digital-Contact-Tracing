'''
First generates data as shown in folder D by varying incubation period, D. Then run List_Data.py script to generates file D_vs_Listsize.txt contains (D, List size).
It helps to understand how list size varies with incubation period.
'''

import matplotlib.pyplot as plt

# Read data from the file
data_file = "/home/student/Desktop/new_project2/D/D_vs_Listsize.txt"

D_values = []  # List to store D values
size_values = []  # List to store size values

# Read data from the file
with open(data_file, 'r') as file:
    for line in file:
        parts = line.strip().split()  # Split each line into parts
        D_values.append(int(parts[0]))  # Append D value to the list
        size_values.append(int(parts[1]))  # Append size value to the list

# Calculate percentage values
per_values = [(size * 100) / (10000 * q) for size, q in zip(size_values, D_values)]

print(size_values)

# Plotting
plt.plot(D_values, size_values, marker='o', linestyle='-')  # Plot D vs size
plt.xlabel('D', fontweight='bold', fontsize=20)  # Set x-axis label
plt.ylabel('List Size', fontweight='bold', fontsize=15)  # Set y-axis label
plt.xticks(fontsize=10, fontweight='bold')  # Set x-axis tick font properties
plt.yticks(fontsize=10, fontweight='bold')  # Set y-axis tick font properties
plt.legend()  # Add legend (if applicable)
plt.grid(False)  # Turn off the grid
plt.show()  # Show the plot
