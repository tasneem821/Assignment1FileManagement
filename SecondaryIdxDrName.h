//
// Created by tasnim on 12/6/2024.
//

#ifndef ASSIGNMENT1FILEMANAGEMENT_SECONDARYIDXDRNAME_H
#define ASSIGNMENT1FILEMANAGEMENT_SECONDARYIDXDRNAME_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Doctor.h"
using namespace std;

class SecondaryIdxDrName {
    map<string, string> secondaryIdx;
    void rewriteIndexFile() {
        ofstream sIndexFile("secondaryidxName", ios::app);
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
    void addDoctorSecIdx(Doctor doctor) {
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
    void updateDoctorName(string& doctorID, const string& newName) {
        for (auto it = secondaryIdx.begin(); it != secondaryIdx.end(); ++it) {
            if (it->second == doctorID) {
                // Update the doctor's name
                secondaryIdx[newName] = doctorID; // Insert the new entry
                secondaryIdx.erase(it);          // Erase the old entry

                rewriteIndexFile();
                cout << "Doctor name updated successfully for ID: " << doctorID << endl;
                return;
            }
        }
        cout << "Error: Doctor with ID " << doctorID << " not found." << endl;
    }

    void deleteSecondaryIndex(const string& doctorID) {
        for (auto it = secondaryIdx.begin(); it != secondaryIdx.end(); ++it) {
            if (it->second == doctorID) {
                secondaryIdx.erase(it);
                break;
            }
        }
        rewriteIndexFile();
        cout << "Doctor with ID " << doctorID << " deleted from the secondary index." << endl;
    }

};

#endif //ASSIGNMENT1FILEMANAGEMENT_SECONDARYIDXDRNAME_H
