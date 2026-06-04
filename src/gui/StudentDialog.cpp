#include "StudentDialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
using namespace std;

StudentDialog::StudentDialog(QWidget* parent)
    : QDialog(parent), isEditMode(false) {
    setWindowTitle("Ajouter un etudiant");
    setupUI();
}

StudentDialog::StudentDialog(shared_ptr<Student> student, QWidget* parent)
    : QDialog(parent), isEditMode(true) {
    setWindowTitle("Modifier un etudiant");
    setupUI();
    fillFromStudent(student);
}

void StudentDialog::setupUI() {
    setMinimumWidth(420);
    setModal(true);

    QVBoxLayout* main = new QVBoxLayout(this);
    main->setSpacing(12);
    main->setContentsMargins(20, 20, 20, 20);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);

    QString fieldStyle = "padding: 6px; border: 1px solid #ccc;"
                         "border-radius: 4px; font-size: 13px;";

    // Type
    typeBox = new QComboBox(this);
    typeBox->addItems({"Licence", "Master", "Doctorat"});
    typeBox->setStyleSheet(fieldStyle);
    form->addRow("Type :", typeBox);

    // Nom
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Ex: Benali Amira");
    nameEdit->setStyleSheet(fieldStyle);
    form->addRow("Nom :", nameEdit);

    // ID
    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText("Ex: 1001 (>= 1000)");
    idEdit->setStyleSheet(fieldStyle);
    form->addRow("ID :", idEdit);

    // GPA
    gpaSpinBox = new QDoubleSpinBox(this);
    gpaSpinBox->setRange(0.0, 4.0);
    gpaSpinBox->setSingleStep(0.1);
    gpaSpinBox->setDecimals(2);
    gpaSpinBox->setStyleSheet(fieldStyle);
    form->addRow("GPA :", gpaSpinBox);

    // Champ dynamique
    extraLabel = new QLabel("Filiere :", this);
    extraEdit  = new QLineEdit(this);
    extraEdit->setPlaceholderText("Ex: Informatique");
    extraEdit->setStyleSheet(fieldStyle);
    form->addRow(extraLabel, extraEdit);

    // Champ PhD
    extraLabel2 = new QLabel("Annee :", this);
    yearSpinBox = new QSpinBox(this);
    yearSpinBox->setRange(1, 8);
    yearSpinBox->setStyleSheet(fieldStyle);
    yearSpinBox->hide();
    extraLabel2->hide();
    form->addRow(extraLabel2, yearSpinBox);

    main->addLayout(form);

    // Boutons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnOk     = new QPushButton("Valider",  this);
    btnCancel = new QPushButton("Annuler",  this);
    btnOk->setStyleSheet(
        "background-color: #27ae60; color: white;"
        "padding: 8px 20px; border-radius: 4px; font-weight: bold;");
    btnCancel->setStyleSheet(
        "background-color: #c0392b; color: white;"
        "padding: 8px 20px; border-radius: 4px; font-weight: bold;");
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    main->addLayout(btnLayout);

    connect(typeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StudentDialog::onTypeChanged);
    connect(btnOk,     &QPushButton::clicked, this, &StudentDialog::onValidate);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void StudentDialog::onTypeChanged(int index) {
    if (index == 0) {
        extraLabel->setText("Filiere :");
        extraEdit->setPlaceholderText("Ex: Informatique");
        extraLabel2->hide(); yearSpinBox->hide();
    } else if (index == 1) {
        extraLabel->setText("Sujet de recherche :");
        extraEdit->setPlaceholderText("Ex: Intelligence Artificielle");
        extraLabel2->hide(); yearSpinBox->hide();
    } else {
        extraLabel->setText("Directeur :");
        extraEdit->setPlaceholderText("Ex: Dr. Hadj");
        extraLabel2->show(); yearSpinBox->show();
    }
}

void StudentDialog::onValidate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Champ manquant", "Le nom est obligatoire.");
        return;
    }
    if (idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Champ manquant", "L'ID est obligatoire.");
        return;
    }
    if (extraEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Champ manquant", "Le champ specifique est obligatoire.");
        return;
    }
    bool ok;
    idEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur ID", "L'ID doit etre un nombre entier.");
        return;
    }
    accept();
}

shared_ptr<Student> StudentDialog::getStudent() const {
    string name  = nameEdit->text().trimmed().toStdString();
    int    id    = idEdit->text().toInt();
    float  gpa   = static_cast<float>(gpaSpinBox->value());
    string extra = extraEdit->text().trimmed().toStdString();
    int    type  = typeBox->currentIndex();

    if (type == 0)
        return make_shared<UndergraduateStudent>(name, id, gpa, extra);
    else if (type == 1)
        return make_shared<GraduateStudent>(name, id, gpa, extra);
    else
        return make_shared<PhDStudent>(name, id, gpa, extra, yearSpinBox->value());
}

void StudentDialog::fillFromStudent(shared_ptr<Student> s) {
    nameEdit->setText(QString::fromStdString(s->getName()));
    idEdit->setText(QString::number(s->getId()));
    idEdit->setReadOnly(true);
    gpaSpinBox->setValue(static_cast<double>(s->getGpa()));
    typeBox->setEnabled(false);

    if (s->getType() == "Licence") {
        typeBox->setCurrentIndex(0);
        auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
        if (u) extraEdit->setText(QString::fromStdString(u->getMajor()));
    } else if (s->getType() == "Master") {
        typeBox->setCurrentIndex(1);
        auto g = dynamic_pointer_cast<GraduateStudent>(s);
        if (g) extraEdit->setText(QString::fromStdString(g->getResearchTopic()));
    } else {
        typeBox->setCurrentIndex(2);
        auto p = dynamic_pointer_cast<PhDStudent>(s);
        if (p) {
            extraEdit->setText(QString::fromStdString(p->getSupervisor()));
            yearSpinBox->setValue(p->getYearOfStudy());
            extraLabel2->show();
            yearSpinBox->show();
        }
    }
}