#ifndef UNDERGRADUATE_H
#define UNDERGRADUATE_H

#include "Student.h"

class UndergraduateStudent : public Student {
private:
    string major;  // Filière (ex : "Informatique")

public:
    UndergraduateStudent(const string& name, int id, float gpa, const string& major);

    void   display()            const override;
    string getType()            const override { return "Licence"; }
    double computeScholarship() const override;

    string getMajor() const { return major; }
    void   setMajor(const string& m) { major = m; }
};

#endif