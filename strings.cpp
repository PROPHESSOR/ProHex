#include "strings.h"
#include "ui_strings.h"

Strings::Strings(QWidget *parent) :
    QWidget(parent), ui(new Ui::Strings) {
    ui->setupUi(this);
}

Strings::~Strings() {
    delete ui;
}

void Strings::generateList(DataStorage *storage) {
    if(storage == nullptr) return;
    ui->listWidget->clear();

    QByteArray data = storage->getAllData();

    qDebug() << "Strings::generateList()";
    int32_t maxAddressLength = uint16_t(QString::number(data.length(), 16).length());
    qDebug() << "maxAddressLength =" << maxAddressLength;

    int64_t i = 0;

    while(i < data.length()) {
        QString string = "";
        QString address = QString("%1").arg(i, maxAddressLength, 16, QChar('0'));

        while(i < data.length()) {
            char tmp = data[int32_t(i)];

            if(tmp < 33) break;

            string += QChar(tmp);
            i++;
        }

        if(string.length() > 1) {
            ui->listWidget->addItem(address + " - " + string);
        }
        i++;
    }
}
