#include "finder.h"
#include "ui_finder.h"

Finder::Finder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Finder) {
    ui->setupUi(this);
}

Finder::~Finder() {
    delete ui;
}
