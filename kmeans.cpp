#include <bits/stdc++.h>
using namespace std;

vector<vector<float> > initialize_centres(int k, vector<vector<float> >* data_points);
bool assign_clusters(vector<vector<float> >* data_points, vector<vector<float> >* centres, vector<int>* cluster_assignments);
float calc_dist(vector<float> point1, vector<float> point2);
void update_centres(vector<vector<float> >* centres, vector<int>* cluster_assignments, vector<vector<float> >* data_points);

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		cout << "Please enter 2 arguments" << endl;
		exit(1);
	}

	int k = atoi(argv[1]);
	string input_file = argv[2];
	string output_file = "kmeans.txt";
	ifstream inFile;
	vector<vector<float> > data_points;

	inFile.open(input_file);
	if(!inFile)
	{
		cout << "File doesn't exist" << endl;
		exit(1);
	}

	string line;
	while(getline(inFile,line))
	{
		vector<float> point;
		istringstream iss(line);
		for(string s; iss >> s; )
			point.push_back(stoi(s));
		data_points.push_back(point);
	}

	int num_points = data_points.size();
	int dimension = data_points[0].size();

	// cout << "num_points = " << num_points << endl;
	// cout << "dimension = " << dimension << endl;

	// initialise centres
	vector<vector<float> > centres = initialize_centres(k, &data_points);

	// cout << "Printing initial centres\n";
	// for(int i=0; i<k; i++)
	// 	cout << centres[i][0] << endl;

	// assign clusters
	vector<int> cluster_assignments(num_points,0);
	bool converged;
	int num_iter = 0;
	
	while(true)
	{
		// assign clusters
		converged = assign_clusters(&data_points, &centres, &cluster_assignments);
		if(converged)
			break;
		// update centres
		update_centres(&centres, &cluster_assignments, &data_points);
		num_iter ++;
	}

	cout << "num_iter = " << num_iter << endl;

	// print cluster assignments
	// for(int i = 0; i < cluster_assignments.size(); i ++)
	// {
	// 	cout << cluster_assignments[i] << endl;
	// }

	vector<vector<int> >final_assignments(k);

	for(int i = 0; i < num_points; i++)
	{
		int cluster = cluster_assignments[i];
		final_assignments[cluster].push_back(i);
	}

	ofstream o_file;
	o_file.open(output_file);
	for(int i = 0; i < k; i ++)
	{
		o_file << "#" << i << endl;
		for(int j = 0; j < final_assignments[i].size(); j++)
		{
			o_file << final_assignments[i][j] << endl;
		}
	}
	o_file.close();





}


vector<vector<float> > initialize_centres(int k, vector<vector<float> >* data_points)
{
	// random initialisation --> replace with kmeans++?
	// vector<int> indices;
	// int num_points = (*data_points).size();
	// while(indices.size() != k)
	// {
	// 	int index = rand()%num_points;
	// 	if(find(indices.begin(), indices.end(), index) == indices.end())
	// 		indices.push_back(index);
	// }

	// // cout << "printing indices\n";
	// // for(int i=0; i<k; i++)
	// // 	cout << indices[i] << endl;

	// vector<vector<float> > initial_centres;
	// for(int i=0; i<k; i++)
	// {
	// 	initial_centres.push_back( (*data_points)[indices[i]] );
	// }

	// return initial_centres;

	srand(5);

	// k-means++
	vector<int> indices;
	int num_points = (*data_points).size();
	int first = rand()%num_points;
	indices.push_back(first);
	cout << first << endl;

	float total_dist = 0.0;
	while(indices.size() != k)
	{
		vector<float> distances;
		int total_dist = 0;
		for(int i = 0; i < num_points; i ++)
		{
			if(find(indices.begin(), indices.end(), i) != indices.end()) //if that point is already a centre
				continue;

			vector<float> point = (*data_points)[i];
			float min_dist = calc_dist(point, (*data_points)[indices[0]]);

			for(int j = 1; j < indices.size(); j++)
			{
				float dist = calc_dist(point, (*data_points)[indices[j]]);
				if(dist < min_dist)
					min_dist = dist; 	
			}

			total_dist += pow(min_dist,2);
			distances.push_back(pow(min_dist,2));

		}


		vector<float> cum_dist;
		cum_dist.push_back(0.0);
		double r = ((double) rand() / (RAND_MAX));
		// cout << "r = " << r << endl;
		for(int i = 1; i < num_points; i++)
		{
			
			cum_dist.push_back(cum_dist[i-1] + float(distances[i]/total_dist) );
			// cout << cum_dist[i] << endl;
			if(find(indices.begin(), indices.end(), i) != indices.end()) //if that point is already a centre
				continue;
			if(r <= cum_dist[i])
			{
				indices.push_back(i);
				cout << i << endl;
				break;
			}
		}


	}
	// cout << "DONE!" << endl;
	// exit(1);
	vector<vector<float> > initial_centres;
	for(int i=0; i<k; i++)
	{
		initial_centres.push_back( (*data_points)[indices[i]] );
	}
	return initial_centres;
	
}

bool assign_clusters(vector<vector<float> >* data_points, vector<vector<float> >* centres, vector<int>* cluster_assignments)
{
	bool converged = true;
	// for each point, calculate nearest centre
	int num_points = (*data_points).size();
	int dimension = (*data_points)[0].size();
	int k = (*centres).size();
	int i,j;
	
	for(i = 0; i < num_points; i ++) //for each point
	{
		float min_dist = calc_dist((*data_points)[i], (*centres)[0]);
		int new_cluster = 0;
		for(j = 1; j < k; j++)
		{
			float dist = calc_dist((*data_points)[i], (*centres)[j]);
			if(dist < min_dist)
			{
				min_dist = dist;
				new_cluster = j;
			}
		}

		if( (*cluster_assignments)[i] != new_cluster)
		{
			converged = false;
			(*cluster_assignments)[i] = new_cluster;
		}

	}
	return converged;

}


float calc_dist(vector<float> point1, vector<float> point2) 
{
	int dimension = point1.size();
	float dist = 0;
	for(int i = 0; i < dimension ; i ++ )
	{
		dist += pow( abs(point1[i] - point2[i]), 2);
	}
	return sqrt(dist);
}


void update_centres(vector<vector<float> >* centres, vector<int>* cluster_assignments, vector<vector<float> >* data_points)
{
	int k = (*centres).size();
	int dimension = (*centres)[0].size();
	int num_points = (*data_points).size();
	vector<vector<float> >new_centres (k, vector<float> (dimension, 0.0));
	vector<int> points_per_cluster(k,0);

	for(int i = 0 ; i < num_points; i++)
	{
		int cluster = (*cluster_assignments)[i];
		points_per_cluster[cluster] += 1;
		transform(new_centres[cluster].begin(), new_centres[cluster].end(), (*data_points)[i].begin(), new_centres[cluster].begin(), plus<int>());
	}
	for(int i = 0; i < k; i ++)
	{
		for(int j = 0; j < dimension; j ++)
		{
			new_centres[i][j] = new_centres[i][j] / points_per_cluster[i];
		}
		// transform(new_centres[i].begin(), new_centres[i].end(), new_centres[i].begin(), bind(multiplies<T>(), placeholder::_1, float(1.0/points_per_cluster[i]) ));
	}

	*data_points = new_centres;

}




