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
    explicit Finder(QWidget *parent = nullptr);
    ~Finder();
    Ui::Finder *ui;

  signals:
    void do_findnext();

  private slots:
    void on_lineEdit_2_textEdited(const QString &);

    void on_hexFindNext_clicked();

  private:
};
