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
    string indexFilename;                    // File to store the index
    map<string, streampos> primaryIndex;     // Maps Doctor ID -> File position
    vector<streampos> availList;             // Stores positions of deleted records

public:
    DoctorPrimaryIndex(const string& indexFile) : indexFilename(indexFile) {}

    // Build the primary index from the index file
    void buildPrimaryIndex();

    // Add a doctor to the primary index
    void addDoctorToIndex(const string& doctorID, streampos position);

    // Delete a doctor from the primary index
    void deleteDoctorFromIndex(const string& doctorID);

    // Update a doctor's position in the primary index
    void updateDoctorInIndex(const string& doctorID, streampos newPosition);

    // Search for a doctor in the primary index
    streampos searchDoctorInIndex(const string& doctorID) const;

    // Display the primary index and AVAIL LIST
    void displayIndexes() const;
};

// Build the primary index from the index file
void DoctorPrimaryIndex::buildPrimaryIndex() {
    primaryIndex.clear();
    availList.clear();

    ifstream indexFile(indexFilename);
    if (!indexFile.is_open()) {
        cerr << "Error: Could not open index file " << indexFilename << endl;
        return;
    }

    string doctorID;
    streamoff offset; // Intermediate type for reading positions

    while (indexFile >> doctorID >> offset) {
        primaryIndex[doctorID] = static_cast<std::streampos>(offset);
    }
    indexFile.close();

    cout << "Primary index built successfully!" << endl;
}

// Add a doctor to the primary index
void DoctorPrimaryIndex::addDoctorToIndex(const string& doctorID, streampos position) {
    if (primaryIndex.find(doctorID) != primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " already exists in the index!" << endl;
        return;
    }

    ofstream primaryIndexFile(indexFilename, ios::app);
    if (!primaryIndexFile.is_open()) {
        cerr << "Error: Could not open index file for writing!" << endl;
        return;
    }

    primaryIndexFile << doctorID << " " << static_cast<streamoff>(position) << endl;
    primaryIndex[doctorID] = position;
    primaryIndexFile.close();

    cout << "Doctor ID " << doctorID << " added to the index at position " << position << "." << endl;
}

// Delete a doctor from the primary index
void DoctorPrimaryIndex::deleteDoctorFromIndex(const string& doctorID) {
    auto it = primaryIndex.find(doctorID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
        return;
    }

    availList.push_back(it->second); // Add position to AVAIL LIST
    primaryIndex.erase(it);          // Remove entry from the index

    cout << "Doctor ID " << doctorID << " deleted from the index." << endl;

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

// Update a doctor's position in the primary index
void DoctorPrimaryIndex::updateDoctorInIndex(const string& doctorID, streampos newPosition) {
    auto it = primaryIndex.find(doctorID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
        return;
    }

    it->second = newPosition; // Update the position in memory

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

    cout << "Doctor ID " << doctorID << " updated to position " << newPosition << "." << endl;
}

// Search for a doctor in the primary index
streampos DoctorPrimaryIndex::searchDoctorInIndex(const string& doctorID) const {
    auto it = primaryIndex.find(doctorID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
        return -1; // Indicate that the doctor is not found
    }

    cout << "Doctor ID " << doctorID << " found at position " << it->second << "." << endl;
    return it->second;
}

// Display the primary index and AVAIL LIST
void DoctorPrimaryIndex::displayIndexes() const {
    cout << "Primary Index:" << endl;
    for (const auto& entry : primaryIndex) {
        cout << entry.first << " -> " << entry.second << endl;
    }

    cout << "\nAVAIL LIST:" << endl;
    for (const auto& pos : availList) {
        cout << pos << endl;
    }
}

#endif // DOCTOR_PRIMARY_INDEX_H
