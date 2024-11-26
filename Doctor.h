//
// Created by tasnim on 11/24/2024.
//

#ifndef ASSIGNMENT_DOCTOR_H
#define ASSIGNMENT_DOCTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Doctor{
public:
char DoctorID[15];
char DoctorName[30];
char Address[30];


};

class SecondaryIdxDoctorName {
    vector<pair<string , string>> secondaryIdx;
public:

    // insert doctor name in the index file
    void addDoctor(Doctor doctor) {
        secondaryIdx.emplace_back(doctor.DoctorName, doctor.DoctorID);

        sort(secondaryIdx.begin(), secondaryIdx.end());
    }

    // search for doctor id with the doctor name
    vector<string> searchByDoctorName(Doctor doctor) {
        vector<string> result;
        for (const auto& pair : secondaryIdx)
            if (pair.first == doctor.DoctorName)
                result.push_back(pair.second);
        return result;
    }

    // delete a doctor from the index file
//    void deleteEntry(Doctor doctor) {
//        auto it = remove_if(secondaryIdx.begin(), secondaryIdx.end(),
//                            [&](const pair<string, string>& entry) {
//                                return entry.first == doctorName && entry.second == doctorID;
//                            });
//        vector<string> result;
//        for (const auto& pair : secondaryIdx) {
//            if (pair.first == doctor.DoctorName)
//                continue;
//            else
//                result.push_back(doctor.DoctorName);
//        }
//        for (const auto& pair : secondaryIdx) {
//            if (pair != secondaryIdx.end()) {
//                secondaryIdx.erase(it, secondaryIdx.end());
//            }
//        }
//
//    }
};

#endif //ASSIGNMENT_DOCTOR_H
