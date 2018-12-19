#pragma once

#include <QByteArray>
#include <QDebug>

class DataStorage {
  public:
    DataStorage(const QByteArray &data, QString associatedFilePath = "");
    QByteArray  getData(int32_t position, int32_t length);
    void        setData(QByteArray &newData);
    void        replace(int32_t position, char data);
    void        insert(int32_t position, char data);
    void        remove(int32_t position);
    int64_t     size();
    char        at(int64_t position);
    QByteArray  getAllData();
    int64_t     find(const QByteArray *arrayToFind, int64_t startPosition, bool incremental = true);

    char        operator[](int64_t position) const;

    bool        isFileAssociated();
    QString     getAssociatedFile();
    void        setAssociatedFile(const QString &newAssociatedFilePath);
  private:
    QByteArray  m_data;
    QString     m_file;
};
