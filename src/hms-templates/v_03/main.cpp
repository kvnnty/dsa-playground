#include <iostream>
#include <string>

using namespace std;

// ---------------- Structures ----------------

struct Patient {
    int patient_id;
    string name;
    string dob;
    string gender;
    Patient* next;
};

struct Doctor {
    int doctor_id;
    string name;
    string specialization;
    Doctor* next;
};

struct Appointment {
    int appointment_id;
    int patient_id;
    int doctor_id;
    string appointment_date;
    Appointment* next;
};

// ---------------- Linked List Heads ----------------

Patient* patientsHead = nullptr;
Doctor* doctorsHead = nullptr;
Appointment* appointmentsHead = nullptr;

// ---------------- Helper Functions ----------------

bool patientExists(int id) {
    Patient* current = patientsHead;
    while (current) {
        if (current->patient_id == id) return true;
        current = current->next;
    }
    return false;
}

bool doctorExists(int id) {
    Doctor* current = doctorsHead;
    while (current) {
        if (current->doctor_id == id) return true;
        current = current->next;
    }
    return false;
}

bool appointmentExists(int id) {
    Appointment* current = appointmentsHead;
    while (current) {
        if (current->appointment_id == id) return true;
        current = current->next;
    }
    return false;
}

// ---------------- Core Operations ----------------

void registerPatient() {
    Patient* newPatient = new Patient();
    cout << "Enter patient ID: ";
    cin >> newPatient->patient_id;

    if (patientExists(newPatient->patient_id)) {
        cout << "Error: Patient ID already exists.\n";
        delete newPatient;
        return;
    }

    cin.ignore();
    cout << "Enter name: ";
    getline(cin, newPatient->name);
    cout << "Enter date of birth (YYYY-MM-DD): ";
    getline(cin, newPatient->dob);
    cout << "Enter gender: ";
    getline(cin, newPatient->gender);
    newPatient->next = patientsHead;
    patientsHead = newPatient;
    cout << "Patient registered successfully.\n";
}

void registerDoctor() {
    Doctor* newDoctor = new Doctor();
    cout << "Enter doctor ID: ";
    cin >> newDoctor->doctor_id;

    if (doctorExists(newDoctor->doctor_id)) {
        cout << "Error: Doctor ID already exists.\n";
        delete newDoctor;
        return;
    }

    cin.ignore();
    cout << "Enter name: ";
    getline(cin, newDoctor->name);
    cout << "Enter specialization: ";
    getline(cin, newDoctor->specialization);
    newDoctor->next = doctorsHead;
    doctorsHead = newDoctor;
    cout << "Doctor registered successfully.\n";
}

void registerAppointment() {
    Appointment* newAppt = new Appointment();
    cout << "Enter appointment ID: ";
    cin >> newAppt->appointment_id;

    if (appointmentExists(newAppt->appointment_id)) {
        cout << "Error: Appointment ID already exists.\n";
        delete newAppt;
        return;
    }

    cout << "Enter patient ID: ";
    cin >> newAppt->patient_id;
    cout << "Enter doctor ID: ";
    cin >> newAppt->doctor_id;

    if (!patientExists(newAppt->patient_id) || !doctorExists(newAppt->doctor_id)) {
        cout << "Error: Either patient or doctor ID does not exist.\n";
        delete newAppt;
        return;
    }

    cin.ignore();
    cout << "Enter appointment date (YYYY-MM-DD): ";
    getline(cin, newAppt->appointment_date);
    newAppt->next = appointmentsHead;
    appointmentsHead = newAppt;
    cout << "Appointment registered successfully.\n";
}

void displayPatients() {
    Patient* current = patientsHead;
    if (!current) {
        cout << "No patients found.\n";
        return;
    }
    cout << "\n--- Patients List ---\n";
    while (current) {
        cout << "ID: " << current->patient_id
             << ", Name: " << current->name
             << ", DOB: " << current->dob
             << ", Gender: " << current->gender << "\n";
        current = current->next;
    }
}

void displayDoctors() {
    Doctor* current = doctorsHead;
    if (!current) {
        cout << "No doctors found.\n";
        return;
    }
    cout << "\n--- Doctors List ---\n";
    while (current) {
        cout << "ID: " << current->doctor_id
             << ", Name: " << current->name
             << ", Specialization: " << current->specialization << "\n";
        current = current->next;
    }
}

void displayAppointments() {
    Appointment* current = appointmentsHead;
    if (!current) {
        cout << "No appointments found.\n";
        return;
    }
    cout << "\n--- Appointments List ---\n";
    while (current) {
        cout << "Appointment ID: " << current->appointment_id
             << ", Patient ID: " << current->patient_id
             << ", Doctor ID: " << current->doctor_id
             << ", Date: " << current->appointment_date << "\n";
        current = current->next;
    }
}

// ---------------- Main Menu ----------------

void showMenu() {
    cout << "\n--- Ruhengeri Hospital System ---\n";
    cout << "1. Register patient\n";
    cout << "2. Register doctor\n";
    cout << "3. Register appointment\n";
    cout << "4. Display patients\n";
    cout << "5. Display doctors\n";
    cout << "6. Display appointments\n";
    cout << "7. Exit\n";
    cout << "Select an option: ";
}

int main() {
    int choice;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: registerPatient(); break;
            case 2: registerDoctor(); break;
            case 3: registerAppointment(); break;
            case 4: displayPatients(); break;
            case 5: displayDoctors(); break;
            case 6: displayAppointments(); break;
            case 7: cout << "Exiting... Goodbye.\n"; return 0;
            default: cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
