#include "../include/hospital.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <type_traits>

Hospital::Hospital() = default;

Hospital::Hospital(const std::string &id, const std::string &nm, const std::string &loc)
    : hospitalId(id), name(nm), location(loc)
{
    loadData();
    normalizeCounters();
}

Hospital::~Hospital()
{
    freeMemory();
}

void Hospital::freeMemory()
{
    // Free patients
    while (patients) {
        Patient* temp = patients;
        patients = patients->next;
        delete temp;
    }

    // Free doctors
    while (doctors) {
        Doctor* temp = doctors;
        doctors = doctors->next;
        delete temp;
    }

    // Free appointments
    while (appointments) {
        Appointment* temp = appointments;
        appointments = appointments->next;
        delete temp;
    }
}

int Hospital::registerPatient(const std::string &n, const std::string &d, const std::string &g)
{
    int id = nextPatientId++;
    patients = new Patient{id, n, d, g, patients};
    saveData();
    return id;
}

int Hospital::registerDoctor(const std::string &n, const std::string &spec)
{
    int id = nextDoctorId++;
    doctors = new Doctor{id, n, spec, doctors};
    saveData();
    return id;
}

int Hospital::registerAppointment(int pid, int did, const std::string &dt)
{
    if (!findPatient(pid) || !findDoctor(did))
        return -1;
    int id = nextAppointmentId++;
    appointments = new Appointment{id, pid, did, dt, appointments};
    saveData();
    return id;
}

void Hospital::displayPatients()
{
    std::cout << "-- Patients in " << name << " (" << hospitalId << ") --\n";
    for (auto *p = patients; p; p = p->next)
        std::cout << p->id << ": " << p->name << " | " << p->dob << " | " << p->gender << "\n";
}

void Hospital::displayDoctors()
{
    std::cout << "-- Doctors in " << name << " (" << hospitalId << ") --\n";
    for (auto *d = doctors; d; d = d->next)
        std::cout << d->id << ": " << d->name << " | " << d->specialization << "\n";
}

void Hospital::displayAppointments()
{
    std::cout << "-- Appointments in " << name << " (" << hospitalId << ") --\n";
    for (auto *a = appointments; a; a = a->next)
        std::cout << a->id << ": P" << a->patientId << " → D" << a->doctorId << " on " << a->date << "\n";
}

Patient *Hospital::findPatient(int id)
{
    for (auto *p = patients; p; p = p->next)
        if (p->id == id)
            return p;
    return nullptr;
}

Doctor *Hospital::findDoctor(int id)
{
    for (auto *d = doctors; d; d = d->next)
        if (d->id == id)
            return d;
    return nullptr;
}

template <typename T>
void Hospital::loadList(const std::string &fn, T *&head)
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
        if constexpr (std::is_same<T, Patient>::value)
        {
            head = new Patient{std::stoi(cols[0]), cols[1], cols[2], cols[3], head};
        }
        else if constexpr (std::is_same<T, Doctor>::value)
        {
            head = new Doctor{std::stoi(cols[0]), cols[1], cols[2], head};
        }
        else
        {
            head = new Appointment{std::stoi(cols[0]), std::stoi(cols[1]), std::stoi(cols[2]), cols[3], head};
        }
    }
}

void Hospital::savePatients(const std::string &fn)
{
    std::ofstream f(fn);
    f << "id,name,dob,gender\n";
    for (auto *p = patients; p; p = p->next)
        f << p->id << ',' << p->name << ',' << p->dob << ',' << p->gender << "\n";
}

void Hospital::saveDoctors(const std::string &fn)
{
    std::ofstream f(fn);
    f << "id,name,specialization\n";
    for (auto *d = doctors; d; d = d->next)
        f << d->id << ',' << d->name << ',' << d->specialization << "\n";
}

void Hospital::saveAppointments(const std::string &fn)
{
    std::ofstream f(fn);
    f << "id,patientId,doctorId,date\n";
    for (auto *a = appointments; a; a = a->next)
        f << a->id << ',' << a->patientId << ',' << a->doctorId << ',' << a->date << "\n";
}

void Hospital::loadData()
{
    loadList<Patient>(hospitalId + "_patients.csv", patients);
    loadList<Doctor>(hospitalId + "_doctors.csv", doctors);
    loadList<Appointment>(hospitalId + "_appointments.csv", appointments);
}

void Hospital::saveData()
{
    savePatients(hospitalId + "_patients.csv");
    saveDoctors(hospitalId + "_doctors.csv");
    saveAppointments(hospitalId + "_appointments.csv");
}

void Hospital::normalizeCounters()
{
    for (auto *p = patients; p; p = p->next)
        nextPatientId = std::max(nextPatientId, p->id + 1);
    for (auto *d = doctors; d; d = d->next)
        nextDoctorId = std::max(nextDoctorId, d->id + 1);
    for (auto *a = appointments; a; a = a->next)
        nextAppointmentId = std::max(nextAppointmentId, a->id + 1);
}
