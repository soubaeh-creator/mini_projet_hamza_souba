#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include "../exceptions/StudentExceptions.h"
using namespace std;

class Student {
protected:
    string name;  // Nom complet
    int    id;    // Identifiant unique (>= 1000)
    float  gpa;   // Note globale [0.0 .. 4.0]

public:
    // Constructeur : lance InvalidIDException ou InvalidGradeException si invalide
    Student(const string& name, int id, float gpa);

    // ---- Méthodes virtuelles pures ----
    // Chaque classe fille DOIT les implémenter
    virtual void   display()             const = 0;
    virtual string getType()             const = 0;
    virtual double computeScholarship()  const = 0;

    // ---- Accesseurs ----
    string getName() const { return name; }
    int    getId()   const { return id;   }
    float  getGpa()  const { return gpa;  }

    // ---- Mutateurs avec validation ----
    void setName(const string& n) { name = n; }
    void setGpa(float g);  // Lance InvalidGradeException si hors [0,4]

    // Destructeur virtuel (indispensable avec l'héritage)
    virtual ~Student() {}
};

#endif