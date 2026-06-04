#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <memory>
#include <vector>
#include "models/Student.h"
using namespace std;

class StatsWidget : public QWidget {
    Q_OBJECT
public:
    explicit StatsWidget(const vector<shared_ptr<Student>>& students,
                         QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    vector<shared_ptr<Student>> students;

    // Compte étudiants par plage GPA
    int countRange(float min, float max) const;
};

class StatsDialog : public QDialog {
    Q_OBJECT
public:
    explicit StatsDialog(const vector<shared_ptr<Student>>& students,
                         QWidget* parent = nullptr);
};

#endif