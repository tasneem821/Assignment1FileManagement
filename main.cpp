
#include <iostream>
#include <regex>
#include <sstream>
#include "SecondaryIndexForAppointment.h"
#include "DoctorPrimaryIndex.h"
#include "AppointmentPrimaryIndex.h"
#include "secondaryIdxDrName.h"
#include "Doctor.h"
#include "Appointment.h"

using namespace std;

// Function to handle "Select all from Doctors where Doctor ID='xxx';"
void handleSelectAllFromDoctors(const string& doctorId, DoctorPrimaryIndex& primaryIndex) {
    streampos pos = primaryIndex.searchDoctorInIndex(doctorId);

    if (pos == -1) {
        cout << "Doctor with ID " << doctorId << " not found.\n";
        return;
    }

    ifstream file("doctor_data.txt");
    if (!file.is_open()) {
        cerr << "Error opening doctor data file.\n";
        return;
    }

    file.seekg(pos);
    string doctorData;
    getline(file, doctorData);
    cout << "Doctor Data: " << doctorData << "\n";
    file.close();
}

// Function to handle "Select all from Appointments where Doctor ID='xxx';"
void handleSelectAllFromAppointments(const string& doctorId, AppointmentSecondaryIndex& secondaryIndex, const DoctorPrimaryIndex& primaryIndex) {
    vector<streampos> positions = secondaryIndex.searchAppointmentsByDoctor(doctorId, primaryIndex);

    if (positions.empty()) {
        cout << "No appointments found for Doctor ID: " << doctorId << "\n";
        return;
    }

    ifstream file("appointments_data.txt");
    if (!file.is_open()) {
        cerr << "Error opening appointments data file.\n";
        return;
    }

    for (const auto& pos : positions) {
        file.seekg(pos);
        string appointmentData;
        getline(file, appointmentData);
        cout << "Appointment: " << appointmentData << "\n";
    }

    file.close();
}

// Function to handle "Select Doctor Name from Doctors where Doctor ID='xxx';"
void handleSelectDoctorNameFromDoctors(const string& doctorId, SecondaryIdxDrName& secondaryIndex) {
    string doctorName = secondaryIndex.getDoctorNameByID(doctorId);

    if (doctorName.empty()) {
        cout << "Doctor with ID " << doctorId << " not found.\n";
        return;
    }

    cout << "Doctor Name: " << doctorName << "\n";
}

// Function to parse and handle SQL-like queries
void handleSQLQuery(const string& query, DoctorPrimaryIndex& doctorPrimaryIndex, AppointmentSecondaryIndex& appointmentSecondaryIndex, SecondaryIdxDrName& secondaryIdxDrName, const DoctorPrimaryIndex& primaryIndex) {
    // Define regex patterns for different query types
    regex selectAllDoctorsPattern(R"(Select all from Doctors where Doctor ID='(.+)';)");
    regex selectAllAppointmentsPattern(R"(Select all from Appointments where Doctor ID='(.+)';)");
    regex selectDoctorNamePattern(R"(Select Doctor Name from Doctors where Doctor ID='(.+)';)");

    smatch match;

    if (regex_match(query, match, selectAllDoctorsPattern)) {
        string doctorId = match[1].str();
        handleSelectAllFromDoctors(doctorId, doctorPrimaryIndex);
    } else if (regex_match(query, match, selectAllAppointmentsPattern)) {
        string doctorId = match[1].str();
        handleSelectAllFromAppointments(doctorId, appointmentSecondaryIndex, primaryIndex);
    } else if (regex_match(query, match, selectDoctorNamePattern)) {
        string doctorId = match[1].str();
        handleSelectDoctorNameFromDoctors(doctorId, secondaryIdxDrName);
    } else {
        cout << "Unsupported query format.\n";
    }
}

int main() {
    int choice;
    while (true) {
        cout << "What do you want to do?\n"
                "1- Add New Doctor\n"
                "2- Add New Appointment\n"
                "3- Update Doctor Name\n"
                "4- Update Appointment Date\n"
                "5- Delete Doctor\n"
                "6- Delete Appointment\n"
                "7- Print Doctor Info\n"
                "8- Print Appointment Info\n"
                "9- Write Query\n"
                "10- Exit\n";
        cin >> choice;
        Doctor d1;
        SecondaryIdxDrName dn1;
        DoctorPrimaryIndex dp1("doctor_primary_index.txt");
        Appointment ap1;
        AppointmentPrimaryIndex apindx("AppointmentPrimaryIndex.txt");
        AppointmentSecondaryIndex apsecidx("AppointmentSecondaryIndex");

        if (choice == 1) {
            string drId, drName, drAddress;
            cout << "Please enter the ID:\n";
            cin >> drId;

            if (d1.searchByDoctorID("Doctors.txt", drId)) {
                cout << "Error: Doctor ID already exists!\n";
                continue;
            }
            cout << "Please enter the name:\n";
            cin.ignore();
            getline(cin, drName);

            cout << "Please enter the address:\n";
            getline(cin, drAddress);

            fstream doctorFile("Doctors.txt", ios::app);
            if (!doctorFile.is_open()) {
                cerr << "Error: Could not open doctors.txt\n";
                continue;
            }
            streampos position = doctorFile.tellp();
            d1.setDoctorID(drId.c_str());
            d1.setDoctorName(drName.c_str());
            d1.setDoctorAddress(drAddress.c_str());
            d1.insert("Doctors.txt");
            doctorFile.close();
            dp1.addDoctorToIndex(drId, position);
            dn1.addDoctorSecIdx("secondaryidxName", d1);
            cout << "Doctor added successfully.\n";
        } else if (choice == 2) {
            string apId, apDate, drId;
            cout << "Please enter the appointment ID:\n";
            cin >> apId;
            cin.ignore();
            cout << "Please enter the date:\n";
            getline(cin, apDate);

            cout << "Please enter the doctor ID:\n";
            getline(cin, drId);
            ap1.setAppointmentID(apId.c_str());
            ap1.setAppointmentDate(apDate.c_str());
            ap1.setDoctorID(drId.c_str());
            ap1.insert("Appointment.txt");
            ofstream appointfile("Appointment.txt", ios::app);
            streampos pos = appointfile.tellp();
            apindx.addAppointmentToIndex(apId, pos);
            apsecidx.addAppointmentToIndex(drId, apId);
            appointfile.close();
            cout << "Appointment added successfully.\n";
        } else if (choice == 3) {
            string drId, newdrname;
            cout << "Please enter the doctor ID:\n";
            cin >> drId;
            cin.ignore();
            cout << "Please enter the new doctor name:\n";
            getline(cin, newdrname);
            d1.updateDoctorName("Doctors.txt", drId, newdrname.c_str());
            dn1.updateDoctorName("secondaryidxName", drId, newdrname);
            cout << "Doctor name updated successfully.\n";
        } else if (choice == 4) {
            string apId, newdate;
            cout << "Please enter the appointment ID:\n";
            cin >> apId;
            cin.ignore();
            cout << "Please enter the new date:\n";
            getline(cin, newdate);
            ap1.updateAppointmentDate("Appointment.txt", apId, newdate.c_str());
            cout << "Appointment date updated successfully.\n";
        } else if (choice == 5) {
            string drId;
            cout << "Please enter the doctor ID to delete:\n";
            cin >> drId;
            d1.deleteFromDoctorFile(drId);
            dp1.deleteDoctorFromIndex(drId);
            dn1.deleteSecondaryIndex("secondaryidxName", drId);
            cout << "Doctor deleted successfully.\n";
        } else if (choice == 6) {
            string apId, drId;
            cout << "Please enter the appointment ID to delete:\n";
            cin >> apId;
            cout << "Please enter the doctor ID:\n";
            cin >> drId;
            ap1.deleteRecord("Appointment.txt", apId);
            apindx.deleteAppointmentFromIndex(apId);
            apsecidx.deleteAppointmentFromIndex(drId, apId);
            cout << "Appointment deleted successfully.\n";
        } else if (choice == 7) {
            string drId;
            cout << "Please enter the doctor ID:\n";
            cin >> drId;
            if (!d1.searchByDoctorID("Doctors.txt", drId)) {
                dp1.searchDoctorInIndex(drId);
            }
        } else if (choice == 8) {
            string apId;
            cout << "Please enter the appointment ID:\n";
            cin >> apId;
            ap1.searchByAppointmentID("Appointment.txt", apId);
        } else if (choice == 9) {
            cin.ignore(); // Clear the newline character left in the input buffer
            string query;
            cout << "Enter your SQL-like query:\n";
            getline(cin, query);
            handleSQLQuery(query, dp1, apsecidx, dn1, dp1);
        } else {
            break;
        }
    }

    return 0;
}
