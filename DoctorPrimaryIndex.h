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
    string indexFilename;
    map<string, streampos> primaryIndex;
    void rewriteIndexFile();

public:
    DoctorPrimaryIndex(const string& indexFile) : indexFilename(indexFile) {}
    void addDoctorToIndex(const string& doctorID, streampos position);
    void deleteDoctorFromIndex(const string& doctorID);
    streampos searchDoctorInIndex(const string& doctorID) const;
};
void DoctorPrimaryIndex::addDoctorToIndex(const string& doctorID, streampos position) {
    primaryIndex[doctorID] = position;
    rewriteIndexFile();
    cout << "Doctor ID " << doctorID << " added to the index at position " << position << "." << endl;
}

void DoctorPrimaryIndex::deleteDoctorFromIndex(const string& doctorID) {
    auto it = primaryIndex.find(doctorID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
        return;
    }
    primaryIndex.erase(it);
    rewriteIndexFile();
    cout << "Doctor ID " << doctorID << " deleted from the index." << endl;
}

streampos DoctorPrimaryIndex::searchDoctorInIndex(const string& doctorID) const {
    auto it = primaryIndex.find(doctorID);
    if (it == primaryIndex.end()) {
        cerr << "Error: Doctor ID " << doctorID << " not found in the index!" << endl;
    }

    cout << "Doctor ID " << doctorID << " found at position " << it->second << "." << endl;
    return it->second;
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
