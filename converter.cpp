#include "converter.h"
#include "ui_converter.h"
#include <QToolTip>
#include <QDebug>

Converter::Converter(QWidget *parent) :
    QWidget(parent), ui(new Ui::Converter) {
    ui->setupUi(this);
}

Converter::~Converter() {
    delete ui;
}

void Converter::calculate() {
    int64_t ipt = input.toLongLong();
    ui->DecInput->setText(input);
    ui->HexInput->setText(QString::number(ipt, 16).toUpper());
    ui->BinInput->setText(QString::number(ipt, 2));
    ui->CharInput->setText(ipt < 9 ? "" : QString(char(ipt)));

    ui->Int8LEInput->setText( QString::number(int8_t(ipt)));
    ui->Uint8LEInput->setText(QString::number(uint8_t(ipt)));
    ui->Int16LEInput->setText( QString::number(int16_t(ipt)));
    ui->Uint16LEInput->setText(QString::number(uint16_t(ipt)));
    ui->Int32LEInput->setText( QString::number(int32_t(ipt)));
    ui->Uint32LEInput->setText(QString::number(uint32_t(ipt)));
    ui->Int64LEInput->setText( QString::number(int64_t(ipt)));
    ui->Uint64LEInput->setText(QString::number(uint64_t(ipt)));
    ui->Float32LEInput->setText(QString::number(double_t(float_t(ipt)))); // TODO: Check
    ui->Float64LEInput->setText(QString::number(double_t(ipt)));

    // TODO: Big endian
    ui->Int8BEInput->setText( QString::number(int8_t(ipt)));
    ui->Uint8BEInput->setText(QString::number(uint8_t(ipt)));
    ui->Int16BEInput->setText( QString::number(int16_t(ipt)));
    ui->Uint16BEInput->setText(QString::number(uint16_t(ipt)));
    ui->Int32BEInput->setText( QString::number(int32_t(ipt)));
    ui->Uint32BEInput->setText(QString::number(uint32_t(ipt)));
    ui->Int64BEInput->setText( QString::number(int64_t(ipt)));
    ui->Uint64BEInput->setText(QString::number(uint64_t(ipt)));
    ui->Float32BEInput->setText(QString::number(double_t(float_t(ipt))));
    ui->Float64BEInput->setText(QString::number(double_t(ipt)));
}

void Converter::calculate(int64_t value) {
    input = QString::number(value);
    calculate();
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
