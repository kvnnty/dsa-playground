#pragma once
#include <string>

// Data Structures
struct Patient
{
    int id;
    std::string name, dob, gender;
    Patient *next;
};

struct Doctor
{
    int id;
    std::string name, specialization;
    Doctor *next;
};

struct Appointment
{
    int id, patientId, doctorId;
    std::string date;
    Appointment *next;
};
