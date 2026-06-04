#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include <string>
#include "../services/StudentManager.h"
using namespace std;

class PersistenceManager {
public:
    // Sauvegarde tous les étudiants dans le fichier
    static void save(const StudentManager& mgr, const string& path);

    // Charge les étudiants depuis le fichier dans mgr
    static void load(StudentManager& mgr, const string& path);

    // Export CSV 
    static void exportCSV(const StudentManager& mgr, const string& path);
};

#endif