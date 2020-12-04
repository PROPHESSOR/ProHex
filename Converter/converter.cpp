#include "converter.h"
#include "ui_converter.h"
#include <QToolTip>
#include <QDebug>

Converter::Converter(QWidget *parent) :
    QWidget(parent), ui(new Ui::Converter) {
    ui->setupUi(this);

    hexMode = false;
    input = "0";
}

Converter::~Converter() {
    delete ui;
}

void Converter::calculate() {
    int64_t ipt = input.toLongLong();
    uint8_t base = hexMode ? 16 : 10;

    longparser_t parser;

    parser.int64 = ipt;

    ui->DecInput->setText(input);
    ui->HexInput->setText(QString::number(ipt, 16).toUpper());
    ui->BinInput->setText(QString::number(ipt, 2));
    ui->CharInput->setText(ipt < 9 ? "" : QString(parser.uint8));

    ui->Int8LEInput->setText(  QString::number(parser.int8,     base));
    ui->Uint8LEInput->setText(  QString::number(parser.uint8,   base));
    ui->Int16LEInput->setText(  QString::number(parser.int16,   base));
    ui->Uint16LEInput->setText(QString::number(parser.uint16,   base));
    ui->Int32LEInput->setText(  QString::number(parser.int32,   base));
    ui->Uint32LEInput->setText(QString::number(parser.uint32,   base));
    ui->Int64LEInput->setText(  QString::number(parser.int64,   base));
    ui->Uint64LEInput->setText(QString::number(parser.uint64,   base));
    ui->Float32LEInput->setText(QString::number(parser.f));
    ui->Float64LEInput->setText(QString::number(parser.d));
}

void Converter::calculate(int64_t value) {
    input = QString::number(value);
    calculate();
}

void Converter::on_innerValue(int64_t value) {
    input = QString::number(value);
    if(!isHidden()) {
        qDebug() << "Converter::on_innerValue(" << value << ")";
        calculate();
    }
}

void Converter::on_DecInput_textEdited(const QString &value) {
    qDebug() << "Converter::on_DecInput_textEdited(" << value << ")";
    input = value;
    calculate();
}

void Converter::on_HexInput_textEdited(const QString &value) {
    qDebug() << "Converter::on_HexInput_textEdited(" << value << ")";
    input = QString::number(value.toLongLong(nullptr, 16), 10);
    calculate();
}

void Converter::on_BinInput_textEdited(const QString &value) {
    qDebug() << "Converter::on_BinInput_textEdited(" << value << ")";
    input = QString::number(value.toLongLong(nullptr, 2), 10);
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

void Converter::on_hexCheckbox_stateChanged(int mode) {
    hexMode = bool(mode);
    calculate();
}
