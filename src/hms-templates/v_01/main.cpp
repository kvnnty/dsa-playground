#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;

const int MAX = 1000;
const float INF = numeric_limits<float>::infinity();

// Data Structures
struct HealthCenter
{
  int id;
  string name;
  string district;
  float lat;
  float lon;
  int capacity;
};

struct Connection
{
  int to;
  float distance;
  int time;
  string description;
};

// Global Variables
vector<HealthCenter> centers;
vector<Connection> adjList[MAX];

// Utility Functions
void clearInput()
{
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isValidNumber(const string &str)
{
  if (str.empty())
    return false;
  for (char c : str)
  {
    if (!isdigit(c) && c != '.' && c != '-')
      return false;
  }
  return true;
}

int findHealthCenterIndex(int id)
{
  for (int i = 0; i < centers.size(); i++)
  {
    if (centers[i].id == id)
      return i;
  }
  return -1;
}

// File I/O Functions
void readHealthCenters()
{
  ifstream file("health_centers.csv");
  if (!file.is_open())
  {
    // Create file with headers if it doesn't exist
    ofstream newFile("health_centers.csv");
    newFile << "ID,Name,District,Latitude,Longitude,Capacity\n";
    newFile.close();
    cout << "Created new health_centers.csv file\n";
    return;
  }

  string line;
  getline(file, line); // Skip header

  while (getline(file, line))
  {
    if (line.empty())
      continue;

    try
    {
      stringstream ss(line);
      string item;
      HealthCenter center;

      getline(ss, item, ',');
      center.id = stoi(item);

      getline(ss, center.name, ',');
      getline(ss, center.district, ',');

      getline(ss, item, ',');
      center.lat = stof(item);

      getline(ss, item, ',');
      center.lon = stof(item);

      getline(ss, item, ',');
      center.capacity = stoi(item);

      centers.push_back(center);
    }
    catch (const exception &e)
    {
      cout << "Error parsing line: " << line << endl;
    }
  }
  file.close();
}

void readConnections()
{
  ifstream file("connections.csv");
  if (!file.is_open())
  {
    // Create file with headers if it doesn't exist
    ofstream newFile("connections.csv");
    newFile << "FromID,ToID,DistanceKM,TimeMinutes,Description\n";
    newFile.close();
    cout << "Created new connections.csv file\n";
    return;
  }

  string line;
  getline(file, line); // Skip header

  while (getline(file, line))
  {
    if (line.empty())
      continue;

    try
    {
      stringstream ss(line);
      string item;
      int from;
      Connection conn;

      getline(ss, item, ',');
      from = stoi(item);

      getline(ss, item, ',');
      conn.to = stoi(item);

      getline(ss, item, ',');
      conn.distance = stof(item);

      getline(ss, item, ',');
      conn.time = stoi(item);

      getline(ss, conn.description, ',');

      adjList[from].push_back(conn);
    }
    catch (const exception &e)
    {
      cout << "Error parsing connection line: " << line << endl;
    }
  }
  file.close();
}

void saveHealthCenters()
{
  ofstream file("health_centers.csv");
  file << "ID,Name,District,Latitude,Longitude,Capacity\n";

  for (const auto &center : centers)
  {
    file << center.id << "," << center.name << "," << center.district << ","
         << center.lat << "," << center.lon << "," << center.capacity << "\n";
  }
  file.close();
}

void saveConnections()
{
  ofstream file("connections.csv");
  file << "FromID,ToID,DistanceKM,TimeMinutes,Description\n";

  for (int i = 0; i < MAX; i++)
  {
    for (const auto &conn : adjList[i])
    {
      file << i << "," << conn.to << "," << conn.distance << ","
           << conn.time << "," << conn.description << "\n";
    }
  }
  file.close();
}

// CRUD Operations for Health Centers
void addHealthCenter()
{
  HealthCenter center;
  string input;

  cout << "\n--- Add Health Center ---\n";

  // Get ID
  cout << "Enter ID: ";
  cin >> input;
  if (!isValidNumber(input))
  {
    cout << "Invalid ID format!\n";
    return;
  }
  center.id = stoi(input);

  // Check for duplicate ID
  if (findHealthCenterIndex(center.id) != -1)
  {
    cout << "Health Center with ID " << center.id << " already exists!\n";
    return;
  }

  clearInput();

  cout << "Enter Name: ";
  getline(cin, center.name);

  cout << "Enter District: ";
  getline(cin, center.district);

  cout << "Enter Latitude: ";
  cin >> input;
  if (!isValidNumber(input))
  {
    cout << "Invalid latitude format!\n";
    return;
  }
  center.lat = stof(input);

  cout << "Enter Longitude: ";
  cin >> input;
  if (!isValidNumber(input))
  {
    cout << "Invalid longitude format!\n";
    return;
  }
  center.lon = stof(input);

  cout << "Enter Capacity: ";
  cin >> input;
  if (!isValidNumber(input))
  {
    cout << "Invalid capacity format!\n";
    return;
  }
  center.capacity = stoi(input);

  centers.push_back(center);
  saveHealthCenters();
  cout << "Health Center added successfully!\n";
}

void editHealthCenter()
{
  int id;
  cout << "\n--- Edit Health Center ---\n";
  cout << "Enter Health Center ID to edit: ";
  cin >> id;

  int index = findHealthCenterIndex(id);
  if (index == -1)
  {
    cout << "Health Center not found!\n";
    return;
  }

  clearInput();
  string input;

  cout << "Current Name: " << centers[index].name << "\nNew Name (or press Enter to keep): ";
  getline(cin, input);
  if (!input.empty())
    centers[index].name = input;

  cout << "Current District: " << centers[index].district << "\nNew District (or press Enter to keep): ";
  getline(cin, input);
  if (!input.empty())
    centers[index].district = input;

  cout << "Current Latitude: " << centers[index].lat << "\nNew Latitude (or press Enter to keep): ";
  getline(cin, input);
  if (!input.empty() && isValidNumber(input))
    centers[index].lat = stof(input);

  cout << "Current Longitude: " << centers[index].lon << "\nNew Longitude (or press Enter to keep): ";
  getline(cin, input);
  if (!input.empty() && isValidNumber(input))
    centers[index].lon = stof(input);

  cout << "Current Capacity: " << centers[index].capacity << "\nNew Capacity (or press Enter to keep): ";
  getline(cin, input);
  if (!input.empty() && isValidNumber(input))
    centers[index].capacity = stoi(input);

  saveHealthCenters();
  cout << "Health Center updated successfully!\n";
}

void viewHealthCenters()
{
  cout << "\n--- Health Centers ---\n";
  cout << left << setw(5) << "ID" << setw(25) << "Name" << setw(15) << "District"
       << setw(12) << "Latitude" << setw(12) << "Longitude" << setw(10) << "Capacity" << endl;
  cout << string(80, '-') << endl;

  for (const auto &center : centers)
  {
    cout << left << setw(5) << center.id << setw(25) << center.name
         << setw(15) << center.district << setw(12) << fixed << setprecision(4)
         << center.lat << setw(12) << center.lon << setw(10) << center.capacity << endl;
  }
}

void removeHealthCenter()
{
  int id;
  cout << "\n--- Remove Health Center ---\n";
  cout << "Enter Health Center ID to remove: ";
  cin >> id;

  int index = findHealthCenterIndex(id);
  if (index == -1)
  {
    cout << "Health Center not found!\n";
    return;
  }

  // Remove health center
  centers.erase(centers.begin() + index);

  // Remove all connections involving this health center
  adjList[id].clear();
  for (int i = 0; i < MAX; i++)
  {
    adjList[i].erase(
        remove_if(adjList[i].begin(), adjList[i].end(),
                  [id](const Connection &conn)
                  { return conn.to == id; }),
        adjList[i].end());
  }

  saveHealthCenters();
  saveConnections();
  cout << "Health Center removed successfully!\n";
}

// CRUD Operations for Connections
void addConnection()
{
  int from, to;
  Connection conn;
  string input;

  cout << "\n--- Add Connection ---\n";
  cout << "Enter From Health Center ID: ";
  cin >> from;

  if (findHealthCenterIndex(from) == -1)
  {
    cout << "Source Health Center not found!\n";
    return;
  }

  cout << "Enter To Health Center ID: ";
  cin >> to;

  if (findHealthCenterIndex(to) == -1)
  {
    cout << "Destination Health Center not found!\n";
    return;
  }

  conn.to = to;

  cout << "Enter Distance (km): ";
  cin >> input;
  if (!isValidNumber(input))
  {
    cout << "Invalid distance format!\n";
    return;
  }
  conn.distance = stof(input);

  cout << "Enter Time (minutes): ";
  cin >> input;
  if (!isValidNumber(input))
  {
    cout << "Invalid time format!\n";
    return;
  }
  conn.time = stoi(input);

  clearInput();
  cout << "Enter Description: ";
  getline(cin, conn.description);

  adjList[from].push_back(conn);
  saveConnections();
  cout << "Connection added successfully!\n";
}

void viewConnections()
{
  cout << "\n--- Connections ---\n";
  cout << left << setw(8) << "From" << setw(8) << "To" << setw(12) << "Distance"
       << setw(10) << "Time" << setw(20) << "Description" << endl;
  cout << string(60, '-') << endl;

  for (int i = 0; i < MAX; i++)
  {
    for (const auto &conn : adjList[i])
    {
      cout << left << setw(8) << i << setw(8) << conn.to
           << setw(12) << conn.distance << setw(10) << conn.time
           << setw(20) << conn.description << endl;
    }
  }
}

void removeConnection()
{
  int from, to;
  cout << "\n--- Remove Connection ---\n";
  cout << "Enter From Health Center ID: ";
  cin >> from;
  cout << "Enter To Health Center ID: ";
  cin >> to;

  auto &connections = adjList[from];
  auto it = remove_if(connections.begin(), connections.end(),
                      [to](const Connection &conn)
                      { return conn.to == to; });

  if (it != connections.end())
  {
    connections.erase(it, connections.end());
    saveConnections();
    cout << "Connection removed successfully!\n";
  }
  else
  {
    cout << "Connection not found!\n";
  }
}

// Graph Algorithms
void dijkstra()
{
  int start, end;
  cout << "\n--- Dijkstra's Shortest Path ---\n";
  cout << "Enter start Health Center ID: ";
  cin >> start;
  cout << "Enter end Health Center ID: ";
  cin >> end;

  if (findHealthCenterIndex(start) == -1 || findHealthCenterIndex(end) == -1)
  {
    cout << "Invalid Health Center ID!\n";
    return;
  }

  vector<float> dist(MAX, INF);
  vector<int> parent(MAX, -1);
  priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;

  dist[start] = 0;
  pq.push({0, start});

  while (!pq.empty())
  {
    int u = pq.top().second;
    pq.pop();

    for (const auto &conn : adjList[u])
    {
      int v = conn.to;
      float weight = conn.distance;

      if (dist[u] + weight < dist[v])
      {
        dist[v] = dist[u] + weight;
        parent[v] = u;
        pq.push({dist[v], v});
      }
    }
  }

  if (dist[end] == INF)
  {
    cout << "No path found!\n";
    return;
  }

  cout << "Shortest distance: " << dist[end] << " km\n";
  cout << "Path: ";

  vector<int> path;
  int current = end;
  while (current != -1)
  {
    path.push_back(current);
    current = parent[current];
  }

  reverse(path.begin(), path.end());
  for (int i = 0; i < path.size(); i++)
  {
    cout << path[i];
    if (i < path.size() - 1)
      cout << " -> ";
  }
  cout << endl;
}

void bfsTraversal()
{
  int start;
  cout << "\n--- BFS Traversal ---\n";
  cout << "Enter starting Health Center ID: ";
  cin >> start;

  if (findHealthCenterIndex(start) == -1)
  {
    cout << "Health Center not found!\n";
    return;
  }

  vector<bool> visited(MAX, false);
  queue<int> q;

  q.push(start);
  visited[start] = true;

  cout << "BFS Traversal: ";
  while (!q.empty())
  {
    int current = q.front();
    q.pop();
    cout << current << " ";

    for (const auto &conn : adjList[current])
    {
      if (!visited[conn.to])
      {
        visited[conn.to] = true;
        q.push(conn.to);
      }
    }
  }
  cout << endl;
}

bool dfsUtil(int v, vector<bool> &visited, vector<bool> &recStack)
{
  visited[v] = true;
  recStack[v] = true;

  for (const auto &conn : adjList[v])
  {
    if (!visited[conn.to] && dfsUtil(conn.to, visited, recStack))
    {
      return true;
    }
    else if (recStack[conn.to])
    {
      return true;
    }
  }

  recStack[v] = false;
  return false;
}

void detectCycle()
{
  cout << "\n--- Cycle Detection ---\n";

  vector<bool> visited(MAX, false);
  vector<bool> recStack(MAX, false);

  for (const auto &center : centers)
  {
    if (!visited[center.id])
    {
      if (dfsUtil(center.id, visited, recStack))
      {
        cout << "Cycle detected in the network!\n";
        return;
      }
    }
  }

  cout << "No cycle detected in the network.\n";
}

void floydWarshall()
{
  cout << "\n--- Floyd-Warshall All-Pairs Shortest Path ---\n";

  vector<vector<float>> dist(MAX, vector<float>(MAX, INF));

  // Initialize distances
  for (int i = 0; i < MAX; i++)
  {
    dist[i][i] = 0;
  }

  for (int i = 0; i < MAX; i++)
  {
    for (const auto &conn : adjList[i])
    {
      dist[i][conn.to] = conn.distance;
    }
  }

  // Floyd-Warshall algorithm
  for (int k = 0; k < MAX; k++)
  {
    for (int i = 0; i < MAX; i++)
    {
      for (int j = 0; j < MAX; j++)
      {
        if (dist[i][k] != INF && dist[k][j] != INF)
        {
          dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }

  cout << "All-pairs shortest distances:\n";
  cout << left << setw(8) << "From" << setw(8) << "To" << setw(12) << "Distance" << endl;
  cout << string(30, '-') << endl;

  for (const auto &from : centers)
  {
    for (const auto &to : centers)
    {
      if (from.id != to.id && dist[from.id][to.id] != INF)
      {
        cout << left << setw(8) << from.id << setw(8) << to.id
             << setw(12) << dist[from.id][to.id] << endl;
      }
    }
  }
}

void primMST()
{
  cout << "\n--- Prim's Minimum Spanning Tree ---\n";

  if (centers.empty())
  {
    cout << "No health centers available!\n";
    return;
  }

  int start = centers[0].id;
  vector<bool> inMST(MAX, false);
  vector<float> key(MAX, INF);
  vector<int> parent(MAX, -1);

  priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;

  key[start] = 0;
  pq.push({0, start});

  float totalWeight = 0;
  cout << "MST Edges:\n";
  cout << left << setw(8) << "From" << setw(8) << "To" << setw(12) << "Weight" << endl;
  cout << string(30, '-') << endl;

  while (!pq.empty())
  {
    int u = pq.top().second;
    pq.pop();

    if (inMST[u])
      continue;

    inMST[u] = true;

    if (parent[u] != -1)
    {
      cout << left << setw(8) << parent[u] << setw(8) << u
           << setw(12) << key[u] << endl;
      totalWeight += key[u];
    }

    for (const auto &conn : adjList[u])
    {
      int v = conn.to;
      if (!inMST[v] && conn.distance < key[v])
      {
        key[v] = conn.distance;
        parent[v] = u;
        pq.push({key[v], v});
      }
    }
  }

  cout << "Total MST weight: " << totalWeight << " km\n";
}

void emergencyRouting()
{
  int start, minCapacity;
  cout << "\n--- Emergency Routing ---\n";
  cout << "Enter current location (Health Center ID): ";
  cin >> start;
  cout << "Enter minimum required capacity: ";
  cin >> minCapacity;

  if (findHealthCenterIndex(start) == -1)
  {
    cout << "Invalid starting Health Center ID!\n";
    return;
  }

  vector<float> dist(MAX, INF);
  priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;

  dist[start] = 0;
  pq.push({0, start});

  int nearestCenter = -1;
  float nearestDistance = INF;

  while (!pq.empty())
  {
    int u = pq.top().second;
    float d = pq.top().first;
    pq.pop();

    if (d > dist[u])
      continue;

    // Check if this center meets capacity requirement
    int centerIndex = findHealthCenterIndex(u);
    if (centerIndex != -1 && centers[centerIndex].capacity >= minCapacity && u != start)
    {
      nearestCenter = u;
      nearestDistance = dist[u];
      break;
    }

    for (const auto &conn : adjList[u])
    {
      int v = conn.to;
      float weight = conn.distance;

      if (dist[u] + weight < dist[v])
      {
        dist[v] = dist[u] + weight;
        pq.push({dist[v], v});
      }
    }
  }

  if (nearestCenter == -1)
  {
    cout << "No health center found with capacity >= " << minCapacity << endl;
  }
  else
  {
    int centerIndex = findHealthCenterIndex(nearestCenter);
    cout << "Nearest suitable health center:\n";
    cout << "ID: " << nearestCenter << endl;
    cout << "Name: " << centers[centerIndex].name << endl;
    cout << "Capacity: " << centers[centerIndex].capacity << endl;
    cout << "Distance: " << nearestDistance << " km\n";
  }
}

void viewRelationships()
{
  cout << "\n--- Health Center Relationships ---\n";

  ofstream file("relationship_table.csv");
  file << "HealthCenterID,HealthCenterName,ConnectedTo,Distance,TravelTime,Description\n";

  cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(12) << "Connected To"
       << setw(10) << "Distance" << setw(10) << "Time" << setw(15) << "Description" << endl;
  cout << string(80, '-') << endl;

  for (const auto &center : centers)
  {
    if (adjList[center.id].empty())
    {
      cout << left << setw(5) << center.id << setw(20) << center.name
           << setw(12) << "None" << endl;
      file << center.id << "," << center.name << ",None,,,\n";
    }
    else
    {
      for (const auto &conn : adjList[center.id])
      {
        cout << left << setw(5) << center.id << setw(20) << center.name
             << setw(12) << conn.to << setw(10) << conn.distance
             << setw(10) << conn.time << setw(15) << conn.description << endl;
        file << center.id << "," << center.name << "," << conn.to << ","
             << conn.distance << "," << conn.time << "," << conn.description << "\n";
      }
    }
  }

  file.close();
  cout << "Relationship table saved to relationship_table.csv\n";
}

void displayMenu()
{
  cout << "\n==== Health Center Network System ====\n";
  cout << "1. Add Health Center\n";
  cout << "2. Edit Health Center\n";
  cout << "3. View Health Centers\n";
  cout << "4. Remove Health Center\n";
  cout << "5. Add Connection\n";
  cout << "6. Edit Connection\n";
  cout << "7. View Connections\n";
  cout << "8. Remove Connection\n";
  cout << "9. View Relationships\n";
  cout << "10. Dijkstra's Shortest Path\n";
  cout << "11. BFS Traversal\n";
  cout << "12. Detect Cycle\n";
  cout << "13. Floyd-Warshall All-Pairs\n";
  cout << "14. Prim's MST\n";
  cout << "15. Emergency Routing\n";
  cout << "0. Exit\n";
  cout << "Enter choice: ";
}

int main()
{
  // Load data from files
  readHealthCenters();
  readConnections();

  cout << "Health Center Network Management System loaded successfully!\n";
  cout << "Loaded " << centers.size() << " health centers.\n";

  int choice;
  while (true)
  {
    displayMenu();

    if (!(cin >> choice))
    {
      cout << "Invalid input! Please enter a number.\n";
      clearInput();
      continue;
    }

    switch (choice)
    {
    case 1:
      addHealthCenter();
      break;
    case 2:
      editHealthCenter();
      break;
    case 3:
      viewHealthCenters();
      break;
    case 4:
      removeHealthCenter();
      break;
    case 5:
      addConnection();
      break;
    case 6:
      cout << "Edit Connection feature - Please remove and re-add the connection.\n";
      break;
    case 7:
      viewConnections();
      break;
    case 8:
      removeConnection();
      break;
    case 9:
      viewRelationships();
      break;
    case 10:
      dijkstra();
      break;
    case 11:
      bfsTraversal();
      break;
    case 12:
      detectCycle();
      break;
    case 13:
      floydWarshall();
      break;
    case 14:
      primMST();
      break;
    case 15:
      emergencyRouting();
      break;
    case 0:
      cout << "Thank you for using the Health Center Network System!\n";
      return 0;
    default:
      cout << "Invalid choice! Please try again.\n";
    }
  }

  return 0;
}