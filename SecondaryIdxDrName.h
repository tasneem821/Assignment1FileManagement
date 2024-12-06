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
    vector<pair<string, string>> secondaryIdx;
public:
    // insert doctor name in the index file
    void addDoctorSecIdx(const string&filename,Doctor doctor) {
        ofstream doctorFile(filename, ios::app);

        secondaryIdx.emplace_back(doctor.DoctorName, doctor.DoctorID);
        sort(secondaryIdx.begin(), secondaryIdx.end());
        doctorFile<<doctor.DoctorName<<' '<<doctor.DoctorID<<'\n';
    }


    // search for doctor id with the doctor name
    vector<string> searchByDoctorName(Doctor doctor) {
        vector<string> result;
        for (const auto &pair: secondaryIdx)
            if (pair.first == doctor.DoctorName)  // first -> name, second -> id
                result.push_back(pair.second);
        return result;
    }


    // update doctor name
    void updateDoctorName(string ID, string& newName) {
        for (auto& entry : secondaryIdx) {
            if (entry.second == ID) {
                entry.first = newName;
                break;
            }
        }
        sort(secondaryIdx.begin(), secondaryIdx.end());


    }
void print(const string& filename){
    ofstream outFile(filename, ios::out);  // Open file for writing (overwrite mode)

    if (!outFile) {
        cerr << "Failed to open file: " << filename << endl;
        return;  // If the file fails to open, we exit
    }

    // Iterate over the vector and write each pair to the file
    for (const auto& entry : secondaryIdx) {
        outFile << entry.second << " " << entry.first << "\n";  // ID and Name
    }

    outFile.close();  // Close the file after writing
    }


    // delete a doctor
    void deleteDoctor(const string& ID) {
        for (auto it = secondaryIdx.begin(); it != secondaryIdx.end(); ++it) {
            if (it->second == ID) {
                secondaryIdx.erase(it);
                break;
            }
        }
    }
    //
    void deleteSecondaryIndex(const std::string &DoctorId) {
        fstream sIdxFile("secondaryidxName.txt", ios::in | ios::out);
        if (!sIdxFile.is_open()) {
            cerr << "Error: Could not open secondary index file secondaryidxName.txt" << endl;
            return;
        }

        string line;
        vector<string> sIdxLines;
        bool found = false;
        while (getline(sIdxFile, line)) {
            size_t spacePos = line.find(' ');
            string indexName = line.substr(0, spacePos);
            string indexId = line.substr(spacePos + 1);

            if (indexId == DoctorId) {
                found = true;
                continue;
            }

            sIdxLines.push_back(line);
        }

        sIdxFile.close();

        if (found) {
            ofstream outSIdxFile("secondaryidxName.txt", ios::trunc);
            if (!outSIdxFile.is_open()) {
                cerr << "Error: Could not open secondary index file secondaryidxName.txt for writing!" << endl;
                return;
            }

            for (const auto &line : sIdxLines) {
                outSIdxFile << line << endl;
            }

            outSIdxFile.close();
            cout << "Secondary index updated successfully!" << endl;
        } else {
            cerr << "Error: Doctor ID " << DoctorId << " not found in secondary index!" << endl;
        }
    }
};

#endif //ASSIGNMENT1FILEMANAGEMENT_SECONDARYIDXDRNAME_H
