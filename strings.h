#pragma once

#include <QWidget>
#include <QByteArray>

#include "datastorage.h"

namespace Ui {
class Strings;
}

class Strings : public QWidget {
    Q_OBJECT

  public:
    explicit Strings(QWidget *parent = nullptr);
    ~Strings();

    void generateList(DataStorage *);

  private:
    Ui::Strings *ui;
};
