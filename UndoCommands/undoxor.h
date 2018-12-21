#pragma once

#include <QUndoCommand>
#include <QObject>

#include "../DataStorage/datastorage.h"

class UndoXOR : public QUndoCommand {
  public:
    UndoXOR(DataStorage *dataStorageObject, const QString &key);

    void undo() override;
    void redo() override;

  private:
    QString      m_key;
    DataStorage *m_pdata;
};
