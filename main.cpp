
#include <bits/stdc++.h>
#include "SecondaryIndexForAppointment.h"
#include "DoctorPrimaryIndex.h"
using namespace std;

int main() {
    fstream ssecondaryIndexFile;
    ssecondaryIndexFile.open("appointment_secondary_index.txt");
    // Create a secondary index instance
    string secondaryIndexFile = "appointment_secondary_index.txt";
    AppointmentSecondaryIndex secondaryIndex(secondaryIndexFile);

    // Rebuild the index from the file (if it exists)
    secondaryIndex.buildSecondaryIndex();

    // Test adding appointments
    secondaryIndex.addAppointmentToIndex("D001", "A001");
    secondaryIndex.addAppointmentToIndex("D001", "A002");
    secondaryIndex.addAppointmentToIndex("D002", "A003");
    secondaryIndex.addAppointmentToIndex("D003", "A004");

    // Display the index
    cout << "\nIndex after adding appointments:" << endl;
    secondaryIndex.displayIndexes();

    // Test deleting an appointment
    secondaryIndex.deleteAppointmentFromIndex("D001", "A001");
    cout << "\nIndex after deleting A001 for Doctor D001:" << endl;
    secondaryIndex.displayIndexes();

    // Test updating a doctor's ID for an appointment
    secondaryIndex.updateDoctorIDForAppointment("A002", "D001", "D004");
    cout << "\nIndex after updating doctor ID for A002 from D001 to D004:" << endl;
    secondaryIndex.displayIndexes();

    // Test searching for appointments for a doctor
    cout << "\nAppointments for Doctor D004:" << endl;
    vector<string> appointments = secondaryIndex.binarySearchAppointments("D004");
    for (const string &appointmentID : appointments) {
        cout << appointmentID << " ";
    }
    cout << endl;

    // Simulate primary index for testing cross-search
    DoctorPrimaryIndex primaryIndex("doctor_primary_index.txt");
    primaryIndex.addDoctorToIndex("D001", 0);
    primaryIndex.addDoctorToIndex("D002", 100);
    primaryIndex.addDoctorToIndex("D003", 200);
    primaryIndex.addDoctorToIndex("D004", 300);

    cout << "\nSearching primary positions for appointments of Doctor D004:" << endl;
    vector<streampos> positions = secondaryIndex.searchAppointmentsByDoctor("D004", primaryIndex);
    for (streampos pos : positions) {
        cout << pos << " ";
    }
    cout << endl;

    return 0;
}
