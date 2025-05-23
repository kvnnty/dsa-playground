#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include "../include/hospital.h"

class Graph
{
public:
    std::unordered_map<std::string, Hospital *> nodes;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adj;
    int nextHospitalIndex = 1;

    Graph();
    ~Graph();

    // Node operations
    void addHospital();
    void deleteHospital();
    void updateHospitalInfo();

    // Connection operations
    void connectHospitals();
    void updateConnection(const std::string &a, const std::string &b);
    void deleteConnection(const std::string &a, const std::string &b);

    // Display
    void listHospitals();
    void displayNetwork();

    // Per-hospital management
    void manageHospital();

private:
    bool areConnected(const std::string &a, const std::string &b);
    std::string genId();
    void loadHospitals();
    void saveHospitals();
    void loadConnections();
    void saveConnections();
};
