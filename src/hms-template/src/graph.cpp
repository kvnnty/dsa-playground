#include "../include/graph.h"
#include "../include/utility.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Graph::Graph()
{
    loadHospitals();
    loadConnections();
}

Graph::~Graph()
{
    // Free all hospital objects
    for (auto& pair : nodes) {
        delete pair.second;
    }
}

bool Graph::areConnected(const std::string &a, const std::string &b)
{
    if (adj.find(a) == adj.end())
        return false;
    for (const auto &edge : adj[a])
    {
        if (edge.first == b)
        {
            return true;
        }
    }
    return false;
}

void Graph::addHospital()
{
    std::cout << "Name: ";
    std::string nm;
    getline(std::cin, nm);
    std::cout << "Location: ";
    std::string loc;
    getline(std::cin, loc);
    std::string id = genId();
    nodes[id] = new Hospital(id, nm, loc);
    saveHospitals();
    saveConnections();
    std::cout << "Added: " << id << "\n";
}

void Graph::deleteHospital()
{
    std::cout << "Hospital ID to delete: ";
    std::string id;
    getline(std::cin, id);
    if (!nodes.count(id))
    {
        std::cout << "Not found.\n";
        return;
    }
    delete nodes[id];
    nodes.erase(id);
    adj.erase(id);
    for (auto &kv : adj)
    {
        auto &v = kv.second;
        v.erase(std::remove_if(v.begin(), v.end(),
                          [&](auto &p)
                          { return p.first == id; }),
                v.end());
    }
    saveHospitals();
    saveConnections();
    std::cout << "Deleted " << id << "\n";
}

void Graph::updateHospitalInfo()
{
    std::cout << "Hospital ID: ";
    std::string id;
    getline(std::cin, id);
    if (!nodes.count(id))
    {
        std::cout << "Not found.\n";
        return;
    }
    std::cout << "New Name (blank=keep): ";
    std::string nm;
    getline(std::cin, nm);
    std::cout << "New Location (blank=keep): ";
    std::string loc;
    getline(std::cin, loc);
    if (!nm.empty())
        nodes[id]->name = nm;
    if (!loc.empty())
        nodes[id]->location = loc;
    saveHospitals();
    std::cout << "Updated " << id << "\n";
}

void Graph::connectHospitals()
{
    std::cout << "From ID: ";
    std::string a;
    getline(std::cin, a);
    std::cout << "To ID:   ";
    std::string b;
    getline(std::cin, b);
    if (!nodes.count(a) || !nodes.count(b))
    {
        std::cout << "Invalid IDs.\n";
        return;
    }
    if (areConnected(a, b))
    {
        std::cout << "Hospitals are already connected.\n";
        return;
    }
    int dist = readInt("Distance (km): ", 0);
    adj[a].push_back({b, dist});
    adj[b].push_back({a, dist});
    saveConnections();
    std::cout << "Connected " << a << " <-> " << b << "\n";
}

void Graph::updateConnection(const std::string &a, const std::string &b)
{
    if (!nodes.count(a) || !nodes.count(b))
    {
        std::cout << "Invalid IDs.\n";
        return;
    }
    auto &va = adj[a], &vb = adj[b];
    int idxA = -1, idxB = -1;
    for (int i = 0; i < va.size(); ++i)
        if (va[i].first == b)
            idxA = i;
    for (int i = 0; i < vb.size(); ++i)
        if (vb[i].first == a)
            idxB = i;
    if (idxA < 0 || idxB < 0)
    {
        std::cout << "No connection found.\n";
        return;
    }
    int nd = readInt("New distance (km): ", 0);
    va[idxA].second = vb[idxB].second = nd;
    saveConnections();
    std::cout << "Updated " << a << "<->" << b << " to " << nd << "km\n";
}

void Graph::deleteConnection(const std::string &a, const std::string &b)
{
    if (!nodes.count(a) || !nodes.count(b))
    {
        std::cout << "Invalid IDs.\n";
        return;
    }
    auto &va = adj[a], &vb = adj[b];
    va.erase(std::remove_if(va.begin(), va.end(),
                       [&](auto &p)
                       { return p.first == b; }),
             va.end());
    vb.erase(std::remove_if(vb.begin(), vb.end(),
                       [&](auto &p)
                       { return p.first == a; }),
             vb.end());
    saveConnections();
    std::cout << "Removed connection " << a << " <-> " << b << "\n";
}

void Graph::listHospitals()
{
    std::cout << "-- Hospitals --\n";
    for (auto &kv : nodes)
        std::cout << kv.first << " | " << kv.second->name << " | " << kv.second->location << "\n";
}

void Graph::displayNetwork()
{
    std::cout << "-- Network --\n";
    for (auto &kv : adj)
    {
        std::cout << kv.first << " -> ";
        for (auto &e : kv.second)
            std::cout << e.first << "(" << e.second << "km) ";
        std::cout << "\n";
    }
}

void Graph::manageHospital()
{
    std::cout << "Hospital ID: ";
    std::string hid;
    getline(std::cin, hid);
    if (!nodes.count(hid))
    {
        std::cout << "Not found.\n";
        return;
    }
    Hospital *h = nodes[hid];
    while (true)
    {
        std::cout << "\n-- Managing " << h->name << " (" << hid << ") --\n"
             << "1. Register Patient\n"
             << "2. Register Doctor\n"
             << "3. Register Appointment\n"
             << "4. Display Patients\n"
             << "5. Display Doctors\n"
             << "6. Display Appointments\n"
             << "7. View Connections\n"
             << "8. Add Connection\n"
             << "9. Update Connection\n"
             << "10.Delete Connection\n"
             << "11.Go Back\n";
        int c = readInt("Choose: ", 1, 11);
        if (c == 11)
            break;
        switch (c)
        {
        case 1:
        {
            std::cout << "Name: ";
            std::string n;
            getline(std::cin, n);
            std::cout << "DOB: ";
            std::string d;
            getline(std::cin, d);
            std::cout << "Gender: ";
            std::string g;
            getline(std::cin, g);
            std::cout << "Added Patient " << h->registerPatient(n, d, g) << "\n";
            break;
        }
        case 2:
        {
            std::cout << "Name: ";
            std::string n;
            getline(std::cin, n);
            std::cout << "Spec: ";
            std::string s;
            getline(std::cin, s);
            std::cout << "Added Doctor " << h->registerDoctor(n, s) << "\n";
            break;
        }
        case 3:
        {
            int pid = readInt("Patient ID: ", 1), did = readInt("Doctor ID: ", 1);
            std::cout << "Date: ";
            std::string dt;
            getline(std::cin, dt);
            int id = h->registerAppointment(pid, did, dt);
            std::cout << (id < 0 ? "Invalid IDs" : "Added Appointment " + std::to_string(id)) << "\n";
            break;
        }
        case 4:
            h->displayPatients();
            break;
        case 5:
            h->displayDoctors();
            break;
        case 6:
            h->displayAppointments();
            break;
        case 7:
        {
            std::cout << "-- Connections from " << hid << " --\n";
            for (auto &e : adj[hid])
                std::cout << e.first << "(" << e.second << "km)\n";
            break;
        }
        case 8:
        {
            std::cout << "Connect to ID: ";
            std::string other;
            getline(std::cin, other);
            if (!nodes.count(other))
            {
                std::cout << "Invalid ID.\n";
                break;
            }
            if (areConnected(hid, other))
            {
                std::cout << "Hospitals are already connected.\n";
                break;
            }
            int dist = readInt("Distance (km): ", 0);
            adj[hid].push_back({other, dist});
            adj[other].push_back({hid, dist});
            saveConnections();
            std::cout << "Connected.\n";
            break;
        }
        case 9:
        {
            std::cout << "Update connection to: ";
            std::string other;
            getline(std::cin, other);
            updateConnection(hid, other);
            break;
        }
        case 10:
        {
            std::cout << "Delete connection to: ";
            std::string other;
            getline(std::cin, other);
            deleteConnection(hid, other);
            break;
        }
        }
    }
}

std::string Graph::genId()
{
    return "H" + std::to_string(nextHospitalIndex++);
}

void Graph::loadHospitals()
{
    std::ifstream f("hospitals.csv");
    if (!f)
        return;
    std::string line;
    getline(f, line);
    int maxIdx = 0;
    while (getline(f, line))
    {
        std::stringstream ss(line);
        std::string id, nm, loc;
        getline(ss, id, ',');
        getline(ss, nm, ',');
        getline(ss, loc, ',');
        maxIdx = std::max(maxIdx, std::stoi(id.substr(1)));
        nodes[id] = new Hospital(id, nm, loc);
    }
    nextHospitalIndex = maxIdx + 1;
}

void Graph::saveHospitals()
{
    std::ofstream f("hospitals.csv");
    f << "id,name,location\n";
    for (auto &kv : nodes)
        f << kv.first << ',' << kv.second->name << ',' << kv.second->location << "\n";
}

void Graph::loadConnections()
{
    std::ifstream f("connections.csv");
    if (!f)
        return;
    std::string line;
    getline(f, line);
    while (getline(f, line))
    {
        std::stringstream ss(line);
        std::string a, b;
        int d;
        getline(ss, a, ',');
        getline(ss, b, ',');
        ss >> d;
        if (nodes.count(a) && nodes.count(b))
        {
            adj[a].push_back({b, d});
            adj[b].push_back({a, d});
        }
    }
}

void Graph::saveConnections()
{
    std::ofstream f("connections.csv");
    f << "from,to,distance\n";
    for (auto &kv : adj)
    {
        const std::string &a = kv.first;
        for (auto &e : kv.second)
        {
            const std::string &b = e.first;
            int d = e.second;
            if (a < b)
                f << a << ',' << b << ',' << d << "\n";
        }
    }
}
