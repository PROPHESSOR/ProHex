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
    qDebug() << "Strings::generateList()";
    if(storage == nullptr) return;
    ui->listWidget->clear();

    m_list.clear();

    QByteArray data = storage->getAllData();

    int32_t maxAddressLength = uint16_t(QString::number(data.length(), 16).length());

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
            m_list.append(address + " - " + string);
        }
        i++;
    }

    if(m_list.length() < LARGELIST) {
        ui->filterInput->setStyleSheet("background-color: none;");
    } else {
        ui->filterInput->setStyleSheet("background-color: #afafaf;");
    }

    filter("");
}

void Strings::on_filterInput_textEdited(const QString &str) {
    if(ui->listWidget->count() < LARGELIST) filter(str);
}

void Strings::filter(const QString &str) {
    qDebug() << "Strings::filter(" << str << ")";

    ui->listWidget->clear();

    this->setCursor(Qt::WaitCursor);

    for(QString &tmp : m_list) {
        if(!str.length() || tmp.contains(str)) ui->listWidget->addItem(tmp);
    }

    this->setCursor(Qt::ArrowCursor);
}

void Strings::on_filterInput_returnPressed() {
    filter(ui->filterInput->text());
}
