#pragma once

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QMessageBox>

#include "../Config/config.h"

namespace Ui {
class Assembler;
}

class Assembler : public QWidget {
    Q_OBJECT

public:
    explicit Assembler(QWidget *parent = nullptr, Config * = nullptr);
    ~Assembler();

    void filter(QString);

public slots:
    void on_innerValue(uint8_t);

private slots:
    void on_filterInput_textEdited(const QString &);

private:
    Ui::Assembler *ui;

    Config *m_config = nullptr;

    QVector<QString> list;
};
