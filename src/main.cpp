#include <iostream>
#include <memory>
#include "models/UndergraduateStudent.h"
#include "models/GraduateStudent.h"
#include "models/PhDStudent.h"
#include "services/StudentManager.h"
#include "persistence/PersistenceManager.h"
using namespace std;

int main() {

    StudentManager mgr;
    const string FILE_PATH = "data/students.txt";

    // ─────────────────────────────────────────
    // CHARGEMENT au démarrage
    // ─────────────────────────────────────────
    cout << "=== Chargement depuis fichier ===\n";
    PersistenceManager::load(mgr, FILE_PATH);
    cout << "Etudiants charges : " << mgr.count() << "\n\n";

    // ─────────────────────────────────────────
    // Si fichier vide → ajouter des étudiants
    // ─────────────────────────────────────────
    if (mgr.count() == 0) {
        cout << "=== Ajout d'etudiants ===\n";
        try {
            mgr.add(make_shared<UndergraduateStudent>("Benali Amira",  1001, 3.8f, "Informatique"));
            mgr.add(make_shared<GraduateStudent>     ("Cherif Omar",   1002, 3.5f, "IA"));
            mgr.add(make_shared<PhDStudent>          ("Meziane Riad",  1003, 3.9f, "Dr. Hadj", 2));
            mgr.add(make_shared<UndergraduateStudent>("Ait Ali Sara",  1004, 2.9f, "Mathematiques"));
            mgr.add(make_shared<GraduateStudent>     ("Hamdi Youssef", 1005, 3.2f, "Reseaux"));
            cout << "5 etudiants ajoutes.\n\n";
        } catch (const exception& e) {
            cerr << "[ERREUR] " << e.what() << "\n";
        }
    }

    // ─────────────────────────────────────────
    // AFFICHAGE de tous les étudiants
    // ─────────────────────────────────────────
    cout << "=== Liste des etudiants ===\n";
    for (auto& pair : mgr.getAll())
        pair.second->display();

    // ─────────────────────────────────────────
    // SAUVEGARDE
    // ─────────────────────────────────────────
    cout << "\n=== Sauvegarde ===\n";
    PersistenceManager::save(mgr, FILE_PATH);

    // ─────────────────────────────────────────
    // TEST : recharger et comparer
    // ─────────────────────────────────────────
    cout << "\n=== Test rechargement ===\n";
    StudentManager mgr2;
    PersistenceManager::load(mgr2, FILE_PATH);

    cout << "Avant : " << mgr.count()  << " etudiants\n";
    cout << "Apres : " << mgr2.count() << " etudiants\n";

    if (mgr.count() == mgr2.count())
        cout << "[OK] Persistance validee ! Les donnees sont identiques.\n";
    else
        cout << "[FAIL] Nombre d'etudiants different apres rechargement !\n";

    // ─────────────────────────────────────────
    // TEST : fichier corrompu
    // ─────────────────────────────────────────
    cout << "\n=== Test fichier corrompu ===\n";
    StudentManager mgr3;
    PersistenceManager::load(mgr3, "data/fichier_inexistant.txt");
    cout << "Etudiants apres fichier absent : " << mgr3.count() << " (doit etre 0)\n";

    return 0;
}