#pragma once

#include <QByteArray>
#include <QDebug>

class DataStorage {
  public:
    DataStorage(const QByteArray &, QString = "");
    QByteArray  getData(int32_t position, int32_t length);
    void        setData(QByteArray &);
    void        replace(int32_t position, char data);
    void        insert(int32_t position, char data);
    void        remove(int32_t position);
    int64_t     size();
    QByteArray  getAllData();

    bool        isFileAssociated();
    QString     getAssociatedFile();
    void        setAssociatedFile(const QString &);
  private:
    QByteArray          m_data;
    QString             m_file;
};
