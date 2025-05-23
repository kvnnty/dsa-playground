#include "../include/parking_lot.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

ParkingLot::ParkingLot(const std::string &id, const std::string &nm, const std::string &loc)
    : lotId(id), name(nm), location(loc)
{
    loadData();
    normalizeCounters();
    updateSpotStatuses();
}

ParkingLot::~ParkingLot()
{
    clearVehicles();
    clearSpots();
    clearSessions();
}

void ParkingLot::clearVehicles()
{
    while (vehicles)
    {
        Vehicle *temp = vehicles;
        vehicles = vehicles->next;
        delete temp;
    }
}

void ParkingLot::clearSpots()
{
    while (spots)
    {
        ParkingSpot *temp = spots;
        spots = spots->next;
        delete temp;
    }
}

void ParkingLot::clearSessions()
{
    while (sessions)
    {
        ParkingSession *temp = sessions;
        sessions = sessions->next;
        delete temp;
    }
}

bool ParkingLot::registerVehicle(const std::string &lp, const std::string &t, const std::string &own)
{
    if (findVehicle(lp))
    {
        std::cout << "Vehicle already exists!\n";
        return false;
    }
    vehicles = new Vehicle{lp, t, own, vehicles};
    saveData();
    return true;
}

int ParkingLot::addParkingSpot(const std::string &t)
{
    int id = nextSpotId++;
    spots = new ParkingSpot{id, t, false, spots};
    saveData();
    return id;
}

int ParkingLot::startParkingSession(const std::string &vId, int sid, const std::string &entry)
{
    if (!findVehicle(vId) || !findSpot(sid))
        return -1;
    ParkingSpot *spot = findSpot(sid);
    if (spot->isOccupied)
        return -2;

    int id = nextSessionId++;
    sessions = new ParkingSession{id, vId, sid, entry, "", sessions};
    spot->isOccupied = true;
    saveData();
    return id;
}

bool ParkingLot::endParkingSession(int sessionId, const std::string &exit)
{
    ParkingSession *session = findSession(sessionId);
    if (!session || session->exitTime != "")
        return false;

    session->exitTime = exit;
    ParkingSpot *spot = findSpot(session->spotId);
    if (spot)
        spot->isOccupied = false;
    saveData();
    return true;
}

bool ParkingLot::deleteVehicle(const std::string &vId)
{
    Vehicle **ptr = &vehicles;
    while (*ptr)
    {
        if ((*ptr)->id == vId)
        {
            // Check for active sessions
            for (auto *s = sessions; s; s = s->next)
            {
                if (s->vehicleId == vId && s->exitTime == "")
                {
                    std::cout << "Cannot delete vehicle with active session\n";
                    return false;
                }
            }
            Vehicle *temp = *ptr;
            *ptr = temp->next;
            delete temp;
            saveData();
            return true;
        }
        ptr = &(*ptr)->next;
    }
    return false;
}

bool ParkingLot::deleteSpot(int sid)
{
    ParkingSpot **ptr = &spots;
    while (*ptr)
    {
        if ((*ptr)->id == sid)
        {
            if ((*ptr)->isOccupied)
            {
                std::cout << "Cannot delete occupied spot\n";
                return false;
            }
            ParkingSpot *temp = *ptr;
            *ptr = temp->next;
            delete temp;
            saveData();
            return true;
        }
        ptr = &(*ptr)->next;
    }
    return false;
}

bool ParkingLot::deleteSession(int sid)
{
    ParkingSession **ptr = &sessions;
    while (*ptr)
    {
        if ((*ptr)->id == sid)
        {
            ParkingSession *temp = *ptr;
            // Free up the parking spot
            ParkingSpot *spot = findSpot(temp->spotId);
            if (spot)
                spot->isOccupied = false;
            *ptr = temp->next;
            delete temp;
            saveData();
            return true;
        }
        ptr = &(*ptr)->next;
    }
    return false;
}

void ParkingLot::displayVehicles()
{
    std::cout << "-- Vehicles in " << name << " (" << lotId << ") --\n";
    for (auto *v = vehicles; v; v = v->next)
        std::cout << "License: " << v->id << " | Type: " << v->type
             << " | Owner: " << v->owner << "\n";
}

void ParkingLot::displaySpots()
{
    std::cout << "-- Parking Spots in " << name << " (" << lotId << ") --\n";
    for (auto *s = spots; s; s = s->next)
        std::cout << s->id << ": " << s->type << " | "
             << (s->isOccupied ? "Occupied" : "Available") << "\n";
}

void ParkingLot::displaySessions(bool currentOnly)
{
    std::cout << "-- Parking Sessions in " << name << " (" << lotId << ") --\n";
    for (auto *s = sessions; s; s = s->next)
    {
        if (!currentOnly || s->exitTime == "")
            std::cout << s->id << ": " << s->vehicleId << " @ S" << s->spotId
                 << " | " << s->entryTime << " - "
                 << (s->exitTime == "" ? "Ongoing" : s->exitTime) << "\n";
    }
}

Vehicle *ParkingLot::findVehicle(const std::string &vId)
{
    for (auto *v = vehicles; v; v = v->next)
        if (v->id == vId)
            return v;
    return nullptr;
}

ParkingSpot *ParkingLot::findSpot(int id)
{
    for (auto *s = spots; s; s = s->next)
        if (s->id == id)
            return s;
    return nullptr;
}

ParkingSession *ParkingLot::findSession(int id)
{
    for (auto *s = sessions; s; s = s->next)
        if (s->id == id)
            return s;
    return nullptr;
}

void ParkingLot::updateSpotStatuses()
{
    std::unordered_set<int> occupiedSpots;
    for (auto *s = sessions; s; s = s->next)
    {
        if (s->exitTime == "")
        {
            occupiedSpots.insert(s->spotId);
        }
    }
    for (auto *spot = spots; spot; spot = spot->next)
    {
        spot->isOccupied = occupiedSpots.count(spot->id);
    }
}

template <typename T>
void ParkingLot::loadList(const std::string &fn, T *&head)
{
    std::ifstream f(fn);
    if (!f)
        return;
    std::string line;
    getline(f, line);
    while (getline(f, line))
    {
        std::stringstream ss(line);
        std::vector<std::string> cols;
        std::string tok;
        while (getline(ss, tok, ','))
            cols.push_back(tok);

        if constexpr (std::is_same<T, Vehicle>::value)
        {
            head = new Vehicle{cols[0], cols[1], cols[2], head};
        }
        else if constexpr (std::is_same<T, ParkingSpot>::value)
        {
            head = new ParkingSpot{std::stoi(cols[0]), cols[1], cols[2] == "1", head};
        }
        else
        {
            head = new ParkingSession{std::stoi(cols[0]), cols[1],
                                      std::stoi(cols[2]), cols[3], cols[4], head};
        }
    }
}

void ParkingLot::saveVehicles(const std::string &fn)
{
    std::ofstream f(fn);
    f << "license_plate,type,owner\n";
    for (auto *v = vehicles; v; v = v->next)
        f << v->id << ',' << v->type << ',' << v->owner << "\n";
}

void ParkingLot::saveSpots(const std::string &fn)
{
    std::ofstream f(fn);
    f << "id,type,is_occupied\n";
    for (auto *s = spots; s; s = s->next)
        f << s->id << ',' << s->type << ',' << (s->isOccupied ? 1 : 0) << "\n";
}

void ParkingLot::saveSessions(const std::string &fn)
{
    std::ofstream f(fn);
    f << "id,vehicle_id,spot_id,entry_time,exit_time\n";
    for (auto *s = sessions; s; s = s->next)
        f << s->id << ',' << s->vehicleId << ',' << s->spotId << ','
          << s->entryTime << ',' << s->exitTime << "\n";
}

void ParkingLot::normalizeCounters()
{
    for (auto *s = spots; s; s = s->next)
        nextSpotId = std::max(nextSpotId, s->id + 1);
    for (auto *s = sessions; s; s = s->next)
        nextSessionId = std::max(nextSessionId, s->id + 1);
}

void ParkingLot::loadData()
{
    loadList<Vehicle>(lotId + "_vehicles.csv", vehicles);
    loadList<ParkingSpot>(lotId + "_spots.csv", spots);
    loadList<ParkingSession>(lotId + "_sessions.csv", sessions);
}

void ParkingLot::saveData()
{
    saveVehicles(lotId + "_vehicles.csv");
    saveSpots(lotId + "_spots.csv");
    saveSessions(lotId + "_sessions.csv");
}