#include "datastorage.h"

// TODO: Добавить setData

DataStorage::DataStorage(const QByteArray &arr) {
    m_data = arr;
}

QByteArray DataStorage::getData(int32_t position, int32_t length) {
    return m_data.mid(position, length);
}

void DataStorage::replace(int32_t position, char data) {
    char tdata[] = {data};

    m_data.replace(position, 1, tdata);
}

void DataStorage::insert(int32_t position, char data) {
    char tdata[] = {data};

    m_data.insert(position, tdata, 1);
}

void DataStorage::remove(int32_t position) {
    m_data.remove(position, 1);
}

int64_t DataStorage::size() {
    return m_data.count();
}
