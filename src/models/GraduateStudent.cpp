#include "GraduateStudent.h"
#include <iostream>
using namespace std;

GraduateStudent::GraduateStudent(const string& name, int id, float gpa,
                                 const string& researchTopic)
    : Student(name, id, gpa), researchTopic(researchTopic) {}

double GraduateStudent::computeScholarship() const {
    // Règle : GPA × 300 €
    return static_cast<double>(gpa) * 300.0;
}

void GraduateStudent::display() const {
    cout << "[Master] ID: " << id
         << " | Nom: "    << name
         << " | GPA: "    << gpa
         << " | Sujet: "  << researchTopic
         << " | Bourse: " << computeScholarship() << " EUR"
         << endl;
}