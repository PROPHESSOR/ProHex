#include "undodelete.h"

UndoDelete::UndoDelete(DataStorage *dataStorage, uint8_t data, int64_t position) {
    m_pdata     = dataStorage;
    m_data      = data;
    m_position  = position;
}

void UndoDelete::undo() {
    m_pdata->insert(m_position, m_data);
    setText(QObject::tr("Restore 0x%1 at %2").arg(QString::number(m_data)).arg(m_position));
}

void UndoDelete::redo() {
    m_pdata->remove(m_position);
    setText(QObject::tr("Remove 0x%1 at %2").arg(QString::number(m_data)).arg(m_position));
}

