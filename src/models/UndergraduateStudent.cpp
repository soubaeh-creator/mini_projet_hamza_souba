#include "UndergraduateStudent.h"
#include <iostream>
using namespace std;

UndergraduateStudent::UndergraduateStudent(const string& name, int id,
                                           float gpa, const string& major)
    : Student(name, id, gpa), major(major) {}

double UndergraduateStudent::computeScholarship() const {
    // Règle : 500 € si GPA >= 3.5, sinon 0 €
    return (gpa >= 3.5f) ? 500.0 : 0.0;
}

void UndergraduateStudent::display() const {
    cout << "[Licence] ID: " << id
         << " | Nom: "    << name
         << " | GPA: "    << gpa
         << " | Filiere: "<< major
         << " | Bourse: " << computeScholarship() << " EUR"
         << endl;
}