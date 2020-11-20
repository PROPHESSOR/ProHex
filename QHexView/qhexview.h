#pragma once

// Based on: https://github.com/virinext/QHexView

#include <stdexcept>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QSize>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QStatusBar>
#include <QKeySequence>
#include <QUndoStack>
#include <QUndoCommand>

#include "constants.h"
#include "../DataStorage/datastorage.h"
#include "../Config/config.h"
#include "../UndoCommands/undocommands.h"

class QHexView : public QAbstractScrollArea {
    Q_OBJECT

  public:
    QHexView(QWidget *parent = nullptr, DataStorage *data = nullptr, Config *config = nullptr, QStatusBar *statusBar = nullptr, QUndoStack *undoStack = nullptr);
    ~QHexView();

    void        recalcView();
    uint64_t    getCursorPosition();
    void setData(DataStorage *pData);
    void gotoOffset(int64_t offset);
    void select(int64_t start, int64_t end);
    void update();

  public slots:
    void clear();

  signals:
    void valueChanged(uint8_t value);
    void doubleClick(void);

  protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
  private:
    uint16_t              m_posHex;
    uint16_t              m_hexWidth;
    uint16_t              m_addressWidth;
    uint16_t              m_maxAddressLength;
    uint16_t              m_asciiWidth;
    uint16_t              m_bytesPerLine;
    uint16_t              m_posAscii;
    uint16_t              m_charWidth;
    uint16_t              m_charHeight;

    uint64_t              m_selectBegin;
    uint64_t              m_selectEnd;
    uint64_t              m_selectInit;
    uint64_t              m_cursorPos;

    DataStorage          *m_data;
    Config               *m_config;
    QStatusBar           *m_statusBar;
    QUndoStack           *m_undoStack;

    enum ENUM_MODE   {
        MODE_READONLY       = 0,
        MODE_WRITE_INSERT   = 1,
        MODE_WRITE_REPLACE  = 2
    } m_mode;

    enum ENUM_WINDOW {
        WINDOW_HEX          = 0,
        WINDOW_ASCII        = 1
    } m_window;


    QSize fullSize() const;
    void resetSelection();
    void resetSelection(int pos);
    void setSelection(int pos);
    void ensureVisible();
    void setCursorPos(int64_t pos);
    void statusBarUpdate();
    void inputSymbol(QChar symbol);
    uint64_t getCursorPos(const QPoint &position);
};
