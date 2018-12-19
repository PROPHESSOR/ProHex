#pragma once

#include <QUndoCommand>
#include <QObject>

#include "../DataStorage/datastorage.h"

/*
 * TODO:
 * Add selection delete support
 */

class UndoInsert : public QUndoCommand {
  public:
    UndoInsert(DataStorage *dataStorageObject, uint8_t removedData, int64_t position);

    void undo() override;
    void redo() override;

  private:
    uint8_t      m_data;
    int64_t      m_position;
    DataStorage *m_pdata;
};
