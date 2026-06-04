# StudentManager - Systeme de Gestion des Etudiants

## Informations
- Annee  : 2025/2026
- Niveau : 1ere annee IMS
- Equipe : HARMANE MOHAMED-AMINE - HAMZA SOUBA
- Video  : https://www.youtube.com/watch?v=mYdZD9PVugQ

## Description
Systeme complet de gestion des etudiants en C++ oriente objet avec :
- OOP (heritage, polymorphisme)
- STL (map, vector, sort, find_if)
- Exceptions personnalisees
- Persistance des donnees (fichiers)
- Interface graphique Qt 6.11

## Structure du projet
StudentManager/
    src/
        models/       -> Student, Undergraduate, Graduate, PhD
        services/     -> StudentManager
        exceptions/   -> Exceptions
        persistence/  -> PersistenceManager
        gui/          -> MainWindow, StudentDialog
    data/             -> students.txt
    tests/            -> test_main.cpp
    StudentManager.pro
    README.md

## Compilation Qt Creator
1. Ouvrir StudentManager.pro
2. Ctrl+B pour compiler
3. Ctrl+R pour lancer

## Compilation Tests
g++ -std=c++17 -I. -Isrc tests/test_main.cpp
src/models/Student.cpp
src/models/UndergraduateStudent.cpp
src/models/GraduateStudent.cpp
src/models/PhDStudent.cpp
src/services/StudentManager.cpp
src/persistence/PersistenceManager.cpp
-o tests/test_output.exe

## Resultats des tests
- 16 tests passes
- 0 tests echoues
