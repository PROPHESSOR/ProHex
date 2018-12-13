#include "datastorage.h"

// TODO: Добавить setData

DataStorage::DataStorage(const QByteArray &data, QString file) {
    m_data = data;
    m_file = file;
}

QByteArray DataStorage::getData(int32_t position, int32_t length) {
    return m_data.mid(position, length);
}

void DataStorage::setData(QByteArray &data) {
    m_data = data;
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

QByteArray DataStorage::getAllData() {
    return m_data;
}

bool DataStorage::isFileAssociated() {
    return !m_file.isEmpty();
}

QString DataStorage::getAssociatedFile() {
    return m_file;
}

void DataStorage::setAssociatedFile(const QString &file) {
    m_file = file;
}
