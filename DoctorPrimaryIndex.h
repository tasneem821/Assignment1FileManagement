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

    // Internal helper to rewrite the index file
    void rewriteIndexFile();

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
void DoctorPrimaryIndex::addDoctorToIndex(const string& doctorID, streampos position) {
    if (primaryIndex.find(doctorID) != primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " already exists in the index!" << endl;
        return;
    }
    if (!availList.empty()) {
        position = availList.back();
        availList.pop_back();
    }

    ofstream primaryIndexFile(indexFilename, ios::app);
    if (!primaryIndexFile.is_open()) {
        cerr << "Error: Could not open index file for writing!" << endl;
        return;
    }

    primaryIndexFile << doctorID << " " << static_cast<streamoff>(position) << endl;
    primaryIndexFile.close();

    cout << "Doctor ID " << doctorID << " added to the index at position " << position << "." << endl;
}

void DoctorPrimaryIndex::deleteDoctorFromIndex(const std::string &doctorID)  {
    fstream pIdxFile("doctor_primary_index.txt", ios::in | ios::out);
    if (!pIdxFile.is_open()) {
        cerr << "Error: Could not open primary index file doctor_primary_index.txt" << endl;
        return;
    }

    string line;
    vector<string> pIdxLines;
    while (getline(pIdxFile, line)) {
        size_t spacePos = line.find(' ');
        string indexId = line.substr(0, spacePos);

        if (indexId != doctorID) {
            pIdxLines.push_back(line);
        }
    }

    pIdxFile.close();
    ofstream outPIdxFile("doctor_primary_index.txt", ios::trunc);
    if (!outPIdxFile.is_open()) {
        cerr << "Error: Could not open primary index file doctor_primary_index.txt for writing!" << endl;
        return;
    }

    for (const auto &line : pIdxLines) {
        outPIdxFile << line << endl;
    }

    outPIdxFile.close();
    cout << "Primary index updated successfully!" << endl;
}

void DoctorPrimaryIndex::updateDoctorInIndex(const string& doctorID, streampos newPosition) {
    auto it = primaryIndex.find(doctorID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
        return;
    }

    it->second = newPosition;
    cout << "Doctor ID " << doctorID << " updated to position " << newPosition << "." << endl;
    rewriteIndexFile();
}

streampos DoctorPrimaryIndex::searchDoctorInIndex(const string& doctorID) const {
    auto it = primaryIndex.find(doctorID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
    }

    cout << "Doctor ID " << doctorID << " found at position " << it->second << "." << endl;
    return it->second;
}

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

void DoctorPrimaryIndex::rewriteIndexFile() {
    ofstream indexFile(indexFilename, ios::trunc);
    if (!indexFile.is_open()) {
        cerr << "Error: Could not open index file for rewriting!" << endl;
        return;
    }

    for (const auto& entry : primaryIndex) {
        indexFile << entry.first << " " << static_cast<streamoff>(entry.second) << endl;
    }
    indexFile.close();
}

#endif // DOCTOR_PRIMARY_INDEX_H
