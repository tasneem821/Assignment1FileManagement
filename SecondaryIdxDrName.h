#ifndef ASSIGNMENT1_SECONDARYIDXDRNAME_H
#define ASSIGNMENT1_SECONDARYIDXDRNAME_H

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
    void addDoctor(Doctor doctor) {
        secondaryIdx.emplace_back(doctor.DoctorName, doctor.DoctorID);
        sort(secondaryIdx.begin(), secondaryIdx.end());
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
    void updateDoctorName(Doctor doctor, string& newName) {
        for (auto& entry : secondaryIdx) {
            if (entry.second == doctor.DoctorID) {
                entry.first = newName;
                break;
            }
        }
        sort(secondaryIdx.begin(), secondaryIdx.end());
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

};

#endif //ASSIGNMENT1_SECONDARYIDXDRNAME_H
