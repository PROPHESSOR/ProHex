#pragma once

#include <QWidget>
#include <QResource>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

namespace Ui {
class Assembler;
}

class Assembler : public QWidget {
    Q_OBJECT

public:
    explicit Assembler(QWidget *parent = nullptr);
    ~Assembler();

    void filter(QString);

private slots:
    void on_filterInput_textEdited(const QString &);

private:
    Ui::Assembler *ui;

    QVector<QString> list;
};
