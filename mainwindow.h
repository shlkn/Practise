#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>
#include <QSqlTableModel>
#include <QSqlError>
#include <QErrorMessage>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    //sql
    bool ConnectToDB();
    void connectTables();
    //UI conection
    //add
    void AddCustomer();
    void AddTypography();
    void AddPublicationInfo();
    //delete
    void DeleteCustomer();
    void DeleteTypo();
    void DeleteIzdat();

    //find
    void FindCustomer();
};

#endif // MAINWINDOW_H
