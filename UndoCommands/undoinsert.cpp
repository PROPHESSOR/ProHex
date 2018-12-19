#include "undoinsert.h"

UndoInsert::UndoInsert(DataStorage *dataStorage, uint8_t data, int64_t position) {
    m_pdata     = dataStorage;
    m_data      = data;
    m_position  = position;
}

void UndoInsert::undo() {
    m_pdata->remove(m_position - 1);
    setText(QObject::tr("Remove insertion 0x%1 at %2").arg(QString::number(m_data, 16)).arg(m_position));
}

void UndoInsert::redo() {
    m_pdata->insert(m_position, m_data);
    setText(QObject::tr("Restore insertion 0x%1 at %2").arg(QString::number(m_data, 16)).arg(m_position));
}

