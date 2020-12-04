#pragma once

#include <QWidget>
#include <QString>
#include <QtEndian>

namespace Ui {
class Converter;
}

union longparser_t {
    int8_t int8;
    uint8_t uint8;
    int16_t int16;
    uint16_t uint16;
    int32_t int32;
    uint32_t uint32;
    int64_t int64;
    uint64_t uint64;
    float f;
    double d;
};

class Converter : public QWidget {
    Q_OBJECT

  public:
    explicit Converter(QWidget *parent = nullptr);
    ~Converter();

    void calculate();
    void calculate(int64_t value);

  public slots:
    void on_innerValue(int64_t value);

  private slots:
    void on_DecInput_textEdited(const QString &);

    void on_HexInput_textEdited(const QString &);

    void on_BinInput_textEdited(const QString &);

    void on_CharInput_textEdited(const QString &);

    void on_hexCheckbox_stateChanged(int arg1);

  private:
    Ui::Converter *ui;

    QString input; // Decimal value
    bool    hexMode;
};
