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

  private slots:
    void on_DecInput_textEdited(const QString &arg1);

    void on_HexInput_textEdited(const QString &arg1);

    void on_BinInput_textEdited(const QString &arg1);

    void on_CharInput_textEdited(const QString &arg1);

  private:
    Ui::Converter *ui;

    QString input; // Decimal value
};
