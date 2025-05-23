#ifndef PARKING_NETWORK_H
#define PARKING_NETWORK_H

#include <unordered_map>
#include <vector>
#include <string>
#include "parking_lot.h"

class ParkingNetwork
{
private:
    bool areConnected(const std::string &a, const std::string &b);
    std::string genId();
    void loadLots();
    void saveLots();
    void loadConnections();
    void saveConnections();

public:
    std::unordered_map<std::string, ParkingLot *> nodes;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adj;
    int nextLotIndex = 1;

    ParkingNetwork();
    ~ParkingNetwork();

    void addParkingLot();
    void updateParkingLot();
    void deleteParkingLot();
    void connectParkingLots();
    void listParkingLots();
    void displayNetwork();
    void manageParkingLot();
};

#endif // PARKING_NETWORK_H