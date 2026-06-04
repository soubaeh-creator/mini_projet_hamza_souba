#include "PersistenceManager.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// ─────────────────────────────────────────
// SAUVEGARDE
// ─────────────────────────────────────────
void PersistenceManager::save(const StudentManager& mgr, const string& path) {
    ofstream file(path);

    if (!file.is_open()) {
        cerr << "[ERREUR] Impossible d'ouvrir le fichier : " << path << "\n";
        return;
    }

    for (auto& pair : mgr.getAll()) {
        auto& s = pair.second;
        string type = s->getType();

        if (type == "Licence") {
            // Cast vers UndergraduateStudent pour accéder à major
            auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
            file << "Licence|" << s->getId()   << "|"
                               << s->getName()  << "|"
                               << s->getGpa()   << "|"
                               << u->getMajor() << "\n";

        } else if (type == "Master") {
            auto g = dynamic_pointer_cast<GraduateStudent>(s);
            file << "Master|" << s->getId()              << "|"
                              << s->getName()             << "|"
                              << s->getGpa()              << "|"
                              << g->getResearchTopic()    << "\n";

        } else if (type == "Doctorat") {
            auto p = dynamic_pointer_cast<PhDStudent>(s);
            file << "Doctorat|" << s->getId()            << "|"
                                << s->getName()           << "|"
                                << s->getGpa()            << "|"
                                << p->getSupervisor()     << "|"
                                << p->getYearOfStudy()    << "\n";
        }
    }

    file.close();
    cout << "[INFO] Sauvegarde reussie dans : " << path << "\n";
}

// ─────────────────────────────────────────
// CHARGEMENT
// ─────────────────────────────────────────
void PersistenceManager::load(StudentManager& mgr, const string& path) {
    ifstream file(path);

    if (!file.is_open()) {
        cout << "[INFO] Fichier absent, demarrage avec liste vide : " << path << "\n";
        return;
    }

    string line;
    int lignesChargees = 0;
    int lignesIgnorees = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        try {
            stringstream ss(line);
            string token;
            vector<string> tokens;

            // Découper la ligne par '|'
            while (getline(ss, token, '|'))
                tokens.push_back(token);

            string type = tokens[0];

            if (type == "Licence" && tokens.size() >= 5) {
                int   id  = stoi(tokens[1]);
                string nm = tokens[2];
                float gpa = stof(tokens[3]);
                string mj = tokens[4];
                mgr.add(make_shared<UndergraduateStudent>(nm, id, gpa, mj));

            } else if (type == "Master" && tokens.size() >= 5) {
                int   id  = stoi(tokens[1]);
                string nm = tokens[2];
                float gpa = stof(tokens[3]);
                string rt = tokens[4];
                mgr.add(make_shared<GraduateStudent>(nm, id, gpa, rt));

            } else if (type == "Doctorat" && tokens.size() >= 6) {
                int   id  = stoi(tokens[1]);
                string nm = tokens[2];
                float gpa = stof(tokens[3]);
                string sv = tokens[4];
                int   yr  = stoi(tokens[5]);
                mgr.add(make_shared<PhDStudent>(nm, id, gpa, sv, yr));

            } else {
                cerr << "[WARN] Ligne ignoree (format inconnu) : " << line << "\n";
                lignesIgnorees++;
                continue;
            }

            lignesChargees++;

        } catch (const exception& e) {
            // Ligne corrompue → on ignore silencieusement
            cerr << "[WARN] Ligne corrompue ignoree : " << line
                 << " (" << e.what() << ")\n";
            lignesIgnorees++;
        }
    }

    file.close();
    cout << "[INFO] Chargement : " << lignesChargees << " etudiants charges"
         << (lignesIgnorees ? ", " + to_string(lignesIgnorees) + " ligne(s) ignoree(s)" : "")
         << "\n";
}




// ─────────────────────────────────────────
// EXPORT
// ─────────────────────────────────────────
void PersistenceManager::exportCSV(const StudentManager& mgr, const string& path) {
    ofstream file(path);
    if (!file.is_open()) {
        cerr << "[ERREUR] Impossible de creer le fichier CSV : " << path << "\n";
        return;
    }
    file << "ID,Nom,Type,GPA,Bourse (EUR),Details\n";
    for (auto& pair : mgr.getAll()) {
        auto& s = pair.second;
        file << s->getId()   << ","
             << s->getName() << ","
             << s->getType() << ","
             << s->getGpa()  << ","
             << s->computeScholarship() << ",";
        if (s->getType() == "Licence") {
            auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
            if (u) file << "Filiere: " << u->getMajor();
        } else if (s->getType() == "Master") {
            auto g = dynamic_pointer_cast<GraduateStudent>(s);
            if (g) file << "Sujet: " << g->getResearchTopic();
        } else if (s->getType() == "Doctorat") {
            auto p = dynamic_pointer_cast<PhDStudent>(s);
            if (p) file << "Directeur: " << p->getSupervisor()
                        << " Annee: "    << p->getYearOfStudy();
        }
        file << "\n";
    }
    file.close();
    cout << "[INFO] Export CSV reussi : " << path << "\n";
}