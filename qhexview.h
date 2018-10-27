#pragma once

// https://github.com/virinext/QHexView

#include "constants.h"

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
    class DataStorage {
      public:
        virtual ~DataStorage() {}
        virtual QByteArray getData(std::size_t position, std::size_t length) = 0;
        virtual std::size_t size() = 0;
    };


    class DataStorageArray: public DataStorage {
      public:
        DataStorageArray(const QByteArray &arr);
        virtual QByteArray getData(std::size_t position, std::size_t length);
        virtual std::size_t size();
      private:
        QByteArray    m_data;
    };

    class DataStorageFile: public DataStorage {
      public:
        DataStorageFile(const QString &fileName);
        virtual QByteArray getData(std::size_t position, std::size_t length);
        virtual std::size_t size();
      private:
        QFile      m_file;
    };



    QHexView(QWidget *parent = 0);
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
    int                   m_posAddr;
    int                   m_posHex;
    int                   m_posAscii;
    unsigned short        m_charWidth;
    unsigned short        m_charHeight;


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
