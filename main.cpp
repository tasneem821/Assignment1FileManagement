
#include <bits/stdc++.h>
#include "SecondaryIndexForAppointment.h"
#include "DoctorPrimaryIndex.h"
#include "AppointmentPrimaryIndex.h"
#include "secondaryIdxDrName.h"
#include "Doctor.h"
#include "Appointment.h"

using namespace std;

int main() {
    int choice;
    while (true) {
        cout << "what do you want to do :\n"
                "\nWhat do you want to do?\n"
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
        cin>>choice;
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
            d1.updateDoctorName("Doctors.txt",drId,newdrname.c_str());
            dn1.updateDoctorName("secondaryidxName", drId, newdrname);
//            dn1.print("secondaryidxName.txt");


        }
        else if(choice==4){
            cout<<"please enter appointment  id \n";
            string apId;
            cin>>apId;
            cin.ignore();
            cout<<"please enter the  new date\n";
            string newdate;
            getline(cin,newdate);



            ap1.updateAppointmentDate("Appointment.txt",apId,newdate.c_str());
        }
        else if(choice==5){
            cout<<"please enter the delete id \n";
            string x;
            cin>>x;
            d1.deleteFromDoctorFile(x);
            dp1.deleteDoctorFromIndex(x);
            dn1.deleteSecondaryIndex("secondaryidxName", x);
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
            d1.searchByDoctorID("Doctors.txt",drId);
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
