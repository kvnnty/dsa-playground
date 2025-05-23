#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <string>

// Data Structures
struct Vehicle
{
    std::string id; // License plate (e.g., "RAD 220 C")
    std::string type, owner;
    Vehicle *next;
};

struct ParkingSpot
{
    int id;
    std::string type;
    bool isOccupied;
    ParkingSpot *next;
};

struct ParkingSession
{
    int id;
    std::string vehicleId; // License plate
    int spotId;
    std::string entryTime, exitTime;
    ParkingSession *next;
};

#endif // DATA_STRUCTURES_H