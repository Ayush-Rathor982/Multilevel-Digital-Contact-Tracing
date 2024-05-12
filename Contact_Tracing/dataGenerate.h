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

double xmax = 100.0; // Maximum value

/*Function for checking that perticular edge s-g already exist or not*/
bool checkcondition(int s, int g, multimap<int, int> &myMap)
{
    bool exists = false;
    auto range = myMap.equal_range(s);
    for (auto it = range.first; it != range.second; ++it)
    {
        if (it->second == g)
            exists = true;
    }
    return exists;
}

random_device rdm;
mt19937 genr(rdm());

/*Generates edge_count using uniform distribution*/
int generate_edges(long long int k)
{
    uniform_int_distribution<> dis(1, k);
    int Random_Index = dis(genr);
    return Random_Index;
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(0.0, 1.0);

/*Removes the already written data from file*/
void RemoveDataInFile(string Filename)
{
    ofstream outfile(Filename, ios::out | ios::trunc);
    outfile.close();
}

/*Function which stored the generated edges(contacts) in file*/
void Add_edges(const std::multimap<int, int> &myMap, int n, int curr_day, float q)
{

    string ParentfolderName = "Input"; // Creating a Input folder
    mkdir(ParentfolderName.c_str(), 0777);

    string folderName = ParentfolderName + "/GeneratedData"; // Creating a folder GenerateData for storing day wise contacts
    mkdir(folderName.c_str(), 0777);

    string FileName = folderName + "/day" + to_string(curr_day) + ".txt"; // Creating a file for current day  which stores the contact for current day

    // Remove data from the file if it exists
    RemoveDataInFile(FileName);

    // Open the file for appending
    std::ofstream outfile(FileName, std::ios::app);

    // Write values to the file
    for (auto it = myMap.begin(); it != myMap.end(); ++it)
    {
        outfile << it->first << " " << it->second << std::endl;
    }

    // Close the file
    outfile.close();
}

/*Function which generates data(contacts) for current day*/
void generate_data(long int n, long int curr_day, float f, int q, int D)
{

    xmax = n; // updating x_max value
    int s, g, edg_cnt;

    long long int max_possible_edges = ((n * (n - 1)) / 2) * f; // Calculating maximum possible edges(contacts) for a day
    //  cout<<max_possible_edges<<endl;
    if (max_possible_edges <= 0)
    {
        max_possible_edges = 1;
    }

    multimap<int, int> myMap; // Stores the edges(contacts)
    // edg_cnt=generate_edges(max_possible_edges);      //Generate Edge_count using uniform distribution

    edg_cnt = (q * n) / (2 * D);

    cout << "Number of close contacts: " << edg_cnt << endl;

    /*This section is for dependent contacts (takes/repeats some contacts from previous days)*/
    /*
    ifstream inputFile("Input/GeneratedData/day"+to_string(curr_day-1)+".txt"); // Assuming your file name is data.txt

    vector<pair<int, int>> edges;

    // Read all edges from the file
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        int source, destination;
        ss >> source >> destination;
        edges.push_back(make_pair(source, destination));
    }

    // Randomly select edges
    const int totalEdges = edges.size();
    const int edgesToSelect = min(totalEdges, ((edg_cnt*10)/100)); // To ensure we don't select more edges than available.  Here 10% contacts are selected, you can vary this percentage.
    for (int i = 0; i < edgesToSelect; ++i) {
        int randomIndex = rand() % totalEdges;
        myMap.insert(make_pair(edges[randomIndex].first,edges[randomIndex].second));
    }



    edg_cnt = (curr_day!=1)? edg_cnt*(0.9):edg_cnt;   //here 90% new contacts will be generated as 10% contacts are repeating. It changes as percentage of contacts repeating vary.
    */
    /*dependent contact section ends*/

    /*Generating edges using power law distribution*/
    for (int j = 1; j <= edg_cnt; j++)
    {

        int vetice1 = generate_edges(n);
        int vetice2 = generate_edges(n);

        /*If vetice1==vetice2 then we have to change one of the vertice's value because self edge not possible*/
        while (vetice1 == vetice2)
        {
            vetice2 = generate_edges(n);
        }

        /*Making edge sorted*/
        s = (vetice1 > vetice2) ? vetice2 : vetice1;
        g = (vetice1 > vetice2) ? vetice1 : vetice2;

        /*Checking that this edge is already exist*/
        if (!checkcondition(s, g, myMap))
        {
            myMap.insert(make_pair(s, g));
        }
        else
        {
            j--;
        }
    }

    /*Adding Generated edges*/
    Add_edges(myMap, n, curr_day, f);
}
