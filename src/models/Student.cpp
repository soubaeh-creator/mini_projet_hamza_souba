#include "Student.h"

Student::Student(const string& name, int id, float gpa) {
    // Validation de l'ID
    if (id < 1000)
        throw InvalidIDException(id);

    // Validation du GPA
    if (gpa < 0.0f || gpa > 4.0f)
        throw InvalidGradeException(gpa);

    this->name = name;
    this->id   = id;
    this->gpa  = gpa;
}

void Student::setGpa(float g) {
    if (g < 0.0f || g > 4.0f)
        throw InvalidGradeException(g);
    gpa = g;
}