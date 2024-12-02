// created By Dina
#ifndef APPOINTMENT_SECONDARY_INDEX_H
#define APPOINTMENT_SECONDARY_INDEX_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

// Linked List node for each entry in the secondary index
struct AppointmentNode
{
    string appointmentID;
    string doctorID;
    AppointmentNode *next;

    AppointmentNode(string appID, string docID) : appointmentID(appID), doctorID(docID), next(nullptr) {}
};

// Class for managing the Secondary Index for Appointment
class AppointmentSecondaryIndex
{
private:
    string indexFilename;                          // File to store the secondary index
    map<string, AppointmentNode *> secondaryIndex; // Maps Doctor ID -> Linked List of Appointments

public:
    AppointmentSecondaryIndex(const string &indexFile) : indexFilename(indexFile) {}

    // Build the secondary index from the index file
    void buildSecondaryIndex();

    // Add an appointment to the secondary index
    void addAppointmentToIndex(const string &doctorID, const string &appointmentID);

    // Delete an appointment from the secondary index
    void deleteAppointmentFromIndex(const string &doctorID, const string &appointmentID);

    // Update Doctor ID for a specific appointment
    void updateDoctorIDForAppointment(const string &appointmentID, const string &oldDoctorID, const string &newDoctorID);

    // Binary search to find appointments by doctor ID
    vector<string> binarySearchAppointments(const string &doctorID) const;

    // Search for appointments by Doctor ID in the secondary index and return positions from the primary index
    vector<streampos> searchAppointmentsByDoctor(const string &doctorID, const DoctorPrimaryIndex &primaryIndex) const;

    // Display the secondary index
    void displayIndexes() const;
};

// Build the secondary index from the index file
void AppointmentSecondaryIndex::buildSecondaryIndex()
{
    secondaryIndex.clear();

    ifstream indexFile(indexFilename);
    if (!indexFile.is_open())
    {
        cerr << "Error: Could not open index file " << indexFilename << endl;
        return;
    }

    string doctorID, appointmentID;
    while (indexFile >> doctorID >> appointmentID)
    {
        addAppointmentToIndex(doctorID, appointmentID);
    }
    indexFile.close();

    cout << "Secondary index built successfully!" << endl;
}

// Add an appointment to the secondary index
void AppointmentSecondaryIndex::addAppointmentToIndex(const string &doctorID, const string &appointmentID)
{
    AppointmentNode *newNode = new AppointmentNode(appointmentID, doctorID);
    if (secondaryIndex.find(doctorID) == secondaryIndex.end())
    {
        secondaryIndex[doctorID] = newNode;
    }
    else
    {
        AppointmentNode *current = secondaryIndex[doctorID];
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newNode;
    }

    // Save to file (this example appends at the end of the file)
    ofstream secondaryIndexFile(indexFilename, ios::app);
    if (!secondaryIndexFile.is_open())
    {
        cerr << "Error: Could not open index file for writing!" << endl;
        return;
    }

    secondaryIndexFile << doctorID << " " << appointmentID << endl;
    secondaryIndexFile.close();

    cout << "Appointment ID " << appointmentID << " added for Doctor ID " << doctorID << "." << endl;
}
// Delete an appointment from the secondary index
void AppointmentSecondaryIndex::deleteAppointmentFromIndex(const string &doctorID, const string &appointmentID)
{
    auto it = secondaryIndex.find(doctorID);
    if (it == secondaryIndex.end())
    {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
        return;
    }

    AppointmentNode *current = it->second;
    AppointmentNode *prev = nullptr;
    while (current != nullptr)
    {
        if (current->appointmentID == appointmentID)
        {
            if (prev == nullptr)
            {
                secondaryIndex[doctorID] = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            delete current;

            // Rebuild the file by clearing it and rewriting the index
            ofstream indexFile(indexFilename, ios::trunc);
            if (!indexFile.is_open())
            {
                cerr << "Error: Could not open index file for rewriting!" << endl;
                return;
            }
            for (const auto &entry : secondaryIndex)
            {
                current = entry.second;
                while (current != nullptr)
                {
                    indexFile << entry.first << " " << current->appointmentID << endl;
                    current = current->next;
                }
            }
            indexFile.close();
            cout << "Appointment ID " << appointmentID << " deleted for Doctor ID " << doctorID << "." << endl;
            return;
        }
        prev = current;
        current = current->next;
    }

    cerr << "Error: Appointment ID " << appointmentID << " not found for Doctor ID " << doctorID << "!" << endl;
}

// Update Doctor ID for a specific appointment
void AppointmentSecondaryIndex::updateDoctorIDForAppointment(const string &appointmentID, const string &oldDoctorID, const string &newDoctorID)
{
    deleteAppointmentFromIndex(oldDoctorID, appointmentID);
    addAppointmentToIndex(newDoctorID, appointmentID);
    cout << "Appointment ID " << appointmentID << " updated from Doctor ID " << oldDoctorID
         << " to Doctor ID " << newDoctorID << "." << endl;
}

// Binary search to find appointments by doctor ID in the secondary index
vector<string> AppointmentSecondaryIndex::binarySearchAppointments(const string &doctorID) const
{
    vector<string> appointments;

    auto it = secondaryIndex.find(doctorID);
    if (it != secondaryIndex.end())
    {
        AppointmentNode *current = it->second;
        while (current != nullptr)
        {
            appointments.push_back(current->appointmentID);
            current = current->next;
        }
    }
    return appointments;
}

// Search for appointments by Doctor ID in the secondary index and return positions from the primary index
vector<streampos> AppointmentSecondaryIndex::searchAppointmentsByDoctor(const string &doctorID, const DoctorPrimaryIndex &primaryIndex) const
{
    vector<streampos> primaryPositions;

    // Perform binary search on secondary index
    vector<string> appointmentIDs = binarySearchAppointments(doctorID);

    if (appointmentIDs.empty())
    {
        cerr << "No appointments found for Doctor ID " << doctorID << endl;
        return primaryPositions;
    }

    // For each appointment ID, get the primary key's position from the primary index
    for (const string &appointmentID : appointmentIDs)
    {
        streampos pos = primaryIndex.searchDoctorInIndex(doctorID);
        if (pos != -1)
        {
            primaryPositions.push_back(pos);
        }
    }

    return primaryPositions;
}

// Display the secondary index
void AppointmentSecondaryIndex::displayIndexes() const
{
    cout << "Secondary Index:" << endl;
    for (const auto &entry : secondaryIndex)
    {
        cout << "Doctor ID: " << entry.first << " -> [ ";
        AppointmentNode *current = entry.second;
        while (current != nullptr)
        {
            cout << current->appointmentID << " ";
            current = current->next;
        }
        cout << "]" << endl;
    }

#endif
