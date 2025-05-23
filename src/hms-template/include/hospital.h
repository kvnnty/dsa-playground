#pragma once
#include <string>
#include "models.h"

class Hospital
{
public:
    std::string hospitalId, name, location;
    Patient *patients = nullptr;
    Doctor *doctors = nullptr;
    Appointment *appointments = nullptr;

    int nextPatientId = 1;
    int nextDoctorId = 1;
    int nextAppointmentId = 1;

    Hospital();
    Hospital(const std::string &id, const std::string &nm, const std::string &loc);
    ~Hospital();

    int registerPatient(const std::string &n, const std::string &d, const std::string &g);
    int registerDoctor(const std::string &n, const std::string &spec);
    int registerAppointment(int pid, int did, const std::string &dt);

    void displayPatients();
    void displayDoctors();
    void displayAppointments();

    void loadData();
    void saveData();

private:
    Patient *findPatient(int id);
    Doctor *findDoctor(int id);

    template <typename T>
    void loadList(const std::string &fn, T *&head);

    void savePatients(const std::string &fn);
    void saveDoctors(const std::string &fn);
    void saveAppointments(const std::string &fn);

    void normalizeCounters();
    void freeMemory();
};
