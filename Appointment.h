//
// Created by tasnim on 11/24/2024.
//

#ifndef ASSIGNMENT_APPOINTMENT_H
#define ASSIGNMENT_APPOINTMENT_H
#include <iostream>
#include <fstream>
#include <map>
#include <cstring> // For strcpy and strcmp
#include <sstream>
#include "vector"

using namespace std;

// Appointment Class
class Appointment {
private:
    char appointmentID[16];  // 15 characters + 1 null terminator
    char appointmentDate[31]; // 30 characters + 1 null terminator
    char doctorID[16];       // 15 characters + 1 null terminator
    vector<streampos>availlist;

public:
    // Constructor
    Appointment() {
        memset(appointmentID, 0, sizeof(appointmentID));
        memset(appointmentDate, 0, sizeof(appointmentDate));
        memset(doctorID, 0, sizeof(doctorID));
    }
    void insert(const string &fileName);
    void deleteRecord(const string&fileName , const string& appointmentid);
    // Setters
    void setAppointmentID(const char* id) {
        strncpy(appointmentID, id, sizeof(appointmentID) - 1);
    }
    void setAppointmentDate(const char* date) {
        strncpy(appointmentDate, date, sizeof(appointmentDate) - 1);
    }
    void setDoctorID(const char* id) {
        strncpy(doctorID, id, sizeof(doctorID) - 1);
    }

    // Getters
    const char* getAppointmentID() const { return appointmentID; }
    const char* getAppointmentDate() const { return appointmentDate; }
    const char* getDoctorID() const { return doctorID; }

    // Static function to parse a record string into an Appointment object
    static Appointment parseRecord(const string& record) {
        Appointment appt;
        size_t pos1 = record.find('|');
        size_t pos2 = record.find('|', pos1 + 1);

        if (pos1 != string::npos && pos2 != string::npos) {
            string id = record.substr(0, pos1);
            string date = record.substr(pos1 + 1, pos2 - pos1 - 1);
            string doctor = record.substr(pos2 + 1);

            appt.setAppointmentID(id.c_str());
            appt.setAppointmentDate(date.c_str());
            appt.setDoctorID(doctor.c_str());
        }

        return appt;
    }

    // Display record
    void display() const {
        cout << "Appointment ID: " << appointmentID << ", "
             << "Appointment Date: " << appointmentDate << ", "
             << "Doctor ID: " << doctorID << endl;
    }
};
void Appointment::insert(const std::string &fileName) {
    stringstream record;
    record<<appointmentID<<"|"<<appointmentDate<<"|"<<doctorID;
    string finialrecord = record.str();
    int recordlenght = finialrecord.length();
    ofstream file;
    file.open(fileName,ios::app);
    file<<recordlenght<<"|"<<finialrecord<<"\n";
    file.close();
}
//delete in doctor data file
void Appointment::deleteRecord(const std::string &fileName, const std::string &appointmentid) {
    fstream file(fileName, ios::in | ios::out);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    bool found = false;
    streampos pos;
    vector<string> lines;

    while (getline(file, line)) {
        pos = file.tellg();
        if (line.length() < 7) continue;

        if (line.substr(3, 4) == appointmentid) {
            found = true;
            line = '*'+line;
            availlist.push_back(pos);
        }
        lines.push_back(line);
    }

    if (!found) {
        cerr << "Error: appointment ID " << appointmentid << " not found!" << endl;
        file.close();
        return;
    }

    file.close();
    ofstream outFile(fileName, ios::trunc);  // Open file in truncate mode (clears the file)

    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << fileName << " for writing!" << endl;
        return;
    }

    for (const auto &line : lines) {
        outFile << line << endl;
    }

    outFile.close();
    cout << "Appointment record with ID " << appointmentid << " deleted successfully!" << endl;
}
#endif //ASSIGNMENT_APPOINTMENT_H
