#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <memory>
#include "models/Student.h"
#include "models/UndergraduateStudent.h"
#include "models/GraduateStudent.h"
#include "models/PhDStudent.h"
using namespace std;

class StudentDialog : public QDialog {
    Q_OBJECT

public:
    explicit StudentDialog(QWidget* parent = nullptr);
    explicit StudentDialog(shared_ptr<Student> student, QWidget* parent = nullptr);
    shared_ptr<Student> getStudent() const;

private slots:
    void onTypeChanged(int index);
    void onValidate();

private:
    QComboBox*      typeBox;
    QLineEdit*      nameEdit;
    QLineEdit*      idEdit;
    QDoubleSpinBox* gpaSpinBox;
    QLabel*         extraLabel;
    QLineEdit*      extraEdit;
    QLabel*         extraLabel2;
    QSpinBox*       yearSpinBox;
    QPushButton*    btnOk;
    QPushButton*    btnCancel;
    bool            isEditMode;

    void setupUI();
    void fillFromStudent(shared_ptr<Student> s);
};

#endif