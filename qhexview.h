#pragma once

// Based on: https://github.com/virinext/QHexView

#include "constants.h"
#include "datastorage.h"

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
#include "config.h"

#include <stdexcept>

class QHexView: public QAbstractScrollArea {
  public:
    QHexView(QWidget *parent = nullptr, DataStorage *data = nullptr, Config *config = nullptr, QStatusBar *statusBar = nullptr);
    ~QHexView();

    void        recalcView();
    uint64_t    getCursorPosition();

  public slots:
    void setData(DataStorage *pData);
    void clear();
    void showFromOffset(int64_t offset);

  protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
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
