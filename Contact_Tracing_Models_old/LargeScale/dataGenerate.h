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
#include<map>
#include<fstream>
#include<cstring>
#include<string>
#include <sys/stat.h> // For creating directories

using namespace std;

 double alpha = 1.2;  // Power-law exponent
 double xmin = 0.8;   // Minimum value
 double xmax = 100.0; // Maximum value


/*Function for checking that perticular edge s-g already exist or not*/
bool checkcondition(int s, int g, multimap<int, int>&myMap){
    bool exists=false;
    auto range = myMap.equal_range(s);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second==g)
            exists=true;
    }
    return exists;
}

    random_device rdm;
    mt19937 genr(rdm());

/*Generates edge_count using uniform distribution*/
int generate_edges(int k)
{
    uniform_int_distribution<> dis(1, k);
    int Random_Index = dis(genr);
    return Random_Index;
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(0.0, 1.0);

/*Removes the already written data from file*/
void RemoveDataInFile(string Filename){
    ofstream outfile(Filename, ios::out | ios::trunc);
    outfile.close();
}


/*Function which stored the generated edges(contacts) in file*/
void Add_edges(const std::multimap<int, int>& myMap, int n, int curr_day, float q) {

    string ParentfolderName = "Input";      //Creating a Input folder
    mkdir(ParentfolderName.c_str(),0777);

    string folderName = ParentfolderName + "/GeneratedData";      //Creating a folder GenerateData
    mkdir(folderName.c_str(),0777);

    string FileName = folderName + "/day"+to_string(curr_day)+".txt";     //Creating a file for current day  which stores the contact for current day

    // Remove data from the file if it exists
    RemoveDataInFile(FileName);

    // Open the file for appending
    std::ofstream outfile(FileName, std::ios::app);

    // Write values to the file
    for (auto it = myMap.begin(); it != myMap.end(); ++it) {
        outfile << it->first << " " << it->second << std::endl;
    }

    // Close the file
    outfile.close();
}

/*Function which generates data(contacts) for current day*/
void generate_data(int n,int curr_day,float f){

    xmax = n;        //updating x_max value 
    int s,g,edg_cnt;

    int max_possible_edges = ((n*(n-1))/2)*f;              //Calculating maximum possible edges(contacts) for a day
    if (max_possible_edges<=0){
        max_possible_edges=1;
    }

        multimap<int, int> myMap;      //Stores the edges(contacts)
        edg_cnt=generate_edges(max_possible_edges);      //Generate Edge_count using uniform distribution

        cout<<"Number of close contacts: "<<edg_cnt<<endl;

        /*Generating edges using power law distribution*/
        for (int j=1; j<=edg_cnt; j++){

            double randValue = dis(gen);        //generating random value in range (0,1)
            double value = ceil(pow((pow(xmax, 1.0 - alpha) - pow(xmin, 1.0 - alpha)) * randValue + pow(xmin, 1.0 - alpha), 1.0 / (1.0 - alpha)));  //generate value using power law
            int vetice1 = static_cast<int>(value);    //typecast value to integer and assign to vertice1
            randValue = dis(gen);               //generating random value in range (0,1)
            value = ceil(pow((pow(xmax, 1.0 - alpha) - pow(xmin, 1.0 - alpha)) * randValue + pow(xmin, 1.0 - alpha), 1.0 / (1.0 - alpha)));    //generate value using power law
            int vetice2 = static_cast<int>(value);    //typecast value to integer and assign to vertice2

            /*If vetice1==vetice2 then we have to change one of the vertice's value because self edge not possible*/
            while (vetice1==vetice2)
            { 
                randValue = dis(gen);          //generating random value in range (0,1)
                value = ceil(pow((pow(xmax, 1.0 - alpha) - pow(xmin, 1.0 - alpha)) * randValue + pow(xmin, 1.0 - alpha), 1.0 / (1.0 - alpha)));    //generate value using power law
                vetice2=static_cast<int>(value);       //typecast value to integer and assign to vertice2
            }
            
            /*Making edge sorted*/
            s = (vetice1>vetice2)? vetice2:vetice1;
            g = (vetice1>vetice2)? vetice1:vetice2;
            
            /*Checking that this edge is already exist*/
            if(!checkcondition(s,g,myMap)){
            myMap.insert(make_pair(s,g));
            }
           
        }
    
    /*Adding Generated edges*/
    Add_edges(myMap, n, curr_day, f);

}

