#ifndef GRADUATE_H
#define GRADUATE_H

#include "Student.h"

class GraduateStudent : public Student {
private:
    string researchTopic;  // Sujet de recherche

public:
    GraduateStudent(const string& name, int id, float gpa,
                    const string& researchTopic);

    void   display()            const override;
    string getType()            const override { return "Master"; }
    double computeScholarship() const override;

    string getResearchTopic() const { return researchTopic; }
    void   setResearchTopic(const string& t) { researchTopic = t; }
};

#endif