#include "undoxor.h"

UndoXOR::UndoXOR(DataStorage *dataStorage,  const QString &key) {
    m_pdata     = dataStorage;
    m_key       = key;
}

void UndoXOR::undo() {
    m_pdata->XOR(m_key);
    setText(QObject::tr("Undo XOR"));
}

void UndoXOR::redo() {
    m_pdata->XOR(m_key);
    setText(QObject::tr("Redo XOR"));
}

