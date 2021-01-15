#include "assembler.h"
#include "ui_assembler.h"

Assembler::Assembler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Assembler) {
    ui->setupUi(this);

    QFile map(QDir("./assemblers/x86.json").absolutePath());
    map.open(QFile::ReadOnly);

    QJsonDocument json(QJsonDocument::fromJson(map.readAll()));

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

void Assembler::on_innerValue(uint8_t value) {
    QString filterValue = QString::number(value, 16);

    if (filterValue.length() == 1) filterValue = "0" + filterValue;

    ui->filterInput->setText(filterValue);
    filter(filterValue);
}

void Assembler::on_filterInput_textEdited(const QString &text) {
    filter(text);
}
