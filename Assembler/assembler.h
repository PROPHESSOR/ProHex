#pragma once

#include <QWidget>

namespace Ui {
class Assembler;
}

class Assembler : public QWidget {
    Q_OBJECT

public:
    explicit Assembler(QWidget *parent = nullptr);
    ~Assembler();

private:
    Ui::Assembler *ui;
};
