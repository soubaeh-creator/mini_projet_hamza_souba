#include "MainWindow.h"
#include "StudentDialog.h"
#include "StatsDialog.h"
#include "exceptions/StudentExceptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
using namespace std;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    setupConnections();
    PersistenceManager::load(manager, FILE_PATH);
    refreshTable();
    updateStatusBar();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    setWindowTitle("Student Manager — IMS 2025/2026");
    setMinimumSize(1050, 680);

    // ── Icône ──
    setWindowIcon(QIcon(":/icon.svg"));

    QWidget*     central    = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    setCentralWidget(central);

    // ── Header ──
    QWidget* header = new QWidget(this);
    header->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "stop:0 #2c3e50, stop:1 #3498db);"
        "border-radius: 10px;"
        "padding: 4px;"
    );
    QHBoxLayout* headerLayout = new QHBoxLayout(header);

    QLabel* logo = new QLabel("🎓", header);
    logo->setStyleSheet("font-size: 32px; background: transparent;");

    QVBoxLayout* titleLayout = new QVBoxLayout();
    QLabel* title = new QLabel("Système de Gestion des Étudiants", header);
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: white;"
        "background: transparent;");

    QLabel* subtitle = new QLabel("IMS — Année Universitaire 2025/2026", header);
    subtitle->setStyleSheet(
        "font-size: 12px; color: #bdc3c7; background: transparent;");

    titleLayout->addWidget(title);
    titleLayout->addWidget(subtitle);
    titleLayout->setSpacing(2);

    headerLayout->addWidget(logo);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    header->setFixedHeight(70);
    mainLayout->addWidget(header);

    // ── Barre recherche + filtre ──
    QWidget* searchBox = new QWidget(this);
    searchBox->setStyleSheet(
        "background-color: white; border-radius: 8px;"
        "border: 1px solid #dce1e7;");
    QHBoxLayout* searchLayout = new QHBoxLayout(searchBox);
    searchLayout->setContentsMargins(10, 6, 10, 6);

    QLabel* searchIcon = new QLabel("🔍", this);
    searchIcon->setStyleSheet("background: transparent; font-size: 14px;");
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Rechercher un étudiant par nom...");
    searchBar->setStyleSheet(
        "border: none; font-size: 13px;"
        "background: transparent; color: #2c3e50;");

    searchLayout->addWidget(searchIcon);
    searchLayout->addWidget(searchBar);
    mainLayout->addWidget(searchBox);

    // ── Filtres ──
    QWidget* filterBox = new QWidget(this);
    filterBox->setStyleSheet(
        "background-color: white; border-radius: 8px;"
        "border: 1px solid #dce1e7;");
    QHBoxLayout* filterLayout = new QHBoxLayout(filterBox);
    filterLayout->setContentsMargins(10, 6, 10, 6);

    QLabel* lblType = new QLabel("📋 Type :", this);
    lblType->setStyleSheet("color: #2c3e50; font-weight: bold; background: transparent;");
    filterType = new QComboBox(this);
    filterType->addItems({"Tous", "Licence", "Master", "Doctorat"});
    filterType->setStyleSheet(
        "padding: 4px 8px; border: 1px solid #dce1e7;"
        "border-radius: 4px; background: #f8f9fa; color: #2c3e50;");

    QLabel* lblMin = new QLabel("GPA min :", this);
    lblMin->setStyleSheet("color: #2c3e50; font-weight: bold; background: transparent;");
    filterGpaMin = new QDoubleSpinBox(this);
    filterGpaMin->setRange(0.0, 4.0);
    filterGpaMin->setSingleStep(0.1);
    filterGpaMin->setValue(0.0);
    filterGpaMin->setStyleSheet(
        "padding: 4px; border: 1px solid #dce1e7;"
        "border-radius: 4px; background: #f8f9fa;");

    QLabel* lblMax = new QLabel("GPA max :", this);
    lblMax->setStyleSheet("color: #2c3e50; font-weight: bold; background: transparent;");
    filterGpaMax = new QDoubleSpinBox(this);
    filterGpaMax->setRange(0.0, 4.0);
    filterGpaMax->setSingleStep(0.1);
    filterGpaMax->setValue(4.0);
    filterGpaMax->setStyleSheet(
        "padding: 4px; border: 1px solid #dce1e7;"
        "border-radius: 4px; background: #f8f9fa;");

    btnFilter = new QPushButton("Filtrer", this);
    btnFilter->setStyleSheet(
        "background-color: #16a085; color: white;"
        "padding: 6px 16px; border-radius: 6px;"
        "font-weight: bold; font-size: 12px;");

    btnReset = new QPushButton("Reset", this);
    btnReset->setStyleSheet(
        "background-color: #95a5a6; color: white;"
        "padding: 6px 16px; border-radius: 6px;"
        "font-weight: bold; font-size: 12px;");

    filterLayout->addWidget(lblType);
    filterLayout->addWidget(filterType);
    filterLayout->addSpacing(10);
    filterLayout->addWidget(lblMin);
    filterLayout->addWidget(filterGpaMin);
    filterLayout->addSpacing(10);
    filterLayout->addWidget(lblMax);
    filterLayout->addWidget(filterGpaMax);
    filterLayout->addSpacing(10);
    filterLayout->addWidget(btnFilter);
    filterLayout->addWidget(btnReset);
    filterLayout->addStretch();
    mainLayout->addWidget(filterBox);

    // ── Tableau ──
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Type", "GPA", "Bourse (EUR)"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(false);
    table->verticalHeader()->setDefaultSectionSize(40);
    table->setShowGrid(false);
    table->setStyleSheet(
        "QTableWidget {"
        "  background-color: white;"
        "  border-radius: 8px;"
        "  border: 1px solid #dce1e7;"
        "  font-size: 13px;"
        "}"
        "QTableWidget::item {"
        "  padding: 8px;"
        "  border-bottom: 1px solid #f0f0f0;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color: #d6eaf8;"
        "  color: #2c3e50;"
        "}"
        "QHeaderView::section {"
        "  background-color: #2c3e50;"
        "  color: white;"
        "  padding: 10px;"
        "  border: none;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"
        "QHeaderView::section:first {"
        "  border-top-left-radius: 8px;"
        "}"
        "QHeaderView::section:last {"
        "  border-top-right-radius: 8px;"
        "}"
    );
    mainLayout->addWidget(table);

    // ── Boutons ──
    QWidget* btnBox = new QWidget(this);
    btnBox->setStyleSheet(
        "background-color: white; border-radius: 8px;"
        "border: 1px solid #dce1e7;");
    QHBoxLayout* btnLayout = new QHBoxLayout(btnBox);
    btnLayout->setContentsMargins(12, 8, 12, 8);
    btnLayout->setSpacing(8);

    auto makeBtn = [](const QString& text, const QString& color,
                      const QString& icon) -> QPushButton* {
        QPushButton* btn = new QPushButton(icon + "  " + text);
        btn->setStyleSheet(QString(
            "QPushButton {"
            "  background-color: %1;"
            "  color: white;"
            "  padding: 10px 18px;"
            "  border-radius: 8px;"
            "  font-weight: bold;"
            "  font-size: 13px;"
            "  border: none;"
            "}"
            "QPushButton:hover {"
            "  background-color: %2;"
            "}"
            "QPushButton:pressed {"
            "  background-color: %3;"
            "}"
        ).arg(color)
         .arg(QColor(color).lighter(115).name())
         .arg(QColor(color).darker(115).name()));
        return btn;
    };

    btnAdd      = makeBtn("Ajouter",      "#27ae60", "➕");
    btnEdit     = makeBtn("Modifier",     "#2980b9", "✏️");
    btnDelete   = makeBtn("Supprimer",    "#e74c3c", "🗑️");
    btnRefresh  = makeBtn("Rafraîchir",   "#e67e22", "🔄");
    btnExportCSV= makeBtn("Export CSV",   "#8e44ad", "📤");
    btnStats    = makeBtn("Statistiques", "#d35400", "📊");
    btnDarkMode = makeBtn("Mode Sombre",  "#2c3e50", "🌙");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addWidget(btnRefresh);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExportCSV);
    btnLayout->addWidget(btnStats);
    btnLayout->addWidget(btnDarkMode);
    mainLayout->addWidget(btnBox);

    // ── Status bar ──
    statusLabel = new QLabel("", this);
    statusLabel->setStyleSheet("color: #555; font-size: 12px; padding: 2px 8px;");
    statusBar()->addWidget(statusLabel);
    statusBar()->setStyleSheet(
        "QStatusBar { background-color: #f8f9fa; border-top: 1px solid #dce1e7; }");
}

void MainWindow::setupConnections() {
    connect(btnAdd,     &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(btnEdit,    &QPushButton::clicked, this, &MainWindow::onEditStudent);
    connect(btnDelete,  &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::onRefresh);
    connect(searchBar,  &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(btnExportCSV,&QPushButton::clicked, this, &MainWindow::onExportCSV);
    connect(btnFilter,  &QPushButton::clicked, this, &MainWindow::onFilterChanged);
    connect(btnReset,   &QPushButton::clicked, this, &MainWindow::onRefresh);
    connect(btnStats,   &QPushButton::clicked, this, &MainWindow::onShowStats);
    connect(btnDarkMode,&QPushButton::clicked, this, &MainWindow::onToggleDarkMode);
    connect(filterType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterChanged);
}

void MainWindow::onAddStudent() {
    StudentDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        try {
            manager.add(dlg.getStudent());
            saveData();
            refreshTable();
            updateStatusBar();
            statusBar()->showMessage("✅ Etudiant ajouté avec succès.", 3000);
        } catch (const DuplicateIDException& e) {
            QMessageBox::critical(this, "Erreur ID", QString(e.what()));
        } catch (const InvalidIDException& e) {
            QMessageBox::critical(this, "Erreur ID", QString(e.what()));
        } catch (const InvalidGradeException& e) {
            QMessageBox::warning(this, "Erreur GPA", QString(e.what()));
        } catch (const exception& e) {
            QMessageBox::critical(this, "Erreur", QString(e.what()));
        }
    }
}

void MainWindow::onEditStudent() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Info",
            "Veuillez sélectionner un étudiant.");
        return;
    }
    int id = table->item(row, 0)->text().toInt();
    try {
        auto s = manager.findById(id);
        StudentDialog dlg(s, this);
        if (dlg.exec() == QDialog::Accepted) {
            manager.update(id, dlg.getStudent());
            saveData();
            refreshTable();
            updateStatusBar();
            statusBar()->showMessage("✅ Etudiant modifié.", 3000);
        }
    } catch (const exception& e) {
        QMessageBox::critical(this, "Erreur", QString(e.what()));
    }
}

void MainWindow::onDeleteStudent() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Info",
            "Veuillez sélectionner un étudiant.");
        return;
    }
    int     id  = table->item(row, 0)->text().toInt();
    QString nom = table->item(row, 1)->text();

    auto reply = QMessageBox::question(this, "Confirmation",
        "Supprimer " + nom + " ?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        try {
            manager.remove(id);
            saveData();
            refreshTable();
            updateStatusBar();
            statusBar()->showMessage("🗑️ Etudiant supprimé.", 3000);
        } catch (const exception& e) {
            QMessageBox::critical(this, "Erreur", QString(e.what()));
        }
    }
}

void MainWindow::onSearchChanged(const QString& text) {
    if (text.isEmpty()) { refreshTable(); return; }
    refreshTable(manager.findByName(text.toStdString()));
}

void MainWindow::onRefresh() {
    searchBar->clear();
    filterType->setCurrentIndex(0);
    filterGpaMin->setValue(0.0);
    filterGpaMax->setValue(4.0);
    refreshTable();
    updateStatusBar();
    statusBar()->showMessage("🔄 Rafraîchi.", 2000);
}

void MainWindow::onFilterChanged() {
    string selectedType = filterType->currentText().toStdString();
    double gpaMin = filterGpaMin->value();
    double gpaMax = filterGpaMax->value();

    vector<shared_ptr<Student>> filtered;
    for (auto& pair : manager.getAll()) {
        auto& s = pair.second;
        bool typeOk = (selectedType == "Tous") ||
                      (s->getType() == selectedType);
        bool gpaOk  = (s->getGpa() >= gpaMin) &&
                      (s->getGpa() <= gpaMax);
        if (typeOk && gpaOk)
            filtered.push_back(s);
    }
    refreshTable(filtered);
    statusBar()->showMessage(
        QString("Filtre : %1 etudiant(s)").arg(filtered.size()), 3000);
}

void MainWindow::onExportCSV() {
    if (manager.count() == 0) {
        QMessageBox::warning(this, "Export CSV",
            "Aucun étudiant à exporter !");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(
        this, "Exporter en CSV", "data/etudiants.csv",
        "CSV Files (*.csv)");
    if (fileName.isEmpty()) return;
    try {
        PersistenceManager::exportCSV(manager, fileName.toStdString());
        QMessageBox::information(this, "Export CSV",
            "Export réussi !\nFichier : " + fileName);
    } catch (const exception& e) {
        QMessageBox::critical(this, "Erreur", QString(e.what()));
    }
}

void MainWindow::onShowStats() {
    if (manager.count() == 0) {
        QMessageBox::warning(this, "Statistiques",
            "Aucun étudiant pour les statistiques !");
        return;
    }
    vector<shared_ptr<Student>> list;
    for (auto& p : manager.getAll())
        list.push_back(p.second);
    StatsDialog dlg(list, this);
    dlg.exec();
}

void MainWindow::onToggleDarkMode() {
    isDarkMode = !isDarkMode;
    if (isDarkMode) {
        applyDarkTheme();
        btnDarkMode->setText("☀️  Mode Clair");
        statusBar()->showMessage("🌙 Mode sombre activé.", 2000);
    } else {
        applyLightTheme();
        btnDarkMode->setText("🌙  Mode Sombre");
        statusBar()->showMessage("☀️ Mode clair activé.", 2000);
    }
}

void MainWindow::applyDarkTheme() {
    setStyleSheet(
        "QMainWindow, QWidget { background-color: #1a1a2e; color: #e0e0e0; }"
        "QLabel { color: #e0e0e0; background: transparent; }"
        "QLineEdit {"
        "  background-color: #16213e; color: #e0e0e0;"
        "  border: 1px solid #0f3460; border-radius: 6px; padding: 6px;"
        "}"
        "QComboBox {"
        "  background-color: #16213e; color: #e0e0e0;"
        "  border: 1px solid #0f3460; border-radius: 4px; padding: 4px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #16213e; color: #e0e0e0;"
        "  selection-background-color: #0f3460;"
        "}"
        "QDoubleSpinBox {"
        "  background-color: #16213e; color: #e0e0e0;"
        "  border: 1px solid #0f3460; border-radius: 4px;"
        "}"
        "QTableWidget {"
        "  background-color: #16213e; color: #e0e0e0;"
        "  border: 1px solid #0f3460; border-radius: 8px; gridline-color: #0f3460;"
        "}"
        "QTableWidget::item:selected { background-color: #0f3460; color: white; }"
        "QHeaderView::section {"
        "  background-color: #0f3460; color: white;"
        "  padding: 10px; border: none; font-weight: bold;"
        "}"
        "QStatusBar { background-color: #0f3460; color: #e0e0e0; }"
        "QPushButton {"
        "  border-radius: 8px; padding: 10px 18px;"
        "  font-weight: bold; color: white; border: none;"
        "}"
    );
}

void MainWindow::applyLightTheme() {
    setStyleSheet("");
    setupUI();
    setupConnections();
}

void MainWindow::refreshTable() {
    vector<shared_ptr<Student>> list;
    for (auto& p : manager.getAll())
        list.push_back(p.second);
    refreshTable(list);
}

void MainWindow::refreshTable(const vector<shared_ptr<Student>>& list) {
    table->setRowCount(0);
    for (auto& s : list) {
        int row = table->rowCount();
        table->insertRow(row);

        auto i0 = new QTableWidgetItem(QString::number(s->getId()));
        auto i1 = new QTableWidgetItem(QString::fromStdString(s->getName()));
        auto i2 = new QTableWidgetItem(QString::fromStdString(s->getType()));
        auto i3 = new QTableWidgetItem(QString::number(s->getGpa(), 'f', 2));
        auto i4 = new QTableWidgetItem(
                    QString::number(s->computeScholarship(), 'f', 0) + " EUR");

        i0->setTextAlignment(Qt::AlignCenter);
        i2->setTextAlignment(Qt::AlignCenter);
        i3->setTextAlignment(Qt::AlignCenter);
        i4->setTextAlignment(Qt::AlignCenter);

        // Badge couleur selon type
        QColor bg, fg;
        if (s->getType() == "Licence") {
            bg = QColor(232, 245, 232); fg = QColor(39, 174, 96);
        } else if (s->getType() == "Master") {
            bg = QColor(232, 240, 254); fg = QColor(41, 128, 185);
        } else {
            bg = QColor(254, 245, 232); fg = QColor(211, 84, 0);
        }

        i2->setForeground(fg);
        i2->setFont(QFont("Arial", 10, QFont::Bold));

        for (auto* item : {i0, i1, i2, i3, i4})
            item->setBackground(bg);

        table->setItem(row, 0, i0);
        table->setItem(row, 1, i1);
        table->setItem(row, 2, i2);
        table->setItem(row, 3, i3);
        table->setItem(row, 4, i4);
    }
}

void MainWindow::updateStatusBar() {
    QString msg = QString(
        "👥 Total : %1 étudiant(s)   |   📊 Moyenne GPA : %2")
        .arg(manager.count())
        .arg(manager.averageGpa(), 0, 'f', 2);
    statusLabel->setText(msg);
}

void MainWindow::saveData() {
    PersistenceManager::save(manager, FILE_PATH);
}