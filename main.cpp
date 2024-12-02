
#include <bits/stdc++.h>
#include "DoctorPrimaryIndex.h"
using namespace std;


#include "DoctorPrimaryIndex.h"

int main() {
    // Initialize the index with a filename for storing the index data
    DoctorPrimaryIndex doctorIndex("doctor_primary_index.txt");

    // Build the primary index from the file (if it exists)
    doctorIndex.buildPrimaryIndex();

    // Display the current state of the index and the AVAIL LIST
    doctorIndex.displayIndexes();

    // Add some doctors to the index
    doctorIndex.addDoctorToIndex("D001", 100);
    doctorIndex.addDoctorToIndex("D002", 200);
    doctorIndex.addDoctorToIndex("D003", 300);

    // Display the updated index
    cout << "\nAfter Adding Doctors:" << endl;
    doctorIndex.displayIndexes();

    // Delete a doctor from the index
    doctorIndex.deleteDoctorFromIndex("D002");

    // Display the index after deletion
    cout << "\nAfter Deleting Doctor D002:" << endl;
    doctorIndex.displayIndexes();

    // Try to add a doctor with an existing ID
    doctorIndex.addDoctorToIndex("D001", 400);

    // Try to delete a non-existent doctor
    doctorIndex.deleteDoctorFromIndex("D999");

    return 0;
}
