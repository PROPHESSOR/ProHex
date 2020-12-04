#include "assembler.h"
#include "ui_assembler.h"

Assembler::Assembler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Assembler) {
    ui->setupUi(this);
}

Assembler::~Assembler() {
    delete ui;
}
