#pragma once

#include <QWidget>
#include <QRegExp>
#include <QDebug>

#include "ui_finder.h"

namespace Ui {
class Finder;
}

class Finder : public QWidget {
    Q_OBJECT

  public:
    explicit Finder(QWidget *parent = nullptr, QByteArray * = nullptr);
    ~Finder();
    Ui::Finder *ui;

  private slots:
    void on_asciiSearch_textEdited(const QString &);
    void on_hexSearch_textEdited(const QString &);

  private:
    QByteArray *m_searchArray;
};
