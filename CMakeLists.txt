cmake_minimum_required(VERSION 3.26)
project(Assignment1FileManagement C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Assignment1FileManagement
        Appointment.h
        Doctor.h)
