#include "converter.h"
#include "ui_converter.h"
#include <QToolTip>
#include <QDebug>

Converter::Converter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::converter) {
    ui->setupUi(this);
}

Converter::~Converter() {
    delete ui;
}

void Converter::calculate() {
    int ipt = input.toInt();
    ui->DecInput->setText(input);
    ui->HexInput->setText(QString::number(ipt, 16).toUpper());
    ui->BinInput->setText(QString::number(ipt, 2));
    ui->CharInput->setText(ipt < 9 ? "" : QString((char) ipt));
}

void Converter::on_DecInput_textEdited(const QString &value) {
    qDebug() << "Converter::on_DecInput_textEdited(" << value << ")";
    input = value;
    calculate();
}

void Converter::on_HexInput_textEdited(const QString &value) {
    qDebug() << "Converter::on_HexInput_textEdited(" << value << ")";
    bool ok;
    input = QString::number(value.toUInt(&ok, 16), 10);
    calculate();
}

void Converter::on_HexInput_textChanged(const QString &arg1) {
    // FIXME: Remove
}

void Converter::on_BinInput_textEdited(const QString &value) {
    qDebug() << "Converter::on_BinInput_textEdited(" << value << ")";
    bool ok;
    input = QString::number(value.toUInt(&ok, 2), 10);
    calculate();
}

void Converter::on_CharInput_textEdited(const QString &value) {
    qDebug() << "Converter::on_CharInput_textEdited(" << value << ")";
    if(value.isEmpty()) {
        input = QString::number(0);
    } else {
        input = QString::number(value.at(0).toLatin1(), 10);
    }
    calculate();
}
