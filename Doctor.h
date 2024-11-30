//
// Created by tasnim on 11/24/2024.
//

#ifndef ASSIGNMENT_DOCTOR_H
#define ASSIGNMENT_DOCTOR_H

#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <sstream>
#include "vector"
#include "algorithm"
using namespace std;

class Doctor{
public:
char DoctorID[15];
char DoctorName[30];
char Address[30];
vector<streampos>availlist;
public:
    Doctor(){

            memset(DoctorID, 0, sizeof(DoctorID));
            memset(DoctorName, 0, sizeof(DoctorName));
            memset(Address, 0, sizeof(Address));

    }

    const char *getDoctorId() const;

    const char *getDoctorName() const;

    const char *getAddress() const;
    void setDoctorID(const char* id) {
        strncpy(DoctorID, id, sizeof(DoctorID) - 1);
    }
    void setDoctorName(const char* name) {
        strncpy(DoctorName, name, sizeof(DoctorName) - 1);
    }
    void setDoctorAddress(const char* add) {
        strncpy(Address, add, sizeof(Address) - 1);
    }

    void insert(const string &fileName);
    void deleteRecord(const string&fileName , const string& Doctorid);

};
//insert in doctor data file
void Doctor::insert(const std::string &fileName) {
stringstream record;
record<<DoctorID<<"|"<<DoctorName<<"|"<<Address;
string finialrecord = record.str();
int recordlenght = finialrecord.length();
ofstream file;
file.open(fileName,ios::app);
file<<recordlenght<<"|"<<finialrecord<<"\n";
file.close();
}
//delete in doctor data file
void Doctor::deleteRecord(const std::string &fileName, const std::string &Doctorid) {
    fstream file(fileName, ios::in | ios::out);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    bool found = false;
    streampos pos;         
    vector<string> lines;  

    while (getline(file, line)) {
        pos = file.tellg();  
        if (line.substr(3, 4) == Doctorid) {
            found = true;  
            line = '*'+line;  
            availlist.push_back(pos);  
        }
        lines.push_back(line);  
    }

    if (!found) {
        cerr << "Error: Doctor ID " << Doctorid << " not found!" << endl;
        file.close();
        return;
    }

    file.close();
    ofstream outFile(fileName, ios::trunc);  // Open file in truncate mode (clears the file)

    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << fileName << " for writing!" << endl;
        return;
    }

    for (const auto &line : lines) {
        outFile << line << endl;
    }

    outFile.close();
    cout << "Doctor record with ID " << Doctorid << " deleted successfully!" << endl;
}
const char *Doctor::getDoctorId() const {
    return DoctorID;
}

const char *Doctor::getDoctorName() const {
    return DoctorName;
}

const char *Doctor::getAddress() const {
    return Address;
}


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
