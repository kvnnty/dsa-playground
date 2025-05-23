#include "../include/parking_network.h"
#include "../include/utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>

ParkingNetwork::ParkingNetwork()
{
    loadLots();
    loadConnections();
}

ParkingNetwork::~ParkingNetwork()
{
    for (auto &kv : nodes)
    {
        delete kv.second;
    }
}

bool ParkingNetwork::areConnected(const std::string &a, const std::string &b)
{
    if (adj.find(a) == adj.end())
        return false;
    for (const auto &edge : adj[a])
        if (edge.first == b)
            return true;
    return false;
}

std::string ParkingNetwork::genId() 
{ 
    return "L" + std::to_string(nextLotIndex++); 
}

void ParkingNetwork::addParkingLot()
{
    std::cout << "Name: ";
    std::string nm;
    getline(std::cin, nm);
    std::cout << "Location: ";
    std::string loc;
    getline(std::cin, loc);
    std::string id = genId();
    nodes[id] = new ParkingLot(id, nm, loc);
    saveLots();
    saveConnections();
    std::cout << "Added: " << id << "\n";
}

void ParkingNetwork::updateParkingLot()
{
    std::cout << "Parking Lot ID to update: ";
    std::string id;
    getline(std::cin, id);
    
    if (!nodes.count(id))
    {
        std::cout << "Parking lot not found.\n";
        return;
    }

    ParkingLot *lot = nodes[id];
    std::cout << "\nCurrent Information:\n";
    std::cout << "ID: " << lot->lotId << "\n";
    std::cout << "Name: " << lot->name << "\n";
    std::cout << "Location: " << lot->location << "\n\n";

    std::cout << "What would you like to update?\n";
    std::cout << "1. Name\n";
    std::cout << "2. Location\n";
    std::cout << "3. Both Name and Location\n";
    
    int choice = readInt("Choose: ", 1, 3);
    
    std::string newName = lot->name;
    std::string newLocation = lot->location;
    
    if (choice == 1 || choice == 3)
    {
        std::cout << "Enter new name (current: " << lot->name << "): ";
        getline(std::cin, newName);
        if (newName.empty())
        {
            std::cout << "Name cannot be empty. Keeping current name.\n";
            newName = lot->name;
        }
    }
    
    if (choice == 2 || choice == 3)
    {
        std::cout << "Enter new location (current: " << lot->location << "): ";
        getline(std::cin, newLocation);
        if (newLocation.empty())
        {
            std::cout << "Location cannot be empty. Keeping current location.\n";
            newLocation = lot->location;
        }
    }
    
    // Update the lot information
    lot->name = newName;
    lot->location = newLocation;
    
    // Save changes to file
    saveLots();
    
    std::cout << "\nParking lot updated successfully!\n";
    std::cout << "Updated Information:\n";
    std::cout << "ID: " << lot->lotId << "\n";
    std::cout << "Name: " << lot->name << "\n";
    std::cout << "Location: " << lot->location << "\n";
}

void ParkingNetwork::deleteParkingLot()
{
    std::cout << "Lot ID to delete: ";
    std::string id;
    getline(std::cin, id);
    if (!nodes.count(id))
    {
        std::cout << "Not found.\n";
        return;
    }

    // Delete associated files
    remove((id + "_vehicles.csv").c_str());
    remove((id + "_spots.csv").c_str());
    remove((id + "_sessions.csv").c_str());

    // Remove from network
    delete nodes[id];
    nodes.erase(id);
    adj.erase(id);

    // Remove from other lots' connections
    for (auto &kv : adj)
    {
        auto &v = kv.second;
        v.erase(remove_if(v.begin(), v.end(),
                          [&](auto &p)
                          { return p.first == id; }),
                v.end());
    }

    saveLots();
    saveConnections();
    std::cout << "Deleted " << id << "\n";
}

void ParkingNetwork::connectParkingLots()
{
    std::cout << "From Lot ID: ";
    std::string a;
    getline(std::cin, a);
    std::cout << "To Lot ID:   ";
    std::string b;
    getline(std::cin, b);
    if (!nodes.count(a) || !nodes.count(b))
    {
        std::cout << "Invalid IDs.\n";
        return;
    }
    if (areConnected(a, b))
    {
        std::cout << "Lots are already connected.\n";
        return;
    }
    int dist = readInt("Distance (meters): ", 0);
    adj[a].push_back({b, dist});
    adj[b].push_back({a, dist});
    saveConnections();
    std::cout << "Connected " << a << " <-> " << b << "\n";
}

void ParkingNetwork::listParkingLots()
{
    std::cout << "-- Parking Lots --\n";
    for (auto &kv : nodes)
        std::cout << kv.first << " | " << kv.second->name
             << " | " << kv.second->location << "\n";
}

void ParkingNetwork::displayNetwork()
{
    std::cout << "-- Parking Network --\n";
    for (auto &kv : adj)
    {
        std::cout << kv.first << " -> ";
        for (auto &e : kv.second)
            std::cout << e.first << "(" << e.second << "m) ";
        std::cout << "\n";
    }
}

void ParkingNetwork::manageParkingLot()
{
    std::cout << "Parking Lot ID: ";
    std::string lid;
    getline(std::cin, lid);
    if (!nodes.count(lid))
    {
        std::cout << "Not found.\n";
        return;
    }
    ParkingLot *lot = nodes[lid];
    while (true)
    {
        std::cout << "\n-- Managing " << lot->name << " (" << lid << ") --\n"
             << "1. Register Vehicle\n"
             << "2. Add Parking Spot\n"
             << "3. Start Parking Session\n"
             << "4. End Parking Session\n"
             << "5. Display Vehicles\n"
             << "6. Display Spots\n"
             << "7. Display Current Sessions\n"
             << "8. Display All Sessions\n"
             << "9. View Connections\n"
             << "10. Delete Vehicle\n"
             << "11. Delete Spot\n"
             << "12. Delete Session\n"
             << "13. Go Back\n";
        int c = readInt("Choose: ", 1, 13);
        if (c == 13)
            break;
        switch (c)
        {
        case 1:
        {
            std::cout << "License Plate: ";
            std::string lp;
            getline(std::cin, lp);
            std::cout << "Vehicle Type: ";
            std::string t;
            getline(std::cin, t);
            std::cout << "Owner Name: ";
            std::string own;
            getline(std::cin, own);
            if (lot->registerVehicle(lp, t, own))
                std::cout << "Vehicle registered\n";
            else
                std::cout << "Registration failed\n";
            break;
        }
        case 2:
        {
            std::cout << "Spot Type: ";
            std::string t;
            getline(std::cin, t);
            std::cout << "Added Spot " << lot->addParkingSpot(t) << "\n";
            break;
        }
        case 3:
        {
            std::cout << "Vehicle License: ";
            std::string vId;
            getline(std::cin, vId);
            int sid = readInt("Spot ID: ", 1);
            time_t now = time(0);
            std::string entry = ctime(&now);
            entry.erase(entry.find('\n'));
            int id = lot->startParkingSession(vId, sid, entry);
            if (id == -1)
                std::cout << "Invalid IDs\n";
            else if (id == -2)
                std::cout << "Spot occupied\n";
            else
                std::cout << "Session started: " << id << "\n";
            break;
        }
        case 4:
        {
            int sid = readInt("Session ID: ", 1);
            time_t now = time(0);
            std::string exit = ctime(&now);
            exit.erase(exit.find('\n'));
            if (lot->endParkingSession(sid, exit))
                std::cout << "Session ended\n";
            else
                std::cout << "Invalid session or already ended\n";
            break;
        }
        case 5:
            lot->displayVehicles();
            break;
        case 6:
            lot->displaySpots();
            break;
        case 7:
            lot->displaySessions(true);
            break;
        case 8:
            lot->displaySessions();
            break;
        case 9:
        {
            std::cout << "-- Connections from " << lid << " --\n";
            for (auto &e : adj[lid])
                std::cout << e.first << "(" << e.second << "m)\n";
            break;
        }
        case 10:
        {
            std::cout << "Vehicle License: ";
            std::string vId;
            getline(std::cin, vId);
            if (lot->deleteVehicle(vId))
                std::cout << "Vehicle deleted\n";
            else
                std::cout << "Delete failed\n";
            break;
        }
        case 11:
        {
            int sid = readInt("Spot ID: ", 1);
            if (lot->deleteSpot(sid))
                std::cout << "Spot deleted\n";
            else
                std::cout << "Delete failed\n";
            break;
        }
        case 12:
        {
            int sid = readInt("Session ID: ", 1);
            if (lot->deleteSession(sid))
                std::cout << "Session deleted\n";
            else
                std::cout << "Delete failed\n";
            break;
        }
        }
    }
}

void ParkingNetwork::loadLots()
{
    std::ifstream f("parking_lots.csv");
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
        nodes[id] = new ParkingLot(id, nm, loc);
    }
    nextLotIndex = maxIdx + 1;
}

void ParkingNetwork::saveLots()
{
    std::ofstream f("parking_lots.csv");
    f << "id,name,location\n";
    for (auto &kv : nodes)
        f << kv.first << ',' << kv.second->name
          << ',' << kv.second->location << "\n";
}

void ParkingNetwork::loadConnections()
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

void ParkingNetwork::saveConnections()
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