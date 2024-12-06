
#include <bits/stdc++.h>
#include "SecondaryIndexForAppointment.h"
#include "DoctorPrimaryIndex.h"
#include "AppointmentPrimaryIndex.h"
#include "secondaryIdxDrName.h"
#include "Doctor.h"
#include "Appointment.h"
#include "DoctorPrimaryIndex.h"

using namespace std;

int main() {
//    fstream ssecondaryIndexFile;
//    ssecondaryIndexFile.open("appointment_secondary_index.txt");
//    // Create a secondary index instance
//    string secondaryIndexFile = "appointment_secondary_index.txt";
//    AppointmentSecondaryIndex secondaryIndex(secondaryIndexFile);
//
//    // Rebuild the index from the file (if it exists)
//    secondaryIndex.buildSecondaryIndex();
//
//    // Test adding appointments
//    secondaryIndex.addAppointmentToIndex("D001", "A001");
//    secondaryIndex.addAppointmentToIndex("D001", "A002");
//    secondaryIndex.addAppointmentToIndex("D002", "A003");
//    secondaryIndex.addAppointmentToIndex("D003", "A004");
//
//        // Display the index
//        cout << "\nIndex after adding appointments:" << endl;
//        secondaryIndex.displayIndexes();
//
//        // Test deleting an appointment
//        secondaryIndex.deleteAppointmentFromIndex("D001", "A001");
//        cout << "\nIndex after deleting A001 for Doctor D001:" << endl;
//        secondaryIndex.displayIndexes();
//
//        // Test updating a doctor's ID for an appointment
//        secondaryIndex.updateDoctorIDForAppointment("A002", "D001", "D004");
//        cout << "\nIndex after updating doctor ID for A002 from D001 to D004:" << endl;
//        secondaryIndex.displayIndexes();
//
//        // Test searching for appointments for a doctor
//        cout << "\nAppointments for Doctor D004:" << endl;
//        vector<string> appointments = secondaryIndex.binarySearchAppointments("D004");
//        for (const string &appointmentID: appointments) {
//            cout << appointmentID << " ";
//        }
//        cout << endl;
//
//        // Simulate primary index for testing cross-search
//        DoctorPrimaryIndex primaryIndex("doctor_primary_index.txt");
//        primaryIndex.addDoctorToIndex("D001", 0);
//        primaryIndex.addDoctorToIndex("D002", 100);
//        primaryIndex.addDoctorToIndex("D003", 200);
//        primaryIndex.addDoctorToIndex("D004", 300);
//
//        cout << "\nSearching primary positions for appointments of Doctor D004:" << endl;
//        vector<streampos> positions = secondaryIndex.searchAppointmentsByDoctor("D004", primaryIndex);
//        for (streampos pos: positions) {
//            cout << pos << " ";
//        }
//        cout << endl;
    int choice;
    while (true) {
        cout << "what do you want to do :\n"
                "\nWhat do you want to do?\n"
                "1- Add New Doctor\n"
                "2- Add New Appointment\n"
                "3- Update Doctor Name\n"
                "4- Update Appointment Date\n"
                "5- Delete Appointment\n"
                "6- Delete Doctor\n"
                "7- Print Doctor Info\n"
                "8- Print Appointment Info\n"
                "9- Write Query\n"
                "10- Exit\n";
        cin>>choice;
        Doctor d1;
        SecondaryIdxDrName dn1;
        Appointment ap1;
            AppointmentPrimaryIndex apindx("AppointmentPrimaryIndex.txt");
                DoctorPrimaryIndex dp1("primaryIndex.txt");
            AppointmentSecondaryIndex apsecidx("AppointmentSecondaryIndex");

        if(choice==1){
                string drId;
                string drName;
                string drAdress;
                cout<<"please enter the id \n";
                cin>>drId;
                cin.ignore();
                cout<<"please enter the name \n";
                getline(cin,drName);
            cin.ignore();

            cout<<"please enter the address \n";
            getline(cin,drAdress);
                d1.setDoctorID(drId.c_str());
                d1.setDoctorName(drName.c_str());
                d1.setDoctorAddress(drAdress.c_str());
                d1.insert("doctors.txt");
            ofstream doctorFile("doctors.txt", ios::app);

            streampos positon = doctorFile.tellp();
            dp1.addDoctorToIndex(drId,positon);
            dn1.addDoctorSecIdx("secondaryidxName.txt",d1);

//cout<<d1.getDoctorId()<<" "<<d1.getAddress()<<" "<<d1.getDoctorName();

        }
        else if(choice==2){
            string apId;
            string apDate;
            string drId;
            cout<<"please enter the id \n";
            cin>>apId;
            cin.ignore();
            cout<<"please enter the date \n";
            getline(cin,apDate);
            cin.ignore();

            cout<<"please enter the drId \n";
            getline(cin,drId);
            ap1.setAppointmentID(apId.c_str());
            ap1.setAppointmentDate(apDate.c_str());
            ap1.setDoctorID(drId.c_str());
            ap1.insert("Appointment.txt");
            ofstream appointfile("Appointment.txt",ios::app);
            streampos pos = appointfile.tellp();
            apindx.addAppointmentToIndex(apId,pos);
            apsecidx.addAppointmentToIndex(drId,apId);


        }
        else if(choice==3){

                cout<<"please enter dr  id \n";
                string drId;
              cin>>drId;
              cin.ignore();
                cout<<"please enter the dr name new\n";
                string newdrname;
                getline(cin,newdrname);



            d1.updateDoctorName("doctors.txt",drId,newdrname.c_str());
            dn1.updateDoctorName(drId,newdrname);
            dn1.print("secondaryidxName.txt");


        }
        else if(choice==4){
            cout<<"please enter appointment  id \n";
            string apId;
            cin>>apId;
            cin.ignore();
            cout<<"please enter the  new date\n";
            string newdate;
            getline(cin,newdate);



            d1.updateDoctorName("Appointment.txt",apId,newdate.c_str());
        }
        else if(choice==5){
            cout<<"please enter the delete id \n";
            string x;
            cin>>x;
            d1.deleteRecord("doctors.txt",x);
            dp1.deleteDoctorFromIndex(x);
            dn1.deleteDoctor(x);
        }
        else if(choice==6){
            cout<<"please enter appointment id\n";
            string apId;
            cin>>apId;

            cout<<"please enter dr id\n";
            string drId;
            cin>>drId;
            ap1.deleteRecord("Appointment.txt",apId);
            apindx.deleteAppointmentFromIndex(apId);
            apsecidx.deleteAppointmentFromIndex(drId,apId);
        }
        else if(choice==7){
            string drId;
            cout<<"please enter dr id\n";
            cin>>drId;
            d1.searchByDoctorID("doctors.txt",drId);
            dp1.searchDoctorInIndex(drId);
        }
        else if(choice==8){
            cout<<"please enter appointment id\n";
            string apId;
            cin>>apId;
ap1.searchByAppointmentID("Appointment.txt",apId);

        }
        else if(choice==9){}
        else{
            break;
        }
    }


    return 0;
}
