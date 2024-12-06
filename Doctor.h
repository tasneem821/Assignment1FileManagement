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
    char DoctorID[15]{};
    char DoctorName[30]{};
    char Address[30]{};
    vector<streampos>availList;
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
    void deleteRecord(const string&fileName , const string& DoctorId);
    void updateDoctorName(const string&fileName, const string&doctorId, const char *doctorName);
    void updateDoctorAddress(const string&fileName,const string&doctorId,const string&doctorAddress);
    bool searchByDoctorID(const string& fileName, const string& doctorId) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << fileName << endl;
            return false;
        }

        // Store all doctor records in a vector
        vector<pair<string, pair<string, string>>> records; // ID, (Name, Address)
        string line;
        while (getline(file, line)) {
            if (line[0] == '*') {
                continue;
            }
            stringstream ss(line);
            string length, doctorid, name, address;
            getline(ss, length, '|');
            getline(ss, doctorid, '|');
            getline(ss, name, '|');
            getline(ss, address);
            records.push_back(make_pair(doctorid, make_pair(name, address)));
        }

        file.close();

        // Sort records by DoctorID (important for binary search)
        sort(records.begin(), records.end());

        // Binary search for the doctor by ID
        auto it = lower_bound(records.begin(), records.end(), make_pair(doctorId, make_pair("", "")),
                              [](const pair<string, pair<string, string>>& a, const pair<string, pair<string, string>>& b) {
                                  return a.first < b.first;
                              });

        if (it != records.end() && it->first == doctorId) {
            // Print doctor information
            cout << "Doctor Found!" << endl;
            cout << "Doctor ID: " << it->first << endl;
            cout << "Doctor Name: " << it->second.first << endl;
            cout << "Doctor Address: " << it->second.second << endl;
            return true;
        }

        cout << "Doctor ID " << doctorId << " not found!" << endl;
        return false;
    }

    void deleteRecord(const string &DoctorId);
    void deleteFromDoctorFile(const string &DoctorId);
};
//insert in doctor data file
void Doctor::insert(const std::string &fileName) {
    stringstream record;
    record << DoctorID << "|" << DoctorName << "|" << Address;
    string finalRecord = record.str();
    int recordLength = finalRecord.length();

    fstream file(fileName, ios::in | ios::out);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    bool inserted = false;

    while (getline(file, line)) {
        cout << "Reading line: " << line << endl;

        if (line[0] == '*') {  // Found a deleted record
            streampos pos = file.tellg();  // Get the current position after reading the line
            pos -= (line.length() + 1);  // Adjust to the start of the line
            file.seekp(pos);

            string replacement = to_string(recordLength) + "|" + finalRecord;
            file << replacement;

            // Pad remaining space if the replacement record is shorter
            int remainingSpace = line.length() - replacement.length();
            if (remainingSpace > 0) {
                file << string(remainingSpace, ' ');
            }

            // Remove the position from availList
            if (!availList.empty()) {
                availList.erase(availList.begin());
            }

            inserted = true;
            break;
        }
    }

    file.close();

    // Append the record at the end if no deleted space was found
    if (!inserted) {
        ofstream outFile(fileName, ios::app);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file " << fileName << " for appending!" << endl;
            return;
        }
        outFile << to_string(recordLength) + "|" + finalRecord << endl;
        outFile.close();
    }

    cout << "Doctor record inserted successfully!" << endl;
}



void Doctor::deleteFromDoctorFile(const std::string &DoctorId) {
    fstream doctorFile("Doctors.txt", ios::in | ios::out);
    if (!doctorFile.is_open()) {
        cerr << "Error: Could not open doctor file Doctors.txt" << endl;
        return;
    }

    string line;
    bool found = false;
    streampos pos;
    vector<string> lines;
    vector<streampos> deletedPositions;

    while (getline(doctorFile, line)) {
        pos = doctorFile.tellg();
        pos -= line.length() + 1;
        size_t firstPipe = line.find('|');
        size_t secondPipe = line.find('|', firstPipe + 1);
        string id = line.substr(firstPipe + 1, secondPipe - firstPipe - 1);

        if (id == DoctorId) {
            found = true;
            line = '*' + line;
            deletedPositions.push_back(pos);
        }

        lines.push_back(line);
    }

    if (!found) {
        cerr << "Error: Doctor ID " << DoctorId << " not found!" << endl;
        doctorFile.close();
        return;
    }

    doctorFile.close();

    ofstream outDoctorFile("Doctors.txt", ios::trunc);
    if (!outDoctorFile.is_open()) {
        cerr << "Error: Could not open doctor file Doctors.txt for writing!" << endl;
        return;
    }

    for (const auto &line : lines) {
        outDoctorFile << line << endl;
    }

    outDoctorFile.close();

    for (const auto &pos : deletedPositions) {
        availList.push_back(pos);
    }

    cout << "Doctor record with ID " << DoctorId << " deleted successfully!" << endl;
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

void Doctor::updateDoctorName(const string&fileName, const std::string &doctorId, const char *doctorName) {
    fstream file(fileName,ios::in|ios::out);
    if(!file.is_open()){
        cerr<<"Error canot open file "+fileName<<"\n";

    }
    string line;
    bool found = false;
    vector<string>lines;
    while(getline(file,line)){
  stringstream ss(line);
  string lenght,doctorid,name,address;
        getline(ss,lenght,'|');
        getline(ss,doctorid,'|');
        getline(ss,name,'|');
        getline(ss,address);
        if(doctorid==doctorId){
            found = true;
            stringstream  record;
            string updatedrecord = doctorid +"|"+doctorName+"|"+address;
            int updatedlenght = updatedrecord.length();
            stringstream updatedfile;
            updatedfile<<updatedlenght<<"|"<<updatedrecord;
        line =  updatedfile.str();

        }

        lines.push_back(line);
    }
file.close();
    ofstream outFile(fileName,ios::trunc);
    for(const auto &line:lines){
        outFile<<line<<"\n";
    }

    outFile.close();
    cout << "Doctor record with ID " << doctorId << " updated successfully!" << endl;


}

void Doctor::updateDoctorAddress(const std::string &fileName, const std::string &doctorId,
                                 const std::string &doctorAddress)  {
    fstream file(fileName,ios::in|ios::out);
    if(!file.is_open()){
        cerr<<"Error canot open file "+fileName<<"\n";

    }
    string line;
    bool found = false;
    vector<string>lines;
    while(getline(file,line)){
        stringstream ss(line);
        string lenght,doctorid,name,address;
        getline(ss,lenght,'|');
        getline(ss,doctorid,'|');
        getline(ss,name,'|');
        getline(ss,address);
        if(doctorid==doctorId){
            found = true;
            stringstream  record;
            string updatedrecord = doctorid +"|"+name+"|"+doctorAddress;
            int updatedlenght = updatedrecord.length();
            stringstream updatedfile;
            updatedfile<<updatedlenght<<"|"<<updatedrecord;
            line =  updatedfile.str();

        }

        lines.push_back(line);
    }
    file.close();
    ofstream outFile(fileName,ios::trunc);
    for(const auto &line:lines){
        outFile<<line<<"\n";
    }

    outFile.close();
    cout << "Doctor record with ID " << doctorId << " updated successfully!" << endl;


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
};

#endif //ASSIGNMENT_DOCTOR_H
