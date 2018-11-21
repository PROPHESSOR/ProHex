#include "datastorage.h"

DataStorageArray::DataStorageArray(const QByteArray &arr) {
    m_data = arr;
}

QByteArray DataStorageArray::getData(std::size_t position, std::size_t length) {
    return m_data.mid(position, length);
}


std::size_t DataStorageArray::size() {
    return m_data.count();
}


DataStorageFile::DataStorageFile(const QString &fileName): m_file(fileName) {
    m_file.open(QIODevice::ReadOnly);
    if(!m_file.isOpen())
        throw std::runtime_error(std::string("Failed to open file `") + fileName.toStdString() + "`");
}

QByteArray DataStorageFile::getData(std::size_t position, std::size_t length) {
    m_file.seek(position);
    return m_file.read(length);
}


std::size_t DataStorageFile::size() {
    return m_file.size();
}
