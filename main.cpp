
#include <bits/stdc++.h>
#include "DoctorPrimaryIndex.h"
using namespace std;


int main() {
    DoctorPrimaryIndex primaryIndex("Doctors.txt");

    // Build primary index
    primaryIndex.buildPrimaryIndex();
    primaryIndex.displayIndexes();

//    // Add doctors
//    primaryIndex.addDoctor("DOC001", "John Smith", "123 Elm St.");
//    primaryIndex.addDoctor("DOC002", "Jane Doe", "456 Oak Rd.");
//    primaryIndex.addDoctor("DOC003", "Alice Jones", "789 Pine St.");
//
//    // Display indexes
//    primaryIndex.displayIndexes();
//
//    // Search for a doctor
//    cout << "\nSearching for Doctor ID 'DOC002':" << endl;
//    primaryIndex.searchByDoctorID("DOC002");
//
//    // Update a doctor
//    primaryIndex.updateDoctor("DOC002", "Jane Roe", "890 Maple Ave.");
//
//    // Search again to see the update
//    cout << "\nAfter Update - Searching for Doctor ID 'DOC002':" << endl;
//    primaryIndex.searchByDoctorID("DOC002");
//
//    // Delete a doctor
//    primaryIndex.deleteDoctor("DOC003");
//
//    // Try to search for the deleted doctor
//    cout << "\nAfter Delete - Searching for Doctor ID 'DOC003':" << endl;
//    primaryIndex.searchByDoctorID("DOC003");
//

    return 0;
}
