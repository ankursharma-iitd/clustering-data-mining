#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

void run_dbscan(int min_points, double epsilon, const string input_file_name, const string output_file_name);
double get_distance(int one, int two);
void initialise_neighbourhood(double epsilon);
bool expand_cluster(int p, int cluster_label, double epsilon, int min_points);
void delete_vector(vector<int> &v, int value);

#define UNCLASSIFIED -2
#define NOISE -1

struct point
{
    int id;                    //unique id of each point
    vector<double> dimensions; // coordinates of the point
    int label_number;          // -2 -> unclassified, -1 -> noise, >= 0 -> cluster ID
    int neighbour_number;      // number of core points from the neightbourhood of this point earlier deleted from D
    vector<int> neighbourhood; //int ids of all the points which are in epsilon-neighbourhood of this point
};

int main(int argc, char *argv[])
{
    int min_points = atoi(argv[1]);
    double epsilon = atof(argv[2]);
    string infile = argv[3];
    string outfile = "dbscan.txt";
    run_dbscan(min_points, epsilon, infile, outfile);
    return 0;
}

unordered_map<int, point *> point_map;
unordered_map<int, int> retrieve_map;
int number_of_points;
int number_of_dims;

void run_dbscan(int min_points, double epsilon, const string input_file_name, const string output_file_name)
{
    //read the input file here, and generate a map fr
    ifstream inputFile(input_file_name);
    if (!inputFile)
    {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    double dimension;
    long absolute_count = 1;
    string str;
    while (getline(inputFile, str))
    {
        point *p = (point *)calloc(1, sizeof(point));
        p->id = absolute_count;
        p->label_number = UNCLASSIFIED; //UNCLASSIFIED
        p->neighbour_number = 0;        // NO NEIGHBOURS DELETED INTITALLY
        p->neighbourhood.emplace_back(absolute_count);
        istringstream inf(str);
        while (inf >> dimension)
        {
            (p->dimensions).emplace_back(dimension);
        }
        point_map[absolute_count++] = p;
    }
    cout << "DATASET READ." << endl;

    //assigning some globals
    number_of_points = point_map.size();
    number_of_dims = point_map[1]->dimensions.size();

    //initialising the neighbourhood value of each point
    initialise_neighbourhood(epsilon);
    cout << "INITIALISATION DONE." << endl;

    //Data stricture is done -> run the dbscan algorithm
    int cluster_label = 0;
    for (int i = 1; i <= number_of_points; i++)
    {
        if (point_map.find(i) != point_map.end() && point_map[i]->label_number == UNCLASSIFIED)
        {
            cout << "CURRENT POINT : " << i << endl;
            if (expand_cluster(i, cluster_label, epsilon, min_points))
            {
                cluster_label ++;
            }
        }
    }
    cout <<  "TOTAL NUMEBR OF CLUSTERS : " << cluster_label << endl;

    //output the results
    unordered_map<int, vector<int>> clusters;
    for(int i = 1; i <= number_of_points; i ++)
    {
        int current_label = -10000;
        if(point_map.find(i) != point_map.end())
        {
            current_label = point_map[i]->label_number;
        }
        else if(retrieve_map.find(i) != retrieve_map.end())
        {
            current_label = retrieve_map[i];
        }

        clusters[current_label].emplace_back(i);
    }

    ofstream outfile;
    outfile.open(output_file_name);
    for(int i = 0; i < cluster_label; i ++)
    {
        outfile << "#" << i << "\n";
        for(int point : clusters[i])
        {
            outfile << point << "\n";
        }
    }
    if(clusters[NOISE].size() > 0)
    {
        outfile << "#outlier\n";
        for(int point : clusters[NOISE])
        {
            outfile << point << "\n";
        }
    }
    outfile.close();
    cout << "FILE CREATED." << endl;

    //this is only for checking purposes
    outfile.open("dbscan_check.txt");
    for(int i = 1; i <= number_of_points; i ++)
    {
        int current_label = -10000;
        if (point_map.find(i) != point_map.end())
        {
            current_label = point_map[i]->label_number;
        }
        else if (retrieve_map.find(i) != retrieve_map.end())
        {
            current_label = retrieve_map[i];
        }
        outfile << current_label << endl;
    }
    outfile.close();

    return;
}

//returns the euclidean distance between two points
double get_distance(point *p_one, point *p_two)
{
    double distance = 0.0;
    for (int i = 0; i < number_of_dims; i++)
    {
        double diff = p_one->dimensions[i] - p_two->dimensions[i];
        distance += (diff * diff);
    }
    return sqrt(distance);
}

//this method is used for intialising the neighbourhood of all the points as per the epsilon value, and updates the point_map as well
void initialise_neighbourhood(double epsilon)
{
    for (int i = 1; i <= number_of_points - 1; i++)
    {
        cout << i << endl;
        for (int j = i + 1; j <= number_of_points; j++)
        {
            if (get_distance(point_map[i], point_map[j]) <= epsilon)
            {
                point_map[i]->neighbourhood.emplace_back(j);
                point_map[j]->neighbourhood.emplace_back(i);
            }
        }
    }
    return;
}

bool expand_cluster(int p, int cluster_label, double epsilon, int min_points)
{
    vector<int> seeds = point_map[p]->neighbourhood;
    point_map[p]->neighbour_number += seeds.size();
    if (point_map[p]->neighbour_number < min_points)
    {
        point_map[p]->label_number = NOISE;
        return false;
    }
    else
    {
        point_map[p]->label_number = cluster_label;
        // cout << "2.0" << endl;
        for (int neighbour : point_map[p]->neighbourhood)
        {
            // cout << "2.01" << endl;
            // cout << "neighbour" << neighbour << endl;
            if (neighbour != p)
            {
                delete_vector(point_map[neighbour]->neighbourhood, p);
            }
        }
        retrieve_map[p] = point_map[p]->label_number; //this will help to retrieve it back later
        point_map.erase(p);
        // cout << "2.1" << endl;
        delete_vector(seeds, p);
        // cout << "2.2" << endl;
        for (int q : seeds)
        {
            if (point_map[q]->label_number == NOISE)
            {
                point_map[q]->label_number = cluster_label;
                delete_vector(seeds, q);
            }
            else
            {
                point_map[q]->label_number = cluster_label;
                point_map[q]->neighbour_number++;
            }
        }
        // cout << "2.3" << endl;
        while (seeds.size() > 0)
        {
            int curPoint = seeds[0];
            vector<int> curSeeds = point_map[curPoint]->neighbourhood;
            point_map[curPoint]->neighbour_number += curSeeds.size();
            // cout << "2.4" << endl;
            if (point_map[curPoint]->neighbour_number >= min_points)
            {
                for (int q : curSeeds)
                {
                    if (point_map[q]->label_number == UNCLASSIFIED)
                    {
                        point_map[q]->label_number = cluster_label;
                        point_map[q]->neighbour_number++;
                        seeds.emplace_back(q);
                    }
                    else if (point_map[q]->label_number == NOISE)
                    {
                        point_map[q]->label_number = cluster_label;
                    }
                }
                // cout << "2.5" << endl;
                for (int neighbour : point_map[curPoint]->neighbourhood)
                {
                    if (neighbour != curPoint)
                    {
                        delete_vector(point_map[neighbour]->neighbourhood, curPoint);
                    }
                }
                retrieve_map[curPoint] = point_map[curPoint]->label_number;
                point_map.erase(curPoint);
                // cout << "2.6" << endl;
            }
            delete_vector(seeds, curPoint);
            // cout << "2.7" << endl;
        }
        // cout << "2.8" << endl;
        return true;
    }
}

void delete_vector(vector<int> &v, int value)
{
    for (vector<int>::iterator iter = v.begin(); iter != v.end(); ++iter)
    {
        if (*iter == value)
        {
            v.erase(iter);
            break;
        }
    }
}
