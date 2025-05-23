#ifndef PARKING_LOT_H
#define PARKING_LOT_H

#include <string>
#include <unordered_set>
#include "data_structures.h"

class ParkingLot
{
public:
    std::string lotId, name, location;
    Vehicle *vehicles = nullptr;
    ParkingSpot *spots = nullptr;
    ParkingSession *sessions = nullptr;

    int nextSpotId = 1;
    int nextSessionId = 1;

    ParkingLot() = default;
    ParkingLot(const std::string &id, const std::string &nm, const std::string &loc);
    ~ParkingLot();

    bool registerVehicle(const std::string &lp, const std::string &t, const std::string &own);
    int addParkingSpot(const std::string &t);
    int startParkingSession(const std::string &vId, int sid, const std::string &entry);
    bool endParkingSession(int sessionId, const std::string &exit);

    // Delete functions
    bool deleteVehicle(const std::string &vId);
    bool deleteSpot(int sid);
    bool deleteSession(int sid);

    void displayVehicles();
    void displaySpots();
    void displaySessions(bool currentOnly = false);

    void loadData();
    void saveData();

private:
    Vehicle *findVehicle(const std::string &vId);
    ParkingSpot *findSpot(int id);
    ParkingSession *findSession(int id);
    void updateSpotStatuses();

    template <typename T>
    void loadList(const std::string &fn, T *&head);
    
    void saveVehicles(const std::string &fn);
    void saveSpots(const std::string &fn);
    void saveSessions(const std::string &fn);
    void normalizeCounters();
    
    // Clean up memory
    void clearVehicles();
    void clearSpots();
    void clearSessions();
};

#endif // PARKING_LOT_H