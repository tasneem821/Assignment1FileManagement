cmake_minimum_required(VERSION 3.21.1)
project(Assignment1FileManagement CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(
        Appointment.h
        Doctor.h
        DoctorPrimaryIndex.h
        main.cpp
        SecondaryIndexForAppointment.h
        AppointmentPrimaryIndex.h
)
