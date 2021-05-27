#include<bits/stdc++.h>

using namespace std;
int n;
vector<vector<int> > population;
int population_size = 100;
priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > tour_distances;
vector<unordered_map<int, double> >  listCityHeap;
vector<vector<int> > parent1;
vector<vector<int> > parent2;

int mtate = 1;
int mthresh = 3;

class City
{
	public:
		double x, y;
		bool visited;
		
		City(){
		this->visited = false;	
		}
		
		
};

bool comparator(priority_queue<pair<double, pair<int, int> >, vector<pair<double, pair<int, int> > >, greater<pair<double, pair<int, int> > > > c1, priority_queue<pair<double, pair<int, int> >, vector<pair<double, pair<int, int> > >, greater<pair<double, pair<int, int> > > > c2)
{
	return c1.top().first <= c2.top().first;	
}

void generatePopulation()
{
	//population.clear();
	for(int i = 0; i < population_size; i++)
		{
			vector<int> path;
			unordered_set<int> trackPath;
		//	int prev = INT_MIN;
			for(int j = 0; j < n; j++)
			{
				int randomCity = (rand() % n);
			
				while( trackPath.find(randomCity) != trackPath.end())
					randomCity = (rand() % n);
				
				path.push_back(randomCity);
				trackPath.insert(randomCity);
				//cout << randomCity << " ";	
			}
			//cout << endl;
			population.push_back(path);
		}
		
		for(int i = 0; i < population_size; i++)
		{
			population[i].push_back(population[i][0]);
		}
}

void generateTour()
{
	priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > tempDistances;
	
	for(int i = 0; i < population_size; i++)
		{
			double sum = 0;
			//cout << "=====================================\n";
			for(int j = 1; j < n+1; j++)
			{
				sum += listCityHeap[population[i][j-1]].find(population[i][j])->second;
			
			}
			
			tempDistances.push(make_pair(sum, i));
		
		}
		tour_distances = tempDistances;
}

void rouletteWheel()
{
//	srand(time(0));
	vector<vector<int> > rou;
	vector<pair<double, int> > temp;
	int count = 0;
	parent1.clear(); parent2.clear();
	while(count < 10)
	{
		temp.push_back(tour_distances.top());
		tour_distances.pop();
		count++;
	}
	//cout << tour_distances.size() << endl;
	for(int i = 0; i < 10; i++)
	{
		
		tour_distances.push(temp[i]);
		
	}
	//cout << tour_distances.size() << endl;
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < (10 - i); j++)
		{
			rou.push_back(population[temp[i].second]);
		}
	}
	
	for(int i = 0; i < 5; i++)
	{
		parent1.push_back(rou[rand() % rou.size()]);
		parent2.push_back(rou[rand() % rou.size()]);
	}

	
}

void mutate(vector<int> &child_1, vector<int> &child_2)
{
//	srand(time(0));
	for(int i = 1; i <= mtate; i++)
	{
		int index1 = rand() % n;
		int index2 = rand() % n;
	
		while(index1 == index2)
			index2 = rand() % n;
	
		swap(child_1[index1], child_1[index2]);
		swap(child_2[index1], child_2[index2]);
	}
	mtate++;
	if(mtate == mthresh)
		mtate = 1;
	
	
}
void newCrossOver()
{
	vector<vector<int> > children;
//	srand(time(0));
	for(int p = 0; p < parent1.size(); p++)
	{
		vector<int> parent_1 = parent1[p];
		vector<int> parent_2 = parent2[p];
	
		int start = rand() % (n);
		
		vector<int> child1, child2;
		
		unordered_set<int> us1, us2;
		
		for(int i = start; i < parent_1.size()-1; i++)
		{
			us1.insert(parent_1[i]);
			us2.insert(parent_2[i]);
		}
			
		int ind = 0;
		
		while( ind < n)
		{
		//	cout << parent_1[ind] << " = " << *us2.begin()+ind << endl;
			if(us2.find(parent_1[ind]) == us2.end())
			{
				child1.push_back(parent_1[ind]);
			}
			ind++;
			
			
			
		}
		
				//cout << "========================================\n";
				for(int i = start; i < parent_2.size()-1; i++)
					child1.push_back(parent_2[i]);

		
		ind = 0;
		
		while( ind < n)
		{
		//	cout << parent_2[ind] << " = " << *us1.begin()+ind << endl;
			if(us1.find(parent_2[ind]) == us1.end())
			{
				child2.push_back(parent_2[ind]);
			}
			ind++;
			
		}
		for(int i = start; i < parent_1.size()-1; i++)
			child2.push_back(parent_1[i]);

		mutate(child1, child2);
		child1.push_back(child1[0]);
		child2.push_back(child2[0]);
		
		children.push_back(child1);
		children.push_back(child2);
		//cout << "\nCorssOvr : Children completed " << children.size() << endl;
	}
	
	
	vector<vector<int> > tempPopulation;
	
	priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > temp_tour_distances;
	
	
	for(int i = 0; i < 90; i++)
	{
		//tempPopulation.push_back(population[i]);
		temp_tour_distances.push(tour_distances.top());
		tempPopulation.push_back(population[tour_distances.top().second]);
		tour_distances.pop();
		
	}
	
	tour_distances = temp_tour_distances;
		
	//copy(tempPopulation.begin(), tempPopulation.begin(), back_inserter(population));

	copy(children.begin(), children.end(), back_inserter(tempPopulation));
	population = tempPopulation;
	//cout << "Population = " << population.size() << endl;
}


int main()
{
	string dis;
	
	
	srand(time(0)); 
	getline(cin, dis);
	
		
		vector<City> cities;
		cin >> n;
		
		int tour[n];
		fill(tour, tour+n, 0);
		
		bool visited[n];
		fill(visited, visited+n, false);
		for(int i = 0; i < n; i++)
		{
			City c;
			double x, y;
			cin >> x >> y;
			cities.push_back(c);
		}	
		
		for(int i = 0; i < n; i++)
		{
			unordered_map<int, double> singleDistance;
			for(int j = 0; j < n; j++)
			{
				double d;
				cin >> d;
				if( d != 0)
				{
					
					singleDistance[j] = d;
				}
			}
			listCityHeap.push_back(singleDistance);
		}
		
		
		
		//generate population
		generatePopulation();
		//cout << "Population generated\n";
		generateTour();
		//cout << "Tour generated " << tour_distances.size() << "\n";
		double prev = tour_distances.top().first;
		//cout << "distance = " << tour_distances.top().first << "\n";
		for(int i = 0;; i++)
		{
			rouletteWheel();
			//cout << "wheel generated\n";
			newCrossOver();
			//cout << "Cross over generated\n";
			generateTour();
			//cout << "tour generated\n";
			if(tour_distances.top().first < prev)
			{
				//cout << tour_distances.top().first << "," << i  << "\n";
				prev = tour_distances.top().first;
				for(int i = 0; i < n; i++)
					cout << population[tour_distances.top().second][i] << " ";
				cout << "\n";
				
				
				
			}
			
			
		
		}
		
		
	
}









