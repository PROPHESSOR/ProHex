#pragma once

#include <QWidget>

namespace Ui {
class Finder;
}

class Finder : public QWidget {
    Q_OBJECT

  public:
    explicit Finder(QWidget *parent = 0);
    ~Finder();

  private:
    Ui::Finder *ui;
};
