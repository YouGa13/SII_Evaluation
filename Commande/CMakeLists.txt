project(Projet_LangageC)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)

add_executable(test commande.c main_co.c commande.h)

