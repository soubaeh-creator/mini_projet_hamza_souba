#ifndef STUDENT_EXCEPTIONS_H
#define STUDENT_EXCEPTIONS_H

#include <exception>
#include <string>
using namespace std;

// ID invalide (< 1000)
class InvalidIDException : public exception {
    string msg;
public:
    InvalidIDException(int id)
        : msg("ID invalide (doit etre >= 1000) : " + to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// GPA hors plage [0.0 - 4.0]
class InvalidGradeException : public exception {
    string msg;
public:
    InvalidGradeException(float gpa)
        : msg("GPA hors plage [0.0 - 4.0] : " + to_string(gpa)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// Etudiant non trouvé
class StudentNotFoundException : public exception {
    string msg;
public:
    StudentNotFoundException(int id)
        : msg("Etudiant introuvable avec ID : " + to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// ID déjà utilisé
class DuplicateIDException : public exception {
    string msg;
public:
    DuplicateIDException(int id)
        : msg("ID deja utilise : " + to_string(id)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

#endif