//
// Created by Nour on 11/26/2024.
//

#ifndef ASSIGNMENT1FILEMANAGEMENT_SECONDARYIDXDRNAME_H
#define ASSIGNMENT1FILEMANAGEMENT_SECONDARYIDXDRNAME_H
#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <algorithm>
#include "Doctor.h"
using namespace std;

class SecondaryIdxDrName {
    map<string, string> secondaryIdx;

    void rewriteIndexFile() {
        ofstream sIndexFile("secondaryidxName", ios::trunc);
        if (!sIndexFile.is_open()) {
            cerr << "Error: Could not open index file for rewriting!" << endl;
            return;
        }

        for (const auto& entry : secondaryIdx) {
            sIndexFile << entry.first << " " << entry.second << endl;
        }
        sIndexFile.close();
    }

public:

    // insert doctor name in the index file
    void addDoctorSecIdx(const string& filename, Doctor doctor) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file '" << filename << "' for reading!" << endl;
            return;
        }
        string line, doctorName, id;
        while (getline(inFile, line)) {
            istringstream iss(line);
            if (iss >> doctorName >> id) {
                secondaryIdx[doctorName] = id;
            }
        }
        inFile.close();
        secondaryIdx[doctor.DoctorName] = doctor.DoctorID;
        rewriteIndexFile();
        cout << "Doctor " << doctor.DoctorName << " added to the secondary index." << endl;
    }



    // search for doctor id with the doctor name
    vector<string> searchByDoctorName(Doctor doctor) {
        vector<string> result;
        for (const auto &pair: secondaryIdx)
            if (pair.first == doctor.DoctorName)
                result.push_back(pair.second);
        return result;
    }


    // update doctor name
    void updateDoctorName(const string& filename, string& doctorID, string& newName) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file '" << filename << "' for reading!" << endl;
            return;
        }
        string line, doctorName, id;
        while (getline(inFile, line)) {
            istringstream iss(line);
            if (iss >> doctorName >> id) {
                secondaryIdx[doctorName] = id;
            }
        }
        inFile.close();

        string oldDoctorName;
        bool found = false;

        for (const auto& entry : secondaryIdx) {
            if (entry.second == doctorID) {
                oldDoctorName = entry.first; // Store the current doctor's name (key)
                found = true;
                break;
            }
        }

        if (found) {
            secondaryIdx[newName] = doctorID; // Add the new name
            secondaryIdx.erase(oldDoctorName);      // Remove the old name
        } else {
            cout << "Error: Doctor with ID '" << doctorID << "' not found!" << endl;
            return;
        }

        rewriteIndexFile();

        cout << "Doctor name updated successfully for ID: " << doctorID << endl;
        }
        
//        for (auto it = secondaryIdx.begin(); it != secondaryIdx.end(); ++it) {
//            if (it->second == doctorID) {
//                // Update the doctor's name
//                secondaryIdx[newName] = doctorID; // Insert the new entry
//                secondaryIdx.erase(it);          // Erase the old entry
//
//                rewriteIndexFile();
//                cout << "Doctor name updated successfully for ID: " << doctorID << endl;
//                return;
//            }
//        }
//        cout << "Error: Doctor with ID '" << doctorID << "' not found." << endl;


//    void updateDoctorName(string& doctorID, const string& newName) {
//        for (auto& entry : secondaryIdx) {
//            if (entry.second == doctorID) {
//                entry.first = newName;
//                break;
//            }
//        }
//        rewriteIndexFile();
//        cout << "Doctor name updated successfully for ID: " << doctorID << endl;
//    }

    void deleteSecondaryIndex(const string& filename, const string& doctorID) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file '" << filename << "' for reading!" << endl;
            return;
        }
        string line, doctorName, id;
        while (getline(inFile, line)) {
            istringstream iss(line);
            if (iss >> doctorName >> id) {
                secondaryIdx[doctorName] = id;
            }
        }
        inFile.close();

        string oldDoctorName;
        bool found = false;

        for (const auto& entry : secondaryIdx) {
            if (entry.second == doctorID) {
                oldDoctorName = entry.first; // Store the current doctor's name (key)
                found = true;
                break;
            }
        }

        if (found) {
            secondaryIdx.erase(oldDoctorName);      // Remove the name
        } else {
            cout << "Error: Doctor with ID '" << doctorID << "' not found!" << endl;
            return;
        }

        rewriteIndexFile();

        cout << "Doctor name Deleted successfully for ID: " << doctorID << endl;
//        for (auto it = secondaryIdx.begin(); it != secondaryIdx.end(); ++it) {
//            if (it->second == doctorID) {
//                secondaryIdx.erase(it);
//                break;
//            }
//        }
//        rewriteIndexFileUpdate();
//        cout << "Doctor with ID " << doctorID << " deleted from the secondary index." << endl;
    }

    string getDoctorNameByID(const string& doctorID) {
        for (const auto& pair : secondaryIdx) {
            if (pair.second == doctorID) { // Match DoctorID
                return pair.first;        // Return DoctorName
            }
        }
        return "Doctor not found"; // If no match is found
    }

 };

#endif //ASSIGNMENT1FILEMANAGEMENT_SECONDARYIDXDRNAME_H
