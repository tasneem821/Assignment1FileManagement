//
// Created by Enas on 11/28/2024.
//

#ifndef DOCTOR_PRIMARY_INDEX_H
#define DOCTOR_PRIMARY_INDEX_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
using namespace std;

class DoctorPrimaryIndex {
private:
    string filename;
    map<string, streampos> primaryIndex; // Maps Doctor ID -> File position
    vector<streampos> availList;        // Stores positions of deleted records

public:
    DoctorPrimaryIndex(const string& file) : filename(file) {}

    // Build primary index and AVAIL LIST
    void buildPrimaryIndex() {
        primaryIndex.clear();
        availList.clear();

        fstream file(filename, ios::in);

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        streampos pos = file.tellg();

        while (getline(file, line)) {
            if (line.empty()) {
                pos = file.tellg();
                continue;
            }

            if (line[0] == '*') {
                // Deleted record, add position to AVAIL LIST
                availList.push_back(pos);
            } else {
                // Extract Doctor ID and add to primary index
                string doctorID = line.substr(0, line.find('|'));
                primaryIndex[doctorID] = pos;
            }

            pos = file.tellg();
        }

        file.close();
    }

    // Add a new doctor record
    void addDoctor(const string& doctorID, const string& doctorName, const string& address) {
        // Check if Doctor ID already exists
        if (primaryIndex.find(doctorID) != primaryIndex.end()) {
            cerr << "Error: Doctor ID " << doctorID << " already exists!" << endl;
            return;
        }

        fstream file(filename, ios::in | ios::out | ios::app);

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        streampos pos;

        if (!availList.empty()) {
            // Use position from AVAIL LIST
            pos = availList.back();
            availList.pop_back();
            file.seekp(pos);
        } else {
            // Append to end of file
            file.seekp(0, ios::end);
            pos = file.tellp();
        }

        // Write the record
        file << doctorID << "|" << doctorName << "|" << address << endl;

        // Update the primary index
        primaryIndex[doctorID] = pos;

        file.close();
        cout << "Doctor record added successfully!" << endl;
    }

    // Delete a doctor record
    void deleteDoctor(const string& doctorID) {
        // Check if Doctor ID exists
        auto it = primaryIndex.find(doctorID);
        if (it == primaryIndex.end()) {
            cerr << "Error: Doctor ID " << doctorID << " not found!" << endl;
            return;
        }

        fstream file(filename, ios::in | ios::out);

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        // Mark the record as deleted
        file.seekp(it->second);
        file.put('*'); // Overwrite the first character with '*'

        // Update the AVAIL LIST and remove from primary index
        availList.push_back(it->second);
        primaryIndex.erase(it);

        file.close();
        cout << "Doctor record deleted successfully!" << endl;
    }

    // Update a doctor record
    void updateDoctor(const string& doctorID, const string& newName, const string& newAddress) {
        // Check if Doctor ID exists
        auto it = primaryIndex.find(doctorID);
        if (it == primaryIndex.end()) {
            cerr << "Error: Doctor ID " << doctorID << " not found!" << endl;
            return;
        }

        fstream file(filename, ios::in | ios::out);

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        // Construct the updated record
        string updatedRecord = doctorID + "|" + newName + "|" + newAddress;

        // Write the updated record at the same position
        file.seekp(it->second);
        file << updatedRecord << endl;

        file.close();
        cout << "Doctor record updated successfully!" << endl;
    }

    // Search a doctor record by ID
    void searchByDoctorID(const string& doctorID) {
        auto it = primaryIndex.find(doctorID);
        if (it == primaryIndex.end()) {
            cout << "Doctor ID " << doctorID << " not found!" << endl;
            return;
        }

        // Fetch record from file
        fstream file(filename, ios::in);

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        file.seekg(it->second);
        string record;
        getline(file, record);

        cout << "Record Found: " << record << endl;

        file.close();
    }

    // Display the primary index and AVAIL LIST
    void displayIndexes() const {
        cout << "Primary Index:" << endl;
        for (const auto& entry : primaryIndex) {
            cout << entry.first << " -> " << entry.second << endl;
        }

        cout << "\nAVAIL LIST:" << endl;
        for (const auto& pos : availList) {
            cout << pos << endl;
        }
    }
};

#endif // DOCTOR_PRIMARY_INDEX_H
