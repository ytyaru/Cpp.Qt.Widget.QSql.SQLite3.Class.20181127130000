#include "memodb.h"

MemoDb::MemoDb()
{
    this->Open();
}

MemoDb::~MemoDb()
{
    this->Close();
}

void MemoDb::Open(QString path)
{
    this->dbPath = QDir(path).filePath("memo.sqlite3");
    QSqlDatabase::removeDatabase("MemoDb");
//    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db = QSqlDatabase::addDatabase("QSQLITE", "MemoDb");
    this->db.setDatabaseName(this->dbPath);
    this->db.open();
    this->CreateTable();
}
void MemoDb::Close()
{
//    if (this->db.isOpen()) { this->db.close(); }
    if (this->db.isOpen()) { qDebug() << "Close() " + this->db.connectionName(); this->db.close(); QSqlDatabase::removeDatabase(this->db.connectionName()); }
}

bool MemoDb::IsExsistTable()
{
    QSqlQuery query(this->db);
    query.exec("select count(*) from sqlite_master where type='table' and name='Memo'");
    int count = 0;
    while (query.next()) { count = query.value(0).toInt(); }
    if (0 < count) { return true; }
    else { return false; }
}

void MemoDb::CreateTable()
{
    if (!this->IsExsistTable()) {
        QSqlQuery query(this->db);
        query.exec("create table Memo(id INTEGER PRIMARY KEY AUTOINCREMENT, Memo text, DateTime text)");
    }
}

void MemoDb::Write(QStringList record)
{
    QSqlQuery query(this->db);
    query.prepare("insert into Memo (Memo, DateTime) values (?, ?)");
    for (int i = 0; i < record.size(); i++) {
        query.bindValue(i, record[i]);
    }
    query.exec();
    qDebug() << record;
}
