#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "StatsDialog.h" 
#include <QComboBox>    
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <memory>
#include <vector>
#include "services/StudentManager.h"
#include "persistence/PersistenceManager.h"
using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onSearchChanged(const QString& text);
    void onRefresh();
    void onExportCSV();
    void onFilterChanged();
    void onShowStats();
    void onToggleDarkMode();

private:
    QTableWidget* table;
    QPushButton*  btnAdd;
    QPushButton*  btnEdit;
    QPushButton*  btnDelete;
    QPushButton*  btnRefresh;
    QPushButton*  btnExportCSV;
    QLineEdit*    searchBar;
    QLabel*       statusLabel;
    QComboBox*      filterType;    
    QDoubleSpinBox* filterGpaMin;  
    QDoubleSpinBox* filterGpaMax;  
    QPushButton*    btnFilter;     
    QPushButton*    btnReset; 
    QPushButton* btnStats; 
    QPushButton* btnDarkMode;
    bool         isDarkMode = false;    

    StudentManager manager;
    const string   FILE_PATH = "data/students.txt";

    void setupUI();
    void setupConnections();
    void refreshTable();
    void refreshTable(const vector<shared_ptr<Student>>& list);
    void updateStatusBar();
    void saveData();
    void applyLightTheme();    
    void applyDarkTheme();    
};

#endif