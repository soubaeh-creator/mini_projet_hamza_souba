#include "StudentManager.h"
#include <iostream>
#include <numeric>
#include <cctype>
using namespace std;

// ─────────────────────────────────────────
// CRUD
// ─────────────────────────────────────────

void StudentManager::add(shared_ptr<Student> s) {
    // Vérifie si l'ID existe déjà
    if (students.count(s->getId()))
        throw DuplicateIDException(s->getId());
    students[s->getId()] = s;
}

void StudentManager::remove(int id) {
    // Vérifie si l'étudiant existe
    if (!students.count(id))
        throw StudentNotFoundException(id);
    students.erase(id);
}

void StudentManager::update(int id, shared_ptr<Student> updated) {
    if (!students.count(id))
        throw StudentNotFoundException(id);
    students[id] = updated;
}

// ─────────────────────────────────────────
// RECHERCHE
// ─────────────────────────────────────────

shared_ptr<Student> StudentManager::findById(int id) const {
    if (!students.count(id))
        throw StudentNotFoundException(id);
    return students.at(id);
}

vector<shared_ptr<Student>> StudentManager::findByName(const string& name) const {
    vector<shared_ptr<Student>> result;

    // Convertir la recherche en minuscules
    string nameLower = name;
    transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    // find_if sur la map → cherche toutes les correspondances partielles
    for (auto it = students.begin(); it != students.end(); ) {
        it = find_if(it, students.end(), [&](const pair<int, shared_ptr<Student>>& p) {
            string n = p.second->getName();
            transform(n.begin(), n.end(), n.begin(), ::tolower);
            return n.find(nameLower) != string::npos;
        });

        if (it != students.end()) {
            result.push_back(it->second);
            ++it;
        }
    }

    return result;
}

// ─────────────────────────────────────────
// TRI
// ─────────────────────────────────────────

vector<shared_ptr<Student>> StudentManager::sortByGpa() const {
    // Copier tous les étudiants dans un vector
    vector<shared_ptr<Student>> v;
    for (auto& pair : students)
        v.push_back(pair.second);

    // Tri décroissant par GPA avec lambda
    sort(v.begin(), v.end(), [](const shared_ptr<Student>& a,
                                 const shared_ptr<Student>& b) {
        return a->getGpa() > b->getGpa();  // > = décroissant
    });

    return v;
}

vector<shared_ptr<Student>> StudentManager::sortByName() const {
    vector<shared_ptr<Student>> v;
    for (auto& pair : students)
        v.push_back(pair.second);

    // Tri alphabétique insensible à la casse avec lambda
    sort(v.begin(), v.end(), [](const shared_ptr<Student>& a,
                                 const shared_ptr<Student>& b) {
        string na = a->getName(), nb = b->getName();
        transform(na.begin(), na.end(), na.begin(), ::tolower);
        transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
        return na < nb;
    });

    return v;
}

// ─────────────────────────────────────────
// STATISTIQUES
// ─────────────────────────────────────────

double StudentManager::averageGpa() const {
    if (students.empty()) return 0.0;

    double total = 0.0;
    for (auto& pair : students)
        total += pair.second->getGpa();

    return total / students.size();
}

int StudentManager::countByType(const string& type) const {
    // count_if : compte les étudiants dont getType() correspond
    return count_if(students.begin(), students.end(),
        [&](const pair<int, shared_ptr<Student>>& p) {
            return p.second->getType() == type;
        });
}