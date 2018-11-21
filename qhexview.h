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

#include <stdexcept>

class QHexView: public QAbstractScrollArea {
  public:
    QHexView(QWidget *parent = nullptr, DataStorage *data = nullptr);
    ~QHexView();

  public slots:
    void setData(DataStorage *pData);
    void clear();
    void showFromOffset(std::size_t offset);

  protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
  private:
    DataStorage          *m_pdata;
    int                   m_posHex;
    int                   m_hexWidth;
    int                   m_addressWidth;
    int                   m_asciiWidth;
    int                   m_bytesPerLine;
    int                   m_posAscii;
    uint16_t              m_charWidth;
    uint16_t              m_charHeight;


    std::size_t           m_selectBegin;
    std::size_t           m_selectEnd;
    std::size_t           m_selectInit;
    std::size_t           m_cursorPos;


    QSize fullSize() const;
    void resetSelection();
    void resetSelection(int pos);
    void setSelection(int pos);
    void ensureVisible();
    void setCursorPos(std::size_t pos);
    std::size_t cursorPos(const QPoint &position);
};
