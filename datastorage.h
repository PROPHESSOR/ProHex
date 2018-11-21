#pragma once

#include <QByteArray>
#include <QFile>

class DataStorage {
  public:
    virtual ~DataStorage() {}
    virtual QByteArray getData(std::size_t position, std::size_t length) = 0;
    virtual std::size_t size() = 0;
};


class DataStorageArray : public DataStorage {
  public:
    DataStorageArray(const QByteArray &arr);
    virtual QByteArray getData(std::size_t position, std::size_t length);
    virtual std::size_t size();
  private:
    QByteArray    m_data;
};

class DataStorageFile : public DataStorage {
  public:
    DataStorageFile(const QString &fileName);
    virtual QByteArray getData(std::size_t position, std::size_t length);
    virtual std::size_t size();
  private:
    QFile      m_file;
};
