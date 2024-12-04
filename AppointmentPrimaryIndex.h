//
// Created by mohamed sallam on 12/4/2024.
//
#ifndef APPOINTMENT_PRIMARY_INDEX_H
#define APPOINTMENT_PRIMARY_INDEX_H
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
using namespace std;

class AppointmentPrimaryIndex {
private:
    string indexFilename;                    // File to store the index
    map<string, streampos> primaryIndex;     // Maps Appointment ID -> File position
    vector<streampos> availList;             // Stores positions of deleted records

public:
    AppointmentPrimaryIndex(const string& indexFile) : indexFilename(indexFile) {}

    // Build the primary index from the index file
    void buildPrimaryIndex();

    // Add an appointment to the primary index
    void addAppointmentToIndex(const string& appointmentID, streampos position);

    // Delete an appointment from the primary index
    void deleteAppointmentFromIndex(const string& appointmentID);

    // Update an appointment's position in the primary index
    void updateAppointmentInIndex(const string& appointmentID, streampos newPosition);

    // Search for an appointment in the primary index
    streampos searchAppointmentInIndex(const string& appointmentID) const;

    // Display the primary index and AVAIL LIST
    void displayIndexes() const;
};

// Definitions (similar to DoctorPrimaryIndex)

// Build the primary index from the index file
void AppointmentPrimaryIndex::buildPrimaryIndex() {
    primaryIndex.clear();
    availList.clear();

    ifstream indexFile(indexFilename);
    if (!indexFile.is_open()) {
        cerr << "Error: Could not open index file " << indexFilename << endl;
        return;
    }

    string appointmentID;
    streamoff offset;

    while (indexFile >> appointmentID >> offset) {
        primaryIndex[appointmentID] = static_cast<streampos>(offset);
    }
    indexFile.close();

    cout << "Primary index built successfully!" << endl;
}

// Add an appointment to the primary index
void AppointmentPrimaryIndex::addAppointmentToIndex(const string& appointmentID, streampos position) {
    if (primaryIndex.find(appointmentID) != primaryIndex.end()) {
        cerr << "Error: Appointment ID " << appointmentID << " already exists in the index!" << endl;
        return;
    }

    ofstream primaryIndexFile(indexFilename, ios::app);
    if (!primaryIndexFile.is_open()) {
        cerr << "Error: Could not open index file for writing!" << endl;
        return;
    }

    primaryIndexFile << appointmentID << " " << static_cast<streamoff>(position) << endl;
    primaryIndex[appointmentID] = position;
    primaryIndexFile.close();

    cout << "Appointment ID " << appointmentID << " added to the index at position " << position << "." << endl;
}

// Delete an appointment from the primary index
void AppointmentPrimaryIndex::deleteAppointmentFromIndex(const string& appointmentID) {
    auto it = primaryIndex.find(appointmentID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Appointment ID " << appointmentID << " not found in the index!" << endl;
        return;
    }

    availList.push_back(it->second);
    primaryIndex.erase(it);

    cout << "Appointment ID " << appointmentID << " deleted from the index." << endl;

    // Rewrite the index file
    ofstream indexFile(indexFilename, ios::trunc);
    if (!indexFile.is_open()) {
        cerr << "Error: Could not open index file for rewriting!" << endl;
        return;
    }

    for (const auto& entry : primaryIndex) {
        indexFile << entry.first << " " << static_cast<std::streamoff>(entry.second) << endl;
    }
    indexFile.close();
}

// Update an appointment's position in the primary index
void AppointmentPrimaryIndex::updateAppointmentInIndex(const string& appointmentID, streampos newPosition) {
    auto it = primaryIndex.find(appointmentID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Appointment ID " << appointmentID << " not found in the index!" << endl;
        return;
    }

    it->second = newPosition;

    // Rewrite the index file
    ofstream indexFile(indexFilename, ios::trunc);
    if (!indexFile.is_open()) {
        cerr << "Error: Could not open index file for updating!" << endl;
        return;
    }

    for (const auto& entry : primaryIndex) {
        indexFile << entry.first << " " << static_cast<std::streamoff>(entry.second) << endl;
    }
    indexFile.close();

    cout << "Appointment ID " << appointmentID << " updated to position " << newPosition << "." << endl;
}

// Search for an appointment in the primary index
streampos AppointmentPrimaryIndex::searchAppointmentInIndex(const string& appointmentID) const {
    auto it = primaryIndex.find(appointmentID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Appointment ID " << appointmentID << " not found in the index!" << endl;
        return -1;
    }

    cout << "Appointment ID " << appointmentID << " found at position " << it->second << "." << endl;
    return it->second;
}

// Display the primary index and AVAIL LIST
void AppointmentPrimaryIndex::displayIndexes() const {
    cout << "Primary Index:" << endl;
    for (const auto& entry : primaryIndex) {
        cout << entry.first << " -> " << entry.second << endl;
    }

    cout << "\nAVAIL LIST:" << endl;
    for (const auto& pos : availList) {
        cout << pos << endl;
    }
}


#endif //APPOINTMENT_PRIMARY_INDEX_H
