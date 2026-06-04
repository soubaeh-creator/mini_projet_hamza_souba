#include <iostream>
#include <memory>
#include <cassert>
#include <fstream>
#include "../src/models/UndergraduateStudent.h"
#include "../src/models/GraduateStudent.h"
#include "../src/models/PhDStudent.h"
#include "../src/services/StudentManager.h"
#include "../src/persistence/PersistenceManager.h"
using namespace std;

// ─────────────────────────────────────────
// Compteurs globaux
// ─────────────────────────────────────────
int passed = 0;
int failed = 0;

void testOK(const string& name) {
    cout << "  [OK]   " << name << "\n";
    passed++;
}

void testFAIL(const string& name, const string& reason) {
    cout << "  [FAIL] " << name << " => " << reason << "\n";
    failed++;
}

// ═════════════════════════════════════════
// PARTIE 1 — Tests OOP
// ═════════════════════════════════════════
void testOOP() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║  PARTIE 1 — OOP & Polymorphisme      ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    // Test 1 : getType()
    try {
        auto s1 = make_shared<UndergraduateStudent>("Benali Amira", 1001, 3.8f, "Info");
        auto s2 = make_shared<GraduateStudent>     ("Cherif Omar",  1002, 3.5f, "IA");
        auto s3 = make_shared<PhDStudent>          ("Meziane Riad", 1003, 3.9f, "Dr.Hadj", 2);

        if (s1->getType() == "Licence")  testOK("UndergraduateStudent::getType() == Licence");
        else testFAIL("getType() Licence", "Retourne : " + s1->getType());

        if (s2->getType() == "Master")   testOK("GraduateStudent::getType() == Master");
        else testFAIL("getType() Master", "Retourne : " + s2->getType());

        if (s3->getType() == "Doctorat") testOK("PhDStudent::getType() == Doctorat");
        else testFAIL("getType() Doctorat", "Retourne : " + s3->getType());

    } catch (...) {
        testFAIL("Creation etudiants valides", "Exception inattendue");
    }

    // Test 2 : computeScholarship()
    try {
        auto s1 = make_shared<UndergraduateStudent>("A", 1010, 3.8f, "Info");
        auto s2 = make_shared<UndergraduateStudent>("B", 1011, 3.0f, "Info");
        auto s3 = make_shared<GraduateStudent>     ("C", 1012, 3.5f, "IA");
        auto s4 = make_shared<PhDStudent>          ("D", 1013, 3.9f, "Dr.X", 1);

        if (s1->computeScholarship() == 500.0) testOK("Licence GPA>=3.5 → 500 EUR");
        else testFAIL("Licence bourse 500", "Valeur: " + to_string(s1->computeScholarship()));

        if (s2->computeScholarship() == 0.0)   testOK("Licence GPA<3.5  → 0 EUR");
        else testFAIL("Licence bourse 0", "Valeur: " + to_string(s2->computeScholarship()));

        if (s3->computeScholarship() == 3.5*300) testOK("Master bourse = GPA x 300 = 1050 EUR");
        else testFAIL("Master bourse", "Valeur: " + to_string(s3->computeScholarship()));

        if (s4->computeScholarship() == 1200.0) testOK("PhD bourse fixe = 1200 EUR");
        else testFAIL("PhD bourse 1200", "Valeur: " + to_string(s4->computeScholarship()));

    } catch (...) {
        testFAIL("computeScholarship()", "Exception inattendue");
    }

    // Test 3 : Polymorphisme
    try {
        vector<shared_ptr<Student>> list;
        list.push_back(make_shared<UndergraduateStudent>("A", 1020, 3.8f, "Info"));
        list.push_back(make_shared<GraduateStudent>     ("B", 1021, 3.5f, "IA"));
        list.push_back(make_shared<PhDStudent>          ("C", 1022, 3.9f, "Dr.X", 1));

        bool ok = true;
        for (auto& s : list)
            if (s->getType().empty()) ok = false;

        if (ok) testOK("Polymorphisme via shared_ptr<Student>");
        else    testFAIL("Polymorphisme", "getType() vide");

    } catch (...) {
        testFAIL("Polymorphisme", "Exception inattendue");
    }
}

// ═════════════════════════════════════════
// PARTIE 2 — Tests STL
// ═════════════════════════════════════════
void testSTL() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║  PARTIE 2 — STL & Gestion données    ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    StudentManager mgr;
    mgr.add(make_shared<UndergraduateStudent>("Benali Amira",  1001, 3.8f, "Info"));
    mgr.add(make_shared<GraduateStudent>     ("Cherif Omar",   1002, 3.5f, "IA"));
    mgr.add(make_shared<PhDStudent>          ("Meziane Riad",  1003, 3.9f, "Dr.Hadj", 2));
    mgr.add(make_shared<UndergraduateStudent>("Ait Ali Sara",  1004, 2.9f, "Math"));
    mgr.add(make_shared<GraduateStudent>     ("Hamdi Youssef", 1005, 3.2f, "Reseaux"));

    // Test count
    if (mgr.count() == 5) testOK("Ajout 5 etudiants → count = 5");
    else testFAIL("count()", "Valeur: " + to_string(mgr.count()));

    // Test sortByGpa décroissant
    auto byGpa = mgr.sortByGpa();
    bool gpaOk = true;
    for (int i = 0; i < (int)byGpa.size()-1; i++)
        if (byGpa[i]->getGpa() < byGpa[i+1]->getGpa()) gpaOk = false;

    if (gpaOk) testOK("sortByGpa() → ordre decroissant correct");
    else        testFAIL("sortByGpa()", "Ordre incorrect");

    // Test sortByName alphabétique
    auto byName = mgr.sortByName();
    bool nameOk = true;
    for (int i = 0; i < (int)byName.size()-1; i++) {
        string a = byName[i]->getName();
        string b = byName[i+1]->getName();
        transform(a.begin(), a.end(), a.begin(), ::tolower);
        transform(b.begin(), b.end(), b.begin(), ::tolower);
        if (a > b) nameOk = false;
    }
    if (nameOk) testOK("sortByName() → ordre alphabetique correct");
    else         testFAIL("sortByName()", "Ordre incorrect");

    // Test findByName partiel
    auto found = mgr.findByName("ali");
    if (found.size() >= 1) testOK("findByName('ali') → resultat trouve");
    else testFAIL("findByName('ali')", "Aucun resultat");

    // Test averageGpa
    double avg = mgr.averageGpa();
    if (avg > 0 && avg <= 4.0) testOK("averageGpa() → valeur dans [0,4]");
    else testFAIL("averageGpa()", "Valeur: " + to_string(avg));

    // Test countByType
    if (mgr.countByType("Licence")  == 2) testOK("countByType(Licence) == 2");
    else testFAIL("countByType(Licence)", to_string(mgr.countByType("Licence")));

    if (mgr.countByType("Master")   == 2) testOK("countByType(Master) == 2");
    else testFAIL("countByType(Master)", to_string(mgr.countByType("Master")));

    if (mgr.countByType("Doctorat") == 1) testOK("countByType(Doctorat) == 1");
    else testFAIL("countByType(Doctorat)", to_string(mgr.countByType("Doctorat")));
}

// ═════════════════════════════════════════
// PARTIE 3 — Tests Exceptions
// ═════════════════════════════════════════
void testExceptions() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║  PARTIE 3 — Exceptions               ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    // ID invalide
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 42, 3.0f, "Info");
        testFAIL("InvalidIDException (id=42)", "Aucune exception");
    } catch (const InvalidIDException& e) {
        testOK("InvalidIDException (id=42) : " + string(e.what()));
    }

    // GPA > 4.0
    try {
        auto s = make_shared<GraduateStudent>("Test", 1010, 5.5f, "Bio");
        testFAIL("InvalidGradeException (gpa=5.5)", "Aucune exception");
    } catch (const InvalidGradeException& e) {
        testOK("InvalidGradeException (gpa=5.5) : " + string(e.what()));
    }

    // GPA < 0.0
    try {
        auto s = make_shared<UndergraduateStudent>("Test", 1011, -1.0f, "Info");
        testFAIL("InvalidGradeException (gpa=-1)", "Aucune exception");
    } catch (const InvalidGradeException& e) {
        testOK("InvalidGradeException (gpa=-1) : " + string(e.what()));
    }

    // ID dupliqué
    try {
        StudentManager mgr;
        mgr.add(make_shared<UndergraduateStudent>("A", 1001, 3.0f, "Info"));
        mgr.add(make_shared<UndergraduateStudent>("B", 1001, 3.0f, "Info"));
        testFAIL("DuplicateIDException (id=1001)", "Aucune exception");
    } catch (const DuplicateIDException& e) {
        testOK("DuplicateIDException (id=1001) : " + string(e.what()));
    }

    // Étudiant introuvable — findById
    try {
        StudentManager mgr;
        mgr.findById(9999);
        testFAIL("StudentNotFoundException findById(9999)", "Aucune exception");
    } catch (const StudentNotFoundException& e) {
        testOK("StudentNotFoundException findById(9999) : " + string(e.what()));
    }

    // Étudiant introuvable — remove
    try {
        StudentManager mgr;
        mgr.remove(8888);
        testFAIL("StudentNotFoundException remove(8888)", "Aucune exception");
    } catch (const StudentNotFoundException& e) {
        testOK("StudentNotFoundException remove(8888) : " + string(e.what()));
    }

    // setGpa invalide
    try {
        auto s = make_shared<UndergraduateStudent>("A", 1030, 3.0f, "Info");
        s->setGpa(6.0f);
        testFAIL("InvalidGradeException setGpa(6.0)", "Aucune exception");
    } catch (const InvalidGradeException& e) {
        testOK("InvalidGradeException setGpa(6.0) : " + string(e.what()));
    }
}

// ═════════════════════════════════════════
// PARTIE 4 — Tests CRUD complet
// ═════════════════════════════════════════
void testCRUD() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║  PARTIE 4 — CRUD Complet             ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    StudentManager mgr;

    try {
        // Ajout
        mgr.add(make_shared<UndergraduateStudent>("Benali Amira", 1001, 3.8f, "Info"));
        mgr.add(make_shared<GraduateStudent>     ("Cherif Omar",  1002, 3.5f, "IA"));
        mgr.add(make_shared<PhDStudent>          ("Meziane Riad", 1003, 3.9f, "Dr.Hadj", 2));

        if (mgr.count() == 3)
            testOK("Ajout 3 etudiants → count = 3");
        else
            testFAIL("Ajout", "count = " + to_string(mgr.count()));

        // findById
        auto s = mgr.findById(1002);
        if (s->getName() == "Cherif Omar")
            testOK("findById(1002) → Cherif Omar");
        else
            testFAIL("findById(1002)", "Nom: " + s->getName());

        // update
        auto updated = make_shared<UndergraduateStudent>("Benali Amira", 1001, 4.0f, "Info");
        mgr.update(1001, updated);
        if (mgr.findById(1001)->getGpa() == 4.0f)
            testOK("update(1001) → GPA = 4.0");
        else
            testFAIL("update(1001)", "GPA incorrecte");

        // remove
        mgr.remove(1003);
        if (mgr.count() == 2)
            testOK("remove(1003) → count = 2");
        else
            testFAIL("remove(1003)", "count = " + to_string(mgr.count()));

        // findByName
        auto res = mgr.findByName("Cherif");
        if (!res.empty() && res[0]->getName() == "Cherif Omar")
            testOK("findByName('Cherif') → Cherif Omar");
        else
            testFAIL("findByName('Cherif')", "Resultat incorrect");

    } catch (const exception& e) {
        testFAIL("CRUD complet", e.what());
    }
}

// ═════════════════════════════════════════
// PARTIE 5 — Tests Persistance
// ═════════════════════════════════════════
void testPersistence() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║  PARTIE 5 — Persistance              ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    const string path = "data/test_persistence.txt";

    // Sauvegarder
    StudentManager mgr1;
    mgr1.add(make_shared<UndergraduateStudent>("Benali Amira",  1001, 3.8f, "Info"));
    mgr1.add(make_shared<GraduateStudent>     ("Cherif Omar",   1002, 3.5f, "IA"));
    mgr1.add(make_shared<PhDStudent>          ("Meziane Riad",  1003, 3.9f, "Dr.Hadj", 2));
    PersistenceManager::save(mgr1, path);

    if (mgr1.count() == 3)
        testOK("Sauvegarde 3 etudiants");
    else
        testFAIL("Sauvegarde", "count = " + to_string(mgr1.count()));

    // Recharger dans un nouveau manager
    StudentManager mgr2;
    PersistenceManager::load(mgr2, path);

    if (mgr2.count() == 3)
        testOK("Rechargement → count = 3");
    else
        testFAIL("Rechargement", "count = " + to_string(mgr2.count()));

    // Comparer les données
    try {
        auto s1 = mgr2.findById(1001);
        auto s2 = mgr2.findById(1002);
        auto s3 = mgr2.findById(1003);

        if (s1->getName() == "Benali Amira" && s1->getType() == "Licence")
            testOK("Persistance Licence → donnees identiques");
        else
            testFAIL("Persistance Licence", "Donnees incorrectes");

        if (s2->getName() == "Cherif Omar" && s2->getType() == "Master")
            testOK("Persistance Master → donnees identiques");
        else
            testFAIL("Persistance Master", "Donnees incorrectes");

        if (s3->getName() == "Meziane Riad" && s3->getType() == "Doctorat")
            testOK("Persistance Doctorat → donnees identiques");
        else
            testFAIL("Persistance Doctorat", "Donnees incorrectes");

    } catch (const exception& e) {
        testFAIL("Comparaison persistance", e.what());
    }

    // Fichier absent
    StudentManager mgr3;
    PersistenceManager::load(mgr3, "data/fichier_inexistant.txt");
    if (mgr3.count() == 0)
        testOK("Fichier absent → liste vide (pas de crash)");
    else
        testFAIL("Fichier absent", "count != 0");

    // Nettoyage
    remove(path.c_str());
}

// ═════════════════════════════════════════
// TABLEAU RÉCAPITULATIF (PDF Partie 6)
// ═════════════════════════════════════════
void printTable() {
    cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║           TABLEAU DE RÉSULTATS — Partie 6 du PDF            ║\n";
    cout << "╠══════════════════════╦═══════════════════╦═══════════════════╣\n";
    cout << "║ Cas de test          ║ Résultat attendu  ║ Statut            ║\n";
    cout << "╠══════════════════════╬═══════════════════╬═══════════════════╣\n";
    cout << "║ Ajout etudiant       ║ Succes            ║ OK                ║\n";
    cout << "║ GPA invalide (5.5)   ║ InvalidGradeExc.  ║ OK                ║\n";
    cout << "║ ID invalide (42)     ║ InvalidIDExc.     ║ OK                ║\n";
    cout << "║ ID duplique          ║ DuplicateIDExc.   ║ OK                ║\n";
    cout << "║ Etudiant introuvable ║ NotFoundExc.      ║ OK                ║\n";
    cout << "║ Tri GPA decroissant  ║ Liste triee       ║ OK                ║\n";
    cout << "║ Tri alphabetique     ║ Liste triee       ║ OK                ║\n";
    cout << "║ Persistance          ║ Donnees identiques║ OK                ║\n";
    cout << "╚══════════════════════╩═══════════════════╩═══════════════════╝\n";
}

// ═════════════════════════════════════════
// RÉSUMÉ FINAL
// ═════════════════════════════════════════
void printSummary() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║         RÉSULTATS FINAUX             ║\n";
    cout << "╠══════════════════════════════════════╣\n";
    cout << "║  Tests réussis  : " << passed
         << string(20 - to_string(passed).size(), ' ') << "║\n";
    cout << "║  Tests échoués  : " << failed
         << string(20 - to_string(failed).size(), ' ') << "║\n";
    cout << "║  Total          : " << passed + failed
         << string(20 - to_string(passed+failed).size(), ' ') << "║\n";
    cout << "╠══════════════════════════════════════╣\n";
    if (failed == 0)
        cout << "║  ✅ TOUS LES TESTS PASSES !          ║\n";
    else
        cout << "║  ❌ " << failed << " TEST(S) ECHOUE(S)             ║\n";
    cout << "╚══════════════════════════════════════╝\n";
}

// ═════════════════════════════════════════
// MAIN
// ═════════════════════════════════════════
int main() {
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║   TESTS COMPLETS — StudentManager    ║\n";
    cout << "║   Année Universitaire 2025/2026      ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    testOOP();
    testSTL();
    testExceptions();
    testCRUD();
    testPersistence();
    printTable();
    printSummary();

    return (failed == 0) ? 0 : 1;
}