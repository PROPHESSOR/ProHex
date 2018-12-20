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

int64_t DataStorage::find(const QByteArray *array, int64_t position, bool incremental) {
    if(m_data == nullptr) return -1;
    if(position > (m_data.size() - array->size())) return -1; // FIXME: Can it be removed?

    for(int64_t i = position; (incremental && i < m_data.size() - array->size()) || (!incremental && i >= 0); incremental ? i++ : i--) {
        bool found = true;

        for(int32_t j = 0; j < array->size(); j++) {
            if(array->at(j) != m_data.at(i + j)) {
                found = false;
                break;
            }
        }

        if(found) {
            qDebug() << "Found at position:" << i * 2;

            return i;
        }
    }

    return -1;
}

bool DataStorage::findReplace(const QByteArray *arrayToFind, const QByteArray *arrayToReplace, int64_t position) {
    qDebug() << "DataStorage::findReplace<one>";
    if(!(arrayToFind->length() && arrayToReplace->length())) return false;
    if((position + arrayToFind->length()) >= m_data.count()) return false;
    if(position < 0)                                         return false;

    if(find(arrayToFind, position, true) != position)        return false;

    m_data.replace(position, arrayToFind->length(), *arrayToReplace);

    return true;
}

bool DataStorage::findReplace(const QByteArray *arrayToFind, const QByteArray *arrayToReplace) {
    qDebug() << "DataStorage::findReplace<all>";
    if(!(arrayToFind->length() && arrayToReplace->length())) return false;

    m_data.replace(*arrayToFind, *arrayToReplace);

    return true;
}

char DataStorage::at(int64_t index) {
    return m_data.at(index);
}

char DataStorage::operator[](int64_t index) const {
    return m_data.at(index);
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
