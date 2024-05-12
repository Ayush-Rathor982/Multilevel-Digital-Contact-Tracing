#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <map>
#include <fstream>
#include <cstring>
#include <string>
#include <sys/stat.h> // For creating directories

using namespace std;

random_device rdm_inf;
mt19937 gen_inf(rdm_inf());

/*Generates data using uniform distribution*/
int generate_data(int k)
{
    uniform_int_distribution<> dis(1, k);
    int Random_Index = dis(gen_inf);
    return Random_Index;
}

/*Removes the already written data from file*/
void RemoveInfDataInFile(string Filename)
{
    ofstream outfile(Filename, ios::out | ios::trunc);
    outfile.close();
}

/*Generates the infected persons list*/
void GenerateInfectedList(int N, int day, data *G_psi, int q)
{
    int k = 2;
    int count_of_infected_persons = generate_data(k); // generate count of infected persons

    count_of_infected_persons = 2;

    string ParentfolderName = "Input"; // Creating a Input folder
    mkdir(ParentfolderName.c_str(), 0777);

    string folderName = ParentfolderName + "/Infected_List"; // Creating a folder Infected_List
    mkdir(folderName.c_str(), 0777);

    string FileName = folderName + "/day" + to_string(day) + ".txt"; // Creating a file infected_list which stores the infected person's lst

    // Remove data from the file if it exists
    RemoveInfDataInFile(FileName);

    // Open the file
    std::ofstream outfile(FileName);
    cout << "Infected List: ";
    xmax = N;

    for (int i = 1; i <= count_of_infected_persons; i++)
    {
        int infected_person = generate_data(N); // generates the infected person

        bool found = false;
        for (int i = (infected_person - 1) * q; i < infected_person * q; i++)
        {
            if (G_psi[i].a != -9999)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            outfile << infected_person << " ";
            cout << infected_person << " ";
        }
        else
        {
            i--;
            cout << endl
                 << "generated infected person not in list " << endl;
        }
    }
    cout << endl;

    // Close the file
    outfile.close();
}
