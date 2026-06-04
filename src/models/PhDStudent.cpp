#include "PhDStudent.h"
#include <iostream>
using namespace std;

PhDStudent::PhDStudent(const string& name, int id, float gpa,
                       const string& supervisor, int yearOfStudy)
    : Student(name, id, gpa), supervisor(supervisor), yearOfStudy(yearOfStudy) {}

double PhDStudent::computeScholarship() const {
    // Règle : bourse fixe de 1200 €/mois
    return 1200.0;
}

void PhDStudent::display() const {
    cout << "[Doctorat] ID: "    << id
         << " | Nom: "          << name
         << " | GPA: "          << gpa
         << " | Directeur: "    << supervisor
         << " | Annee: "        << yearOfStudy
         << " | Bourse: "       << computeScholarship() << " EUR"
         << endl;
}