#pragma once

#include <QWidget>
#include <QRegExp>
#include <QDebug>

#include "ui_finder.h"

#define HEIGHT_SEARCH 301
#define HEIGHT_REPLACE 541

namespace Ui {
class Finder;
}

class Finder : public QWidget {
    Q_OBJECT

  public:
    explicit Finder(QWidget *parent = nullptr, QByteArray *searchArray = nullptr, QByteArray *replaceArray = nullptr);
    ~Finder();

    Ui::Finder *ui;

    enum ENUM_SEARCH_MODE {MODE_SEARCH = 0, MODE_REPLACE = 1};
    void setMode(ENUM_SEARCH_MODE mode);

  private slots:
    void on_asciiSearch_textEdited(const QString &);
    void on_hexSearch_textEdited(const QString &);

    void on_openReplace_clicked();

    void on_asciiReplace_textEdited(const QString &arg1);

    void on_hexReplace_textEdited(const QString &arg1);

  private:
    QByteArray *m_searchArray;
    QByteArray *m_replaceArray;

    ENUM_SEARCH_MODE m_mode;
};
