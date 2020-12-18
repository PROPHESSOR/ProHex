#include "assembler.h"
#include "ui_assembler.h"

Assembler::Assembler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Assembler) {
    ui->setupUi(this);

    QResource map(":/assembler/Assembler/x86.json");

    QJsonDocument json(QJsonDocument::fromJson(map.uncompressedData()));

    //qDebug() << "Json" << json.array()[0] << json.array().size();

    QJsonArray array = json.array();

    list.clear();

    for (auto ref : array) {
        QJsonObject object = ref.toObject();

        list.append(object["po"].toString() + " - " + object["mnemonic"].toString() + "   " + object["description"].toString());
    }

    filter("");
}

Assembler::~Assembler() {
    delete ui;
}

void Assembler::filter(QString filter) {
    ui->listWidget->clear();

    for (QString &line : list) {
        if (!filter.length() || line.contains(filter)) ui->listWidget->addItem(line);
    }
}

void Assembler::on_filterInput_textEdited(const QString &text) {
    filter(text);
}
