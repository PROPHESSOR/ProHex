#pragma once

#include <QByteArray>

class DataStorage {
  public:
    DataStorage(const QByteArray &arr);
    QByteArray  getData(int32_t position, int32_t length);
    void        replace(int32_t position, char data);
    void        insert(int32_t position, char data);
    void        remove(int32_t position);
    int64_t     size();
  private:
    QByteArray          m_data;
};
