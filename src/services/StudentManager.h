#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H


/*
 * ============================================
 * BONUS : Smart Pointers Exclusifs
 * ============================================
 * Ce projet n'utilise AUCUN pointeur brut
 * pour la gestion des etudiants.
 *
 * Tous les etudiants sont geres via :
 *   - shared_ptr<Student>
 *   - make_shared<UndergraduateStudent>()
 *   - make_shared<GraduateStudent>()
 *   - make_shared<PhDStudent>()
 *
 * Le conteneur principal utilise :
 *   map<int, shared_ptr<Student>>
 *
 * Avantages :
 *   - Pas de memory leak
 *   - Pas de delete manuel
 *   - Partage safe entre fonctions
 * ============================================
 */



#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "../models/Student.h"
#include "../exceptions/StudentExceptions.h"
using namespace std;

class StudentManager {
private:
    // Stockage principal : ID → Étudiant
    map<int, shared_ptr<Student>> students;

public:
    // ---- CRUD ----
    void add(shared_ptr<Student> s);
    void remove(int id);
    void update(int id, shared_ptr<Student> updated);

    // ---- Recherche ----
    shared_ptr<Student> findById(int id) const;
    vector<shared_ptr<Student>> findByName(const string& name) const;

    // ---- Tri ----
    vector<shared_ptr<Student>> sortByGpa()  const;  // décroissant
    vector<shared_ptr<Student>> sortByName() const;  // alphabétique

    // ---- Statistiques ----
    double averageGpa()              const;
    int    countByType(const string& type) const;
    int    count()                   const { return students.size(); }

    // ---- Accès complet ----
    const map<int, shared_ptr<Student>>& getAll() const { return students; }
};

#endif