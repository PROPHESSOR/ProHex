#pragma once

#include <QUndoCommand>
#include <QObject>

#include "../DataStorage/datastorage.h"

/*
 * TODO:
 * Add selection delete support
 */

class UndoReplace : public QUndoCommand {
  public:
    UndoReplace(DataStorage *dataStorageObject, uint8_t oldData, uint8_t newData, int64_t position);

    void undo() override;
    void redo() override;

  private:
    uint8_t      m_oldData;
    uint8_t      m_newData;
    int64_t      m_position;
    DataStorage *m_pdata;
};
