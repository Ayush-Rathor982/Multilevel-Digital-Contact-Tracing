// Multilevel Digital Contact Tracing, G. Mahapatra, P. Pradhan, A. Khan, S. K. Setua, R. K. Pal, A. Rathor, R. Chattaraj, S. Banerjee.

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include <set>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <bits/stdc++.h>
#include <dirent.h>
#include <filesystem>
#include <sys/stat.h>  // For mkdir function
#include <unistd.h>    // For access function

using namespace std;



/*stores the person's contacts in array*/
class data
{
public:
    int a = -9999;          // stores contected person
    vector<int> *vector_pointer = NULL;    //it points to the vector of size of incubation_period which stores the contact information of persons for past day(incubation_period)
};

/*stores person's contact information in list node*/
class list_node_data
{
public:
    int a = -9999;        // stores contected person
    vector<int> *vector_pointer = NULL;         //it points to the vector of size of incubation_period which stores the contact information of persons for past day(incubation_period)
    list_node_data *prev, *next;       //node pointers
    //constructor
    list_node_data()
    {
        prev = NULL;
        next = NULL;
    }
};

/*stores person's contact in link list and also stores size/count of link list*/
class List{
    public:
     int count;
     list_node_data * list_pointer;      //it points to the link list of persons
     //constructor
     List(){
        count=0;
        list_pointer = NULL;
     }
};

int incubation_period = 5;     // It's a period after which symptoms of infection start appearing
int infection_recovery_interval = 8;      //It's a period required to recover a infected person
vector<set<int>> contact_trace_list(infection_recovery_interval);     //It's a vector which stores the set of traced persons in a day
vector<set<int>> Infected_list(infection_recovery_interval);          //It's a vector which stores the set of infected persons in a day


/*makes the contact 0(false) in vectors for current day, because as day started no one have any contact, they makes contact as day runs*/
void Initializeto0(data *G_psi, List *G_psi_list, int n, int q, int curr_day)
{
    for (int t = 0; t < n * q; t++)    //traversing the array which stores contact information of persons
    {
        if (G_psi[t].vector_pointer != NULL)
            G_psi[t].vector_pointer[0][curr_day] = 0;      //if vector pointer is not null then that means they have contact in past days, now set contact for current day to 0(false)
    }
    for (int i = 0; i < n; i++)         //traversing the linked list of each person 'i'  
    {
        if (G_psi_list[i].list_pointer != NULL)       //checking whether person 'i' have linked list or not
        {
            list_node_data *temp = G_psi_list[i].list_pointer;      //pointer to the linked list of person 'i'
            while (temp != NULL)
            {
                temp->vector_pointer[0][curr_day] = 0;
                temp = temp->next;
            }
        }
    }
}

/*Storing the link lists size or count in a file. It helps to observe that how link list grows.*/
void store_list_count(List *G_psi_list, int N, int current_day){
    string folder = "List_Count";
    mkdir(folder.c_str(), 0777);    //creating new folder List_Count

    string subfolder = folder + "/day_count";   //creating a subfolder day_count in List_Count folder which stores daywise list size count
    mkdir(subfolder.c_str(),0777);

    std::string FileName = subfolder + "/day" + std::to_string(current_day) + ".txt";       //creating a new file which store list count/size

    ofstream outfile(FileName, std::ios::app);      //opening new created file

    for(int i=0; i<N; i++){
        outfile<<i+1<<" "<<G_psi_list[i].count<<endl;        //writing list count in new created file
    }
    outfile.close();       //closing the file
}


/*Adds the new contact between v1 and v2 as they have no contact before */
void add_contact(data *G_psi, List *G_psi_list, int v1, int v2, int q, int current_day)
{
    bool is_contact_added = false;     //Checking that contact is added in array or not, if not then it will be added in linked list
     
    /*Checking that if any index is empty in array scope of v1, it is for checking that till now v1 makes contacts less than average degree or not*/ 
    for (int i = (v1 - 1) * q; i < v1 * q; i++)
    {
        if (G_psi[i].vector_pointer == NULL)    //if any index is empty in array scope of a1 then adding v2 in it, means adding contact information between v1 and v2 in array scope of v1
        {
            G_psi[i].a = v2;                    // adding v2 in array scope of v1, it shows that v1 and v2 makes contact
            G_psi[i].vector_pointer = new vector<int>(incubation_period, 0);        //allocation a vector of size of incubation period which stores contact history of these person for incubation period
            G_psi[i].vector_pointer[0][current_day] = 1;      //set the vector pointer's value for current day to 1
            is_contact_added = true;      // contact is added in array scope of v1
            break;
        }
    }
    /*If there is no empty feild in array scope of v1 then adding contact in linked list*/
    if (!is_contact_added)
    {
        // creating new list node
        list_node_data *new_list_node = new list_node_data;
        new_list_node->a = v2;
        new_list_node->vector_pointer = new vector<int>(incubation_period, 0);    //allocation a vector of size of incubation period which stores contact history of these person for incubation period
        new_list_node->vector_pointer[0][current_day] = 1;
           
        // inserting new node at the front of linked list of v1
        if (G_psi_list[v1 - 1].list_pointer == NULL)
        {   
            G_psi_list[v1-1].count++;                            //incresing linked list size or count by 1
            G_psi_list[v1 - 1].list_pointer = new_list_node;
        }
        else
        {   
            G_psi_list[v1-1].count++;                             //incresing linked list size or count by 1
            new_list_node->next = G_psi_list[v1-1].list_pointer;
            new_list_node->next->prev = new_list_node;
            G_psi_list[v1-1].list_pointer = new_list_node;
        }
    }
 

    /*As we added v2 in v1's scope, now adding v1 in v2's scope*/
    is_contact_added = false;

    /*Checking that if any index is empty in array scope of v2, it is for checking that till now v2 makes contacts less than average degree or not*/ 
    for (int i = (v2 - 1) * q; i < v2 * q; i++)
    {
        if (G_psi[i].vector_pointer == NULL)           //if any index is empty in array scope of a1 then adding v2 in it, means adding contact information between v1 and v2 in array scope of v2
        {  
            G_psi[i].a = v1;                            // adding v1 in array scope of v2, it shows that v1 and v2 makes contact
            G_psi[i].vector_pointer = new vector<int>(incubation_period, 0);        //allocation a vector of size of incubation period which stores contact history of these person for incubation period
            G_psi[i].vector_pointer[0][current_day] = 1;               //set the vector pointer's value for current day to 1
            is_contact_added = true;                           // contact is added in array scope of v2
            break;
        }
    }
    /*If there is no empty feild in array scope of v2 then adding contact in linked list*/
    if (!is_contact_added)
    {
        // creating new list node
        struct list_node_data *new_list_node = new list_node_data;
        new_list_node->a = v1;
        new_list_node->vector_pointer = new vector<int>(incubation_period, 0);        //allocation a vector of size of incubation period which stores contact history of these person for incubation period
        new_list_node->vector_pointer[0][current_day] = 1;

        // inserting new node at the front of linked list of v2
        if (G_psi_list[v2 - 1].list_pointer == NULL)
        {   
            G_psi_list[v2-1].count++;
            G_psi_list[v2 - 1].list_pointer = new_list_node;
        }
        else
        {   
            G_psi_list[v2-1].count++;
            new_list_node->next = G_psi_list[v2-1].list_pointer;
            new_list_node->next->prev = new_list_node;
            G_psi_list[v2-1].list_pointer = new_list_node;
        }
    }
}

/*Removes those contacts whose vector's all values(incubation period values) are 0(false), It is because they have no contacts in past days(incubation period)*/
void remove_contact(data *G_psi, List *G_psi_list, int n, int q)
{
    /*Traversing the array which stores contact information of persons to remove a contact whose vector's all values are 0(false)*/
    for (int cc = 0; cc < n * q; cc++)
    {
        if (G_psi[cc].vector_pointer != NULL)
        {
            int vector_value = 1;            //it tells that there exist a contact or not, whose vector's all values are 0(false),  if it's value is 1 then such contact is exist and we have to remove it, if it's value is 0 then no such contact exist.
            for (int ch = 0; ch < incubation_period; ch++)
              vector_value = vector_value*(1-G_psi[cc].vector_pointer[0][ch]);      //calculating vactor_value

            if(vector_value)         //if vector_value is 1 then such contact exist and we have to remove it
            {
                G_psi[cc].a = -9999;
                G_psi[cc].vector_pointer = NULL; 
            }
        }
    }
    
    /*Traversing the linked lists of each person to remove a contact whose vector's all values are 0(false)*/
    for (int i = 0; i < n; i++)
    {
        if (G_psi_list[i].list_pointer!= NULL)
        {
            list_node_data *temp = G_psi_list[i].list_pointer;      //pointer to the linked list of person 'i'
            while (temp != NULL)
            {
                int vector_value = 1;          //it tells that there exist a contact or not, whose vector's all values are 0(false),  if it's value is 1 then such contact is exist and we have to remove it, if it's value is 0 then no such contact exist.
                for (int ch = 0; ch < incubation_period; ch++)
                 vector_value = vector_value*(1 - temp->vector_pointer[0][ch]);      //calculating vactor_valu

                list_node_data *temp1 = temp;         
                temp = temp->next;              //updating the pointer

                /*if vector_value is 1 then such contact exist and we have to remove it by following linked list deletion methods*/
                if (vector_value)           
                {
                    if (temp1->next == NULL && temp1->prev == NULL)
                    {   
                        G_psi_list[i].count = 0;       //as link list becomes empty, so set it's size or count to 0
                        G_psi_list[i].list_pointer = NULL;
                    }
                    else if (temp1->next == NULL)
                    {   
                        G_psi_list[i].count--;         //decresing link list size or count by 1
                        temp1->prev->next = NULL;
                        delete (temp1);
                    }
                    else if (temp1->prev == NULL)
                    {   
                        G_psi_list[i].count--;         //decresing link list size or count by 1
                        temp1->next->prev = NULL;
                        G_psi_list[i].list_pointer = temp1->next;
                        delete (temp1);
                    }
                    else
                    {   
                        G_psi_list[i].count--;          //decresing link list size or count by 1
                        temp1->next->prev = temp1->prev;
                        temp1->prev->next = temp1->next;
                        delete (temp1);
                    }
                }
            }
        }
    }
}

/*Printing the contacts information, how persons contacting*/
void printing_data(data *G_psi, List *G_psi_list, int n, int q)
{
    int x = 0;
    int contacting_person = 1;

    string folderName = "Output";      //Creating a Output folder
    mkdir(folderName.c_str(),0777);

    string FileName = folderName + "/Contact_Graph_info.txt";     

    ofstream out(FileName);

    for (int y = 0; y < n * q; y++)
    {

        if (x % q == 0)
        {
            out << "\t"
                 << "Contacting Person " << contacting_person << "\n \n";
            contacting_person++;
        }
        out <<"index"<< y << " "
             << "Contected Person " << G_psi[y].a << "\t"
             << ""
             << "\t ";
        if (G_psi[y].vector_pointer != NULL)
        {

            for (int ii = 0; ii < incubation_period; ii++)
            {
                out << G_psi[y].vector_pointer[0][ii] << " ";
            }
        }
        out << "\n";

        if ((y + 1) % q == 0 && G_psi_list[y / q].list_pointer != NULL)
        {
            out << y << " Printing list" << endl;
            list_node_data *temp = G_psi_list[y / q].list_pointer;
            while (temp != NULL)
            {
                out << "\t"
                     << "Contected Person " << temp->a << "\t";
                for (int i = 0; i < incubation_period; i++)
                    out << temp->vector_pointer[0][i] << " ";
                out << endl;
                temp = temp->next;
            }
        }

        x = x + 1;
        if (x % q == 0)
        {
            out << "\n \n";
        }
    }
}

/*function for storing edges*/
void Storingedges(int ver1, int ver2, string filename)
{
    string ParentfolderName = "Output";      //Creating a Input folder
    mkdir(ParentfolderName.c_str(),0777); 
    string folder = ParentfolderName + "/infected_path_list";     //making a folder infected_path_list
    mkdir(folder.c_str(),0777); 
    string FileName = folder + "/" + filename;   //making file which stores edges
    //cout<<endl<<endl<<"FileName "<<FileName<<endl;
    ofstream outfile(FileName, ios::app);   // open file for appending
    if(ver1!=0 && ver2!=0)
    outfile << ver1 << " " << ver2 << endl; // write values to file

    outfile.close(); // close the file
}

/*converting binary to decimal*/
long int btod_convert(bool *arr)
{
    long int dec = 0;

    for (int i = 4, j = 0; i >= 0; i--, j++)
    {
        dec = dec + arr[i] * pow(2, j);
    }
    return dec;
}

/*function for binary pattern*/
long int binary_pattern(vector<int> &vec, int p)
{

    bool newArray[incubation_period];
    int newIndex = 0;

    for (int j = (p + 1) % incubation_period; j != p; j = (j + 1) % incubation_period)
        newArray[newIndex++] = vec[j];

    newArray[newIndex] = vec[p];

    return btod_convert(newArray);
}

/*function for checking early and latest contact*/
int TO_C3(vector<int> &vec, int p, int con)
{

    bool newArray[incubation_period];
    int newIndex = 0;

    for (int j = (p + 1) % incubation_period; j != p; j = (j + 1) % incubation_period)
        newArray[newIndex++] = vec[j];

    newArray[newIndex] = vec[p];

    if (con == 1)
    {
        for (int i = 0; i < incubation_period; i++)
            if (newArray[i] == 1)
                return i;
    }

    if (con == 0)
    {
        for (int j = incubation_period - 1; j >= 0; j--)
            if (newArray[j])
                return j;
    }

    return -1;
}

bool Traceoperator(int dec1, int dec2, int i, int j)
{
    return (dec1 >= dec2) ? true : (i <= j) ? true
                                            : false;
}

/*Function for tracing the contacts of infected person level wise*/
void Level_trace(data *G_psi, List *G_psi_list, int infected_person, bool infected_pointer_list[], int max_level, int q, int curr_trace_day, int current_day, int day)
{
    queue<pair<string, pair<int, int>>> Q1, Q2;   //Making queues, 
                                                  //string value => traced person is in array or linked list
                                                  //pair<int,int> => pair of infected person and his directly contacted person, if string value is list
                                                                                      //OR 
                                                                     //pair of traced person's index in array scope of infected person and traced person, if string value is array

    int level = 1;    //Tracing started with first level

    string ParentfolderName = "Output";      //Creating a Input folder
    mkdir(ParentfolderName.c_str(),0777); 
    string folder = ParentfolderName + "/Level_traverse";     //making a folder Level_traverse
    mkdir(folder.c_str(),0777); 
    string filename = folder + "/day" + to_string(day) + ".txt";   //making file which stores trace list or infected list for curent day 
    ofstream outfile(filename, ios::app);   // open file for appending
    outfile << endl <<"Infected Person \t"<<infected_person<<endl; // write values to file
    outfile <<"level "<<level<<endl;

    Storingedges( 0, 0, "day"+to_string(day)+".txt");

    /*First level taverse for array scope of infected person*/
    for (int i = (infected_person - 1) * q; i < infected_person * q; i++)
    { // tracing the contacts of infected person
        if (G_psi[i].vector_pointer != NULL && !infected_pointer_list[G_psi[i].a])
        { // checking whether traversed person already lie in infected list or not

            contact_trace_list[curr_trace_day].insert(G_psi[i].a);     //inserting the traced person into contact_trace_list
            Q1.push({"array", {i, G_psi[i].a}});                   // inserting traced person into Q1 and telling it lies in array scope of    
            outfile<<G_psi[i].a<<" ";                              
            infected_pointer_list[G_psi[i].a] = true;              // make person to lie in infected list
            Storingedges(infected_person, G_psi[i].a, "day"+to_string(day)+".txt"); // storing edge
        }
    }
    /*First level traverse for link list of infected person*/
    if (G_psi_list[infected_person - 1].list_pointer != NULL)      // checking if list empty or not
    {                                                          
        list_node_data *temp = G_psi_list[infected_person - 1].list_pointer;    // making pointer to the link list of infected person
        while (temp != NULL)
        { // traversing list
            if (!infected_pointer_list[temp->a])
            {
                contact_trace_list[curr_trace_day].insert(temp->a);      //inserting the traced person into contact_trace_list
                Q1.push({"list", {infected_person - 1, temp->a}});       //inserting traced person into Q1 and telling it lies in list and contact pair 
                outfile<<temp->a<<" ";
                infected_pointer_list[temp->a] = true;              // make person to lie in infected list
                Storingedges(infected_person, temp->a, "day"+to_string(day)+".txt"); // storing the edge
            }
            temp = temp->next;
        }
    }
    outfile<<endl;
    level++;         //go to the second level traverse

    /*multiple level traverse*/
    while ((level <= max_level) && !Q1.empty() && Q2.empty())    //trace persons to the max level and while there is contacts happens between persons upto max level
    {
        outfile <<"level "<<level<<endl;
        while (!Q1.empty())                   
        {
            pair<string, pair<int, int>> front_pair = Q1.front();   // takes the first element of Q1 that means taking first person's information for tracing its contact
            string S = front_pair.first;
            pair<int, int> nested_pair = front_pair.second;
            int index = nested_pair.first;
            int person = nested_pair.second;
            Q1.pop(); // deleting the first element of Q1 

            long int dec1, early;
            if (S == "array")
            {
                dec1 = binary_pattern(G_psi[index].vector_pointer[0], current_day); // converting the vector to decimal
                early = TO_C3(G_psi[index].vector_pointer[0], current_day, 1);      // gets earliest contact, when it made contact with infected person
            }
            else
            {
                list_node_data *temp = G_psi_list[index].list_pointer;
                for (; temp->a != person; temp = temp->next) // traversing the list
                    ;
                dec1 = binary_pattern(temp->vector_pointer[0], current_day); // converting the vector to decimal
                early = TO_C3(temp->vector_pointer[0], current_day, 1);      // gets earliest contact, when it made contact with infected person
            }

            /*traversing the person's contact array*/
            for (int i = (person - 1) * q; i < person * q; i++)
            {
                if (G_psi[i].vector_pointer != NULL && !infected_pointer_list[G_psi[i].a])
                {
                    long int dec2 = binary_pattern(G_psi[i].vector_pointer[0], current_day); // converting the vector to decimal
                    int latest = TO_C3(G_psi[i].vector_pointer[0], current_day, 0);          // gets the latest contact

                    /*calling the traceoperator to check that infection spread or not*/
                    if (Traceoperator(dec1, dec2, early, latest) && G_psi[i].a != infected_person)
                    {
                        contact_trace_list[curr_trace_day].insert(G_psi[i].a);  
                        Q2.push({"array", {i, G_psi[i].a}});
                        outfile<<G_psi[i].a<<" ";
                        infected_pointer_list[G_psi[i].a] = true;
                        Storingedges(person, G_psi[i].a, "day"+to_string(day)+".txt");
                    }
                }
            }

            /*traversing the person's contact list*/
            if (G_psi_list[person - 1].list_pointer != NULL)
            {
                list_node_data *temp = G_psi_list[person - 1].list_pointer;
                while (temp != NULL)
                {
                    if (!infected_pointer_list[temp->a])
                    {
                        long int dec2 = binary_pattern(temp->vector_pointer[0], current_day); // converting the vector to decimal
                        int latest = TO_C3(temp->vector_pointer[0], current_day, 0);          // gets the latest contact

                        if (Traceoperator(dec1, dec2, early, latest) && temp->a != infected_person)
                        {
                            contact_trace_list[curr_trace_day].insert(temp->a);
                            Q2.push({"list", {person - 1, temp->a}});
                            outfile<<temp->a<<" ";
                            infected_pointer_list[temp->a] = true;
                            Storingedges(person, temp->a, "day"+to_string(day)+".txt");
                        }
                    }
                    temp = temp->next;
                }
            }
        }
        cout << endl;
        outfile<<endl;
        swap(Q1, Q2); // swaping Q1 and Q2
        level++;
    }

    outfile.close();
}


/*Function for deleting Previously stored data*/
void deleteFilesInFolder(const string& folderPath) {
    DIR* dir = opendir(folderPath.c_str());    

    if (dir != nullptr) {
        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                string filePath = folderPath + "/" + entry->d_name;
                if (unlink(filePath.c_str()) == 0) {
                    // cout << "Deleted: " << filePath << endl;
                } else {
                    // cerr << "Error deleting: " << filePath << endl;
                }
            }
        }

        closedir(dir);
    } else {
        // cerr << "Error opening directory: " << folderPath << endl;
    }
}



int main()
{   
    /*Start Deleting files which stored previous data*/
    deleteFilesInFolder("Output/infected_path_list");
    deleteFilesInFolder("Output/Level_traverse");
    deleteFilesInFolder("Output");
    /*Stop Deleting files which stored previous data*/

    cout<<endl<<"Which Test Case you want to run"<<endl;
    cout<<"=====================================================";
    cout << endl << "1 => Test Case 1"<<endl
            <<"2 => Test Case 2"<<endl;
    cout<<"====================================================="<<endl;    
    cout<<endl<<"Enter your choice: ";

    int test_case;
    cin>>test_case;

    int N = (test_case==1)? 6 : 10 ;         // Total Population size
    
    int q = 5;         // q is the average Degree, how many average contacts a person make in a day

    int current_day = 0; // Pointer to the current day
    int infection_recovery_pointer =0;  // Pointer to the current recovery day

    data G_psi[N * q];     // Array for storing contact information of each person, here each person was allocated q indexes 
    List G_psi_list[N];    // Array of linked list, allocating linked list to each person, if person makes conatct more than average degree than those contacts will goes in linked list
    bool trace_list[N];    // Its array for checking that perticular person is in trace list or not, that means a person already made contact with infected person 
    for (bool &v : trace_list)
    {
        v = false; // Initialize trace list to false
    }


    int choice = 1;
    int day=1;
    int day_count = 1;  //it for taking count of incubation period as initially we have to genrate data for incubation period because till that no symptoms will be there
    while (1)
    {   
        if(day_count <= 6) {day_count++;}
        else if(choice == 1) {choice = 2;}
        else {choice = 3;}

        switch (choice)
        {
        case 1:
        {  


                Initializeto0(G_psi, G_psi_list, N, q, current_day);   // function for initializing current day value in vectors to 0
                
                ifstream FileName("Input/TestCase"+to_string(test_case)+"/GeneratedData/day" + to_string(day) + ".txt");    // It reads the generated data for the current day from file which stores data(contacts) as edges 
                int a1, b1;        // for vertices of edges

                while (FileName >> a1 >> b1)
                {   
                    if(!trace_list[a1] || !trace_list[b1])   //Checking that whether a1 or b1 is in trace list or not 
                    {  
                    bool exists_in_ArrayOrList = false;      // for cheking wether the vetices lie in array or list, that means they already made a contact or not

                    /*Checking wether b1 lie in array scope of a1*/
                    for (int jj = (a1 - 1) * q; jj < a1 * q; jj++)   //traversing the array scope of a1
                    {    
                        if (G_psi[jj].a == b1)         // checking that b1 lie in array scope of a1
                        {
                            exists_in_ArrayOrList = true; // b1 lies in array
                            G_psi[jj].vector_pointer[0][current_day] = 1;        //set vector pointer's value for current day to 1
                            break;
                        }
                    }
 
                    /*If b1 not lie in array scpe of a1, then checking wether b1 lie in list scope of a1*/
                    if (!exists_in_ArrayOrList && G_psi_list[a1 - 1].list_pointer != NULL)
                    {
                        list_node_data *temp = G_psi_list[a1 - 1].list_pointer;      //pointer to the linked list of a1
                        while (temp != NULL)
                        {
                            if (temp->a == b1)
                            {
                                temp->vector_pointer[0][current_day] = 1;       //set vector pointer's value for current day to 1
                                exists_in_ArrayOrList = true; // b1 lies in list
                                break;
                            }
                            temp = temp->next;
                        }
                    }
                    
                    /*If a1 made contact with b1, then b1 also made contact with a1*/
                    if(exists_in_ArrayOrList){
                        bool exist_in_Array = false;     // for checking a1 lie in array or linked list of b1
                        /*Checking wether a1 lie in array scope of b1*/
                        for (int jj = (b1 - 1) * q; jj < b1 * q; jj++)
                        {    
                            if (G_psi[jj].a == a1)
                            {
                                exist_in_Array = true; // a1 lies in array scope of b1
                                G_psi[jj].vector_pointer[0][current_day] = 1;   //set vector pointer's value for current day to 1
                                break;
                            }
                        }
                        /*If a1 not lie in array scope of b1 then it definetly lie in linked list of b1*/
                        if (!exist_in_Array )
                        {
                            list_node_data *temp = G_psi_list[b1 - 1].list_pointer;    // pointer to linked list of b1
                            while (temp != NULL)
                            {
                                if (temp->a == a1)
                                {
                                    temp->vector_pointer[0][current_day] = 1;     //set vector pointer's value for current day to 1
                                    exist_in_Array  = true;   // a1 lies in linked list ob b1
                                    break;
                                }
                                temp = temp->next;
                            }
                        }
                    }

                    /*Execute when a1 and b1 are not in each other's scope, that means they have no contact before*/
                    if (!exists_in_ArrayOrList)
                    {
                        add_contact(G_psi, G_psi_list, a1, b1, q, current_day); // function for adding a1-b1 contact 
                    }
                  }  
                }

               remove_contact(G_psi, G_psi_list, N, q); // Removing those edges(contacts) whose vector contain all 0's, that means they have no contact in past days(incubation period)

               //store_list_count(G_psi_list,N,day);   // Storing the linked lists size in a file, its for observing that how linked list grows

               /*Remove all the person from contact_trace_list whose recovery period completed and who recovered from infection*/
                for(auto it = contact_trace_list[infection_recovery_pointer].begin();it!=contact_trace_list[infection_recovery_pointer].end(); ++it)
                  trace_list[*it]=false;
                contact_trace_list[infection_recovery_pointer].clear();
                
                /*Remove all the persons from Infected_list who recovered from infection*/
                for(auto it = Infected_list[infection_recovery_pointer].begin();it!=Infected_list[infection_recovery_pointer].end(); ++it)
                  trace_list[*it]=false;
                Infected_list[infection_recovery_pointer].clear();

                current_day = (current_day + 1) % incubation_period;       //updating current_day pointer
                infection_recovery_pointer = (infection_recovery_pointer + 1) % infection_recovery_interval;     //updating infection_recovery_pointer 
                day++;

            printing_data(G_psi, G_psi_list, N, q);    // Printing whole data,  how each person making contact in current day

            break;
        }
        case 2: /// For giving the Infected List
        {

            ifstream inf_file("Input/TestCase"+to_string(test_case)+"/Infected_List/day6.txt");     //Opening a file which stores the infected persons
            int infected_person;       // infected person
            int curr_day = (current_day + (incubation_period - 1)) % incubation_period;         //again goes to current day, as we increamented its value
            int curr_trace_day = (infection_recovery_pointer + (infection_recovery_interval-1)) % infection_recovery_interval;        //again goes to current trace day, as we increamented its value

            while (inf_file >> infected_person)        //reading the file infected.txt which stores the infected persons
            {  
                auto it = contact_trace_list[curr_trace_day].find(infected_person);       //checking that infected person is already in contact_trace_list or not
                if(it != contact_trace_list[curr_trace_day].end())                        //if infected person already in contact_trace_list then remove it from there so that it will be added to infected list 
                  contact_trace_list[curr_trace_day].erase(it);
                
                trace_list[infected_person]=true;               //Set it true, it is just for showing that person either in contact_trace_list or infected list
                Infected_list[curr_trace_day].insert(infected_person);            // adding person to infected list
                Level_trace(G_psi, G_psi_list, infected_person, trace_list, 3, q, curr_trace_day, curr_day, day-1);             //Tracing the contacts of infected person
            }
            break;
        }
        case 3:
        {
           exit(0);
        }

        }
    }
return 0;    
}
