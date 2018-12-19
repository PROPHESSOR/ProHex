#include "undoreplace.h"

UndoReplace::UndoReplace(DataStorage *dataStorage, uint8_t oldData, uint8_t newData, int64_t position) {
    m_pdata     = dataStorage;
    m_oldData   = oldData;
    m_newData   = newData;
    m_position  = position;
}

void UndoReplace::undo() {
    m_pdata->replace(m_position, m_oldData);
    setText(QObject::tr("Undo replacement 0x%1 -> 0x%2 at %3").arg(QString::number(m_oldData, 16)).arg(QString::number(m_newData, 16)).arg(m_position));
}

void UndoReplace::redo() {
    m_pdata->replace(m_position, m_newData);
    setText(QObject::tr("Restore replacement 0x%1 -> 0x%2 at %3").arg(QString::number(m_oldData, 16)).arg(QString::number(m_newData, 16)).arg(m_position));
}

