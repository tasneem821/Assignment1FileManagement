// created By Dina

#ifndef APPOINTMENT_SECONDARY_INDEX_H
#define APPOINTMENT_SECONDARY_INDEX_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

class AppointmentSecondaryIndex
{
private:
    string filename;
    map<string, vector<string>> secondaryIndex; // Maps Doctor ID -> List of Appointment IDs
    vector<streampos> availList;                // Stores positions of deleted records

public:
    AppointmentSecondaryIndex(const string &file) : filename(file) {}

    // Build secondary index and AVAIL LIST
    void buildSecondaryIndex()
    {
        secondaryIndex.clear();
        availList.clear();

        fstream file(filename, ios::in);

        if (!file.is_open())
        {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        streampos pos = file.tellg();

        while (getline(file, line))
        {
            if (line.empty())
            {
                pos = file.tellg();
                continue;
            }

            if (line[0] == '*')
            {
                // Deleted record, add position to AVAIL LIST
                availList.push_back(pos);
            }
            else
            {
                // Extract Doctor ID and Appointment ID, add to secondary index
                istringstream iss(line);
                string appointmentID, appointmentDate, doctorID;
                getline(iss, appointmentID, '|');
                getline(iss, appointmentDate, '|');
                getline(iss, doctorID, '|');

                secondaryIndex[doctorID].push_back(appointmentID);
            }

            pos = file.tellg();
        }

        file.close();
    }

    // Add a new appointment record
    void addAppointment(const string &appointmentID, const string &appointmentDate, const string &doctorID)
    {
        // Check if Appointment ID already exists in any doctor's appointments
        for (const auto &entry : secondaryIndex)
        {
            if (find(entry.second.begin(), entry.second.end(), appointmentID) != entry.second.end())
            {
                cerr << "Error: Appointment ID " << appointmentID << " already exists!" << endl;
                return;
            }
        }

        fstream file(filename, ios::in | ios::out | ios::app);

        if (!file.is_open())
        {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        streampos pos;

        if (!availList.empty())
        {
            // Use position from AVAIL LIST
            pos = availList.back();
            availList.pop_back();
            file.seekp(pos);
        }
        else
        {
            // Append to end of file
            file.seekp(0, ios::end);
            pos = file.tellp();
        }

        // Write the record
        file << appointmentID << "|" << appointmentDate << "|" << doctorID << endl;

        // Update the secondary index
        secondaryIndex[doctorID].push_back(appointmentID);

        file.close();
        cout << "Appointment record added successfully!" << endl;
    }

    // Delete an appointment record
    void deleteAppointment(const string &appointmentID)
    {
        bool found = false;

        fstream file(filename, ios::in | ios::out);

        if (!file.is_open())
        {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        streampos pos = file.tellg();

        while (getline(file, line))
        {
            if (line.substr(0, line.find('|')) == appointmentID)
            {
                found = true;

                // Mark the record as deleted
                file.seekp(pos);
                file.put('*'); // Overwrite the first character with '*'

                // Remove from secondary index
                for (auto &entry : secondaryIndex)
                {
                    auto &appointments = entry.second;
                    appointments.erase(remove(appointments.begin(), appointments.end(), appointmentID), appointments.end());
                }

                // Add position to AVAIL LIST
                availList.push_back(pos);
                break;
            }

            pos = file.tellg();
        }

        file.close();

        if (found)
            cout << "Appointment record deleted successfully!" << endl;
        else
            cerr << "Error: Appointment ID " << appointmentID << " not found!" << endl;
    }

    // Search appointments by Doctor ID
    void searchByDoctorID(const string &doctorID)
    {
        auto it = secondaryIndex.find(doctorID);
        if (it == secondaryIndex.end())
        {
            cout << "No appointments found for Doctor ID " << doctorID << endl;
            return;
        }

        cout << "Appointments for Doctor ID " << doctorID << ":" << endl;
        for (const string &appointmentID : it->second)
        {
            cout << "- " << appointmentID << endl;
        }
    }

    // Display the secondary index and AVAIL LIST
    void displayIndexes() const
    {
        cout << "Secondary Index (Doctor ID -> Appointment IDs):" << endl;
        for (const auto &entry : secondaryIndex)
        {
            cout << entry.first << ": ";
            for (const string &appointmentID : entry.second)
            {
                cout << appointmentID << " ";
            }
            cout << endl;
        }

        cout << "\nAVAIL LIST:" << endl;
        for (const auto &pos : availList)
        {
            cout << pos << endl;
        }
    }
};

#endif
