for D in [5,10,15,20,25]:

    # Read data from the file
    data_file = "/home/student/Desktop/new_project2/D/D"+str(D)+"_population10000/Data_size_for_population10000.txt"  # Assuming your file name is data.txt
    population = []
    arraysize = []
    listsize = []

    with open(data_file, 'r') as file:
        for line in file:
            parts = line.strip().split()
            population.append(int(parts[1]))
            listsize.append(int(parts[2]))
            arraysize.append(int(parts[3]))

    listsize = listsize[10:100]

    # print(q,listsize)        

    # Calculate the average of listsize
    listsize_average = int(sum(listsize) / len(listsize))

    # Write the average to a new file
    output_file = "/home/student/Desktop/new_project2/D/D_vs_Listsize.txt"
    with open(output_file, 'a') as file:
        file.write(f"{D} {listsize_average}\n")

    print("Average of listsize:", listsize_average)