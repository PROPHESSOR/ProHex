#include "assembler.h"
#include "ui_assembler.h"

Assembler::Assembler(QWidget *parent, Config *config) :
    QWidget(parent), ui(new Ui::Assembler), m_config(config) {
    ui->setupUi(this);

    QString specs = config->getAssemblerSpecs();
    QString specsFile = QDir("./assemblers/").absoluteFilePath(specs);

    qDebug() << "Assembler::loadSpec" << specs << specsFile;

    setWindowTitle("Assembler (" + specs + ")");

    list.clear();

    QFile map(specsFile);
    if (!map.open(QFile::ReadOnly)) {
        QMessageBox::information(this, tr("Specification error!"), tr("Can't open assembler specification file!\nTry to change spec file in Preferences."));
        setWindowTitle(tr("Assembler (Can't load specs)"));
    } else {
        QJsonDocument json(QJsonDocument::fromJson(map.readAll()));

        QJsonArray array = json.array();

        for (auto ref : array) {
            QJsonObject object = ref.toObject();

            list.append(object["po"].toString() + " - " + object["mnemonic"].toString() + "   " + object["description"].toString());
        }
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
