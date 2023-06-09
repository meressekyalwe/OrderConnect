#include "database.h"
#include <QDebug>
#include <QApplication>
#include <QSqlError>
#include <QSqlDriver>
#include <QtWidgets>

database::database(QObject *parent)
    : QObject{parent}
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("database.db");

    db.open();

    if(!db.isOpen())
    {
       closed =false;

       qDebug() << "Cannot open database:" << db.lastError();

    }
    else
    {
       qDebug() << "database opened";
    }

    getEmployeLoginData();
}

database::~database()
{
    qDebug() << "database closed";

    closed = true;

    db.close();
}

QSqlTableModel* database::createTableModel(const QString tableName)
{
    QSqlTableModel* model = new QSqlTableModel();
    model->setTable(tableName);
    model->select();
    return model;
}

QStringList database::getTableNames()
{
    QStringList tableNames;

    if (db.isValid())
    {
        QSqlQuery query(db);

        query.exec("SELECT name FROM sqlite_master WHERE type='table'");

        while (query.next())
        {
            tableNames.append(query.value(0).toString());

            qDebug() << tableNames;
        }
    }

    return tableNames;
}

void database::getEmployeLoginData()
{
    if (db.isValid())
    {
        QSqlQuery query(db);

        query.exec("SELECT * FROM Работники");

        while (query.next())
        {
            qDebug() << query.value(1).toString() << query.value(9).toString();

            LoginData LD{query.value(1).toString(), query.value(9).toString()};

            EmployeLoginData.push_back(LD);
        }
    }
}

bool database::login(LoginData LD)
{
    int count = 0;

    for (auto tmp : EmployeLoginData)
    {
        if (tmp == LD) count += 1;
    }

    qDebug() << "Count " << EmployeLoginData.size();

    return (count);
}


