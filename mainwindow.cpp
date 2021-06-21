#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    tbCustomer = new QSqlQueryModel;
    tbPublisher = new QSqlQueryModel;
    tbTypography = new QSqlQueryModel;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    ui->setupUi(this);



    if (!ConnectToDB())
        while (true)
            QApplication::quit();
    connectTables();
    setlocale(LC_ALL, "Russian");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//sql
bool MainWindow::ConnectToDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("PublishingHouse");

    db.setUserName("accauntent");
    db.setHostName("companybase");
    db.setPassword("secretCoDe");
    //qDebug() << db.lastError().text();
    if (!db.open())
    {

        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromLocal8Bit("Ошибка"));
        msgBox.setText(QString::fromLocal8Bit("База данных не подключена"));
        msgBox.exec();
        qDebug()<<db.lastError().text();
        return false;
    }
    return true;
}

void MainWindow::connectTables()
{
    QSqlQuery query;


    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));


    QString tableCustomer = "CREATE TABLE IF NOT EXISTS Customer ( "
      "ID INTEGER PRIMARY KEY NOT NULL,"
      "CustomerSurname VARCHAR(40) NOT NULL,"
      "CustomerName VARCHAR(20) NOT NULL,"
      "CustomerSecondName VARCHAR(20) ,"
      "CustomerTel VARCHAR(20) NOT NULL,"
      "CustomerEmail VARCHAR(20) NOT NULL,"
      "CustomerAddress VARCHAR(20) NOT NULL,"
      "CustomerOrganization VARCHAR(20) NOT NULL,"
      "Order_number INTEGER NOT NULL )";

    if (!query.exec(tableCustomer))
    {
        QMessageBox msg;
        msg.setWindowTitle(QString::fromLocal8Bit("Внимание!"));
        msg.setText("Ошибка!\nНе удалось подключить таблицу заказчиков!"); //QString::fromLocal8Bit("Ошибка!\nНе удалось подключить таблицу заказчиков!"));
        msg.exec();
        return;
    }
    QString tableTypography = "CREATE TABLE IF NOT EXISTS Typography ("
      "ID INTEGER PRIMARY KEY NOT NULL,"
      "Name VARCHAR(20) NOT NULL,"
      "address VARCHAR(40) NOT NULL,"
      "Tel VARCHAR(20) NOT NULL,"
      "Email VARCHAR(20), "
      "Director VARCHAR(40) )";

    if (!query.exec(tableTypography))
    {
        QMessageBox msg;
        msg.setWindowTitle(QString::fromLocal8Bit("Внимание!"));
        msg.setText(QString::fromLocal8Bit("Ошибка!\nНе удалось подключить таблицу типографий!"));
        msg.exec();
        return;
    }



    QString tablePublicationInfo = "CREATE TABLE IF NOT EXISTS PublicationInfo ("
      "ID_publication INTEGER PRIMARY KEY NOT NULL,"
      "Author VARCHAR(20) NOT NULL,"
      "Name VARCHAR(40) NOT NULL,"
      "Count_of_list INTEGER NOT NULL,"
      "Circulation INTEGER NOT NULL, "
      "Order_number INTEGER NOT NULL, "
      "Publishing_house VARCHAR(40) NOT NULL )";
    if (!query.exec(tablePublicationInfo))
    {
        QMessageBox msg;
        msg.setWindowTitle(QString::fromLocal8Bit("Внимание!"));
        msg.setText(QString::fromLocal8Bit("Ошибка!\nНе удалось подключить таблицу изданий!"));
        msg.exec();
        return;
    }


    tbCustomer->setQuery("SELECT * FROM Customer;");
    tbPublisher->setQuery("SELECT * FROM PublicationInfo;");
    tbTypography->setQuery("SELECT * FROM Typography;");

    ui->CustomerTable->setModel(tbCustomer);
    ui->TypographyTable->setModel(tbTypography);
    ui->PublicationInfoTable->setModel(tbPublisher);

    connect(ui->AddCustomer, &QPushButton::clicked,this, &MainWindow::AddCustomer);
    connect(ui->AddTypography,&QPushButton::clicked,this,&MainWindow::AddTypography);
    connect(ui->AddPublicationInfo,&QPushButton::clicked,this,&MainWindow::AddPublicationInfo);
    connect(ui->DeleteCustomer,&QPushButton::clicked,this,&MainWindow::DeleteCustomer);
    connect(ui->FindCustomer,&QPushButton::clicked,this,&MainWindow::FindCustomer);
    connect(ui->DeleteTypography,&QPushButton::clicked,this,&MainWindow::DeleteTypo);
    connect(ui->DeleteIzdat,&QPushButton::clicked,this,&MainWindow::DeleteIzdat);
}

void MainWindow::AddCustomer()
{


    QString name, surname, secondName, tel, email, address, org; // temp for query
    int order_number;
    name = ui->Name->text();
    surname = ui->Surname->text();
    secondName = ui->SecondName->text();
    tel = ui->CustomerTel->text();
    email = ui->CustomerEmail->text();
    address = ui->CustomerAddress->text();
    org = ui->CustomerOrg->text();

    bool ok;
    order_number = ui->CustomerOrderNum->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение номера заказа либо отсутвует либо не является числом.");
    }



    if ((name != "") &&
        (surname != "") &&
        (secondName != "") &&
        (tel != "") &&
        (email != "") &&
        (address != "") &&
        (org != ""))
    {
        QSqlQuery query;


        QString strF = "INSERT INTO Customer (CustomerName, CustomerSurname, CustomerSecondName, CustomerTel, CustomerEmail, CustomerAddress, CustomerOrganization, Order_number)"
                      "VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8');";

        QString str = strF.arg(name)
                          .arg(surname)
                          .arg(secondName)
                          .arg(tel)
                          .arg(email)
                          .arg(address)
                          .arg(org)
                          .arg(order_number);

        if (!query.exec(str))
            qDebug()<<query.lastError().text()<<1;
        tbCustomer->setQuery("SELECT * FROM Customer;");
        ui->CustomerTable->setModel(tbCustomer);

    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Ошибка");
        msg.setText("Есть пустые поля, заполните их и повторите снова.");
        msg.exec();
        return;
    }
}

void MainWindow::AddTypography()
{
    QString name, address, tel, email, Director; // temp for query
    name = ui->TypoName->text();
    address = ui->TypoAddress->text();
    tel = ui->TypoTel->text();
    email = ui->TypoEmail->text();
    Director = ui->TypoDirector->text();



    if ((name != "") &&
        (address != "") &&
        (tel != "") &&
        (email != "") &&
        (Director != ""))
    {
        QSqlQuery query;


        QString strF = "INSERT INTO Typography (Name, address, Tel, Email, Director)"
                      "VALUES('%1', '%2', '%3', '%4', '%5');";

        QString str = strF.arg(name)
                          .arg(address)
                          .arg(tel)
                          .arg(email)
                          .arg(Director);

        if (!query.exec(str))
            qDebug()<<query.lastError().text()<<1;
        tbTypography->setQuery("SELECT * FROM Typography;");
        ui->TypographyTable->setModel(tbTypography);

    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Ошибка");
        msg.setText("Есть пустые поля, заполните их и повторите снова.");
        msg.exec();
        return;
    }
}

void MainWindow::AddPublicationInfo()
{
    QString author, name, publishing_house; // temp for query
    int publication_id, cnt_of_list, Circulation, Order_number;
    bool ok;


    publication_id = ui->IzdatID_publication->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение кода либо отсутвует либо не является числом.");
        msg.exec();
        return;
    }


    cnt_of_list = ui->IzdatCnt->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение объёма либо отсутвует либо не является числом.");
        msg.exec();
        return;
    }


    Circulation = ui->IzdatCirculation->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение тиража либо отсутвует либо не является числом.");
        msg.exec();
        return;
    }

    Order_number = ui->IzdatOrder_number->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение номера заказа либо отсутвует либо не является числом.");
        msg.exec();
        return;
    }

    name = ui->IzdatName->text();
    author = ui->IzdatAuthor->text();
    publishing_house = ui->Izdat->text();

    if ((name != "") &&
        (author != "") &&
         (publishing_house != ""))
    {
        QSqlQuery query;


        QString strF = "INSERT INTO PublicationInfo (ID_publication, Author, Name, Count_of_list, Circulation, Order_number, Publishing_house)"
                      "VALUES(%1, '%2', '%3', %4, %5, %6, '%7');";

        QString str = strF.arg(publication_id)
                          .arg(author)
                          .arg(name)
                          .arg(cnt_of_list)
                          .arg(Circulation)
                          .arg(Order_number)
                          .arg(publishing_house);

        if (!query.exec(str))
            qDebug()<<query.lastError().text()<<1;
        tbPublisher->setQuery("SELECT * FROM PublicationInfo;");
        ui->PublicationInfoTable->setModel(tbPublisher);

    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Ошибка");
        msg.setText("Есть пустые поля, заполните их и повторите снова.");
        msg.exec();
        return;
    }
}

void MainWindow::DeleteCustomer()
{
    QString str = ui->CustomerDelID->text();
    if (str == "")
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Нет ID для удаления!");
        msg.exec();
        return;
    }
    bool ok;
    int id = str.toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение ID не является числом.");
        msg.exec();
        return;
    }
    QSqlQuery query;
    str = "DELETE FROM Customer WHERE ID = %1;";
    QString cstr = str.arg(id);
    if (!query.exec(cstr))
        qDebug()<<query.lastError().text();
    tbCustomer->setQuery("SELECT * FROM Customer;");
    ui->CustomerTable->setModel(tbCustomer);
}

void MainWindow::DeleteTypo()
{
    QString str = ui->TypoDel->text();
    if (str == "")
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Нет ID для удаления!");
        msg.exec();
        return;
    }
    bool ok;
    int id = str.toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение ID не является числом.");
        msg.exec();
        return;
    }
    QSqlQuery query;
    str = "DELETE FROM Typography WHERE ID = %1;";
    QString cstr = str.arg(id);
    if (!query.exec(cstr))
        qDebug()<<query.lastError().text();
    tbTypography->setQuery("SELECT * FROM Typography;");
    ui->TypographyTable->setModel(tbTypography);
}

void MainWindow::DeleteIzdat()
{
    QString str = ui->IzdatDel->text();
    if (str == "")
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Нет ID для удаления!");
        msg.exec();
        return;
    }
    bool ok;
    int id = str.toInt(&ok);
    if(!ok)
    {
        QMessageBox msg;
        msg.setWindowTitle("Внимание!");
        msg.setText("Введённое значение ID не является числом.");
        msg.exec();
        return;
    }
    QSqlQuery query;
    str = "DELETE FROM PublicationInfo WHERE ID_publication = %1;";
    QString cstr = str.arg(id);
    if (!query.exec(cstr))
        qDebug()<<query.lastError().text();
    tbPublisher->setQuery("SELECT * FROM PublicationInfo;");
    ui->PublicationInfoTable->setModel(tbPublisher);
}

void MainWindow::FindCustomer()
{
    QString name, surname, secondName; // temp for query
    name = ui->CustomerFindName->text();
    surname = ui->CustomerFindSurname->text();
    secondName = ui->CustomerFindSecondName->text();

    if ((name != "") &&
        (surname != "") &&
        (secondName != ""))
    {
        QSqlQuery query;


        QString strF = "SELECT * FROM Customer WHERE (CustomerName = '%1') and "
                       "(CustomerSurname = '%2') and (CustomerSecondName = '%3');";


        QString str = strF.arg(name)
                          .arg(surname)
                          .arg(secondName);

        QSqlRecord rec;
        qDebug() << name << " " << surname << " " << secondName;
        if (!query.exec(str))
        {
            qDebug()<<query.lastError().text()<<1;
        }
        else
        {
            rec = query.record();
            if(query.next())
            {
                qDebug() << query.value(rec.indexOf("ID")).toString();
                QMessageBox msg;
                msg.setWindowTitle("Успех");
                QString temp = "id - ";
                temp.append(query.value(rec.indexOf("ID")).toString());
                msg.setText(temp);
                msg.exec();
                qDebug() << temp;
                return;
            }
        }


    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Ошибка");
        msg.setText("Есть пустые поля, заполните их и повторите снова.");
        msg.exec();
        return;
    }
}
