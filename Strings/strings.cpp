#include "strings.h"
#include "ui_strings.h"

Strings::Strings(QWidget *parent) :
    QWidget(parent), ui(new Ui::Strings) {
    ui->setupUi(this);
    minStringLength = ui->stringLength->value();
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
            char tmp = data.at(i);

            if(tmp < 33) break; // Unprintable separator

            string += QChar(tmp);
            i++;
        }

        m_list.append(address + STRINGS_SEPARATOR + string);
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
        bool noFilter = !str.length();
        bool isInMinStringLength = tmp.split(STRINGS_SEPARATOR)[1].length() >= minStringLength;
        if((noFilter || tmp.contains(str)) && isInMinStringLength) ui->listWidget->addItem(tmp);
    }

    this->setCursor(Qt::ArrowCursor);

    ui->bottomLabel->setText(QString::asprintf("%d lines", ui->listWidget->count()));
}

void Strings::on_filterInput_returnPressed() {
    filter(ui->filterInput->text());
}

void Strings::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    int64_t offset = current->text().split(STRINGS_SEPARATOR)[0].toLongLong(nullptr, 16);
    int64_t prevOffset = previous && previous->text().split(STRINGS_SEPARATOR)[0].toLongLong(nullptr, 16);
    qDebug() << "currentItemChanged(" << prevOffset << "->" << offset;
    emit offsetChanged(offset);
}

void Strings::on_stringLength_valueChanged(int value) {
    qDebug() << "on_stringLength_valueChanged(" << value << ")";
    minStringLength = value;

    if (m_list.length() < LARGELIST) filter(ui->filterInput->text());
}
