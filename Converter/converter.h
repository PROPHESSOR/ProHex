#pragma once

#include <QWidget>
#include <QString>
#include <QtEndian>

namespace Ui {
class Converter;
}

class Converter : public QWidget {
    Q_OBJECT

  public:
    explicit Converter(QWidget *parent = nullptr);
    ~Converter();

    void calculate();
    void calculate(int64_t value);

  public slots:
    void on_innerValue(uint8_t value);

  private slots:
    void on_DecInput_textEdited(const QString &);

    void on_HexInput_textEdited(const QString &);

    void on_BinInput_textEdited(const QString &);

    void on_CharInput_textEdited(const QString &);

  private:
    Ui::Converter *ui;

    QString input; // Decimal value
};
