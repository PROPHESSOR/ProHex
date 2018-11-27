#pragma once

// https://github.com/virinext/QHexView

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
#include "config.h"

#include <stdexcept>

class QHexView: public QAbstractScrollArea {
  public:
    QHexView(QWidget *parent = nullptr, DataStorage *data = nullptr, Config *config = nullptr, QStatusBar *statusBar = nullptr);
    ~QHexView();

    void recalcView();

  public slots:
    void setData(DataStorage *pData);
    void clear();
    void showFromOffset(uint64_t offset);

  protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
  private:
    uint16_t              m_posHex;
    uint16_t              m_hexWidth;
    uint16_t              m_addressWidth;
    uint16_t              m_asciiWidth;
    uint16_t              m_bytesPerLine;
    uint16_t              m_posAscii;
    uint16_t              m_charWidth;
    uint16_t              m_charHeight;


    uint64_t              m_selectBegin;
    uint64_t              m_selectEnd;
    uint64_t              m_selectInit;
    uint64_t              m_cursorPos;

    DataStorage          *m_pdata;
    Config               *m_config;
    QStatusBar           *m_statusBar;


    QSize fullSize() const;
    void resetSelection();
    void resetSelection(int pos);
    void setSelection(int pos);
    void ensureVisible();
    void setCursorPos(int64_t pos);
    uint64_t getCursorPos(const QPoint &position);
};
