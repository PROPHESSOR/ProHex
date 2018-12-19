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

    if(ui->listWidget->count() < LARGELIST) {
        ui->searchInput->setStyleSheet("background-color: none;");
    } else {
        ui->searchInput->setStyleSheet("background-color: #afafaf;");
    }
}

void Strings::on_searchInput_textEdited(const QString &str) {
    m_searchChanged = true;
    if(ui->listWidget->count() < LARGELIST) search(str);
}

void Strings::clearSelection() {
    ui->listWidget->clearSelection();
}

void Strings::search(const QString &str) {
    m_searchChanged = false;
    clearSelection();

    if(str.isEmpty()) return;

    this->setCursor(Qt::WaitCursor);

    m_searchlist = ui->listWidget->findItems(str, Qt::MatchCaseSensitive | Qt::MatchContains);

    if(m_searchlist.length()) {
        m_selectedIndex = 0;

        for(QListWidgetItem *tmp : m_searchlist) {
            tmp->setSelected(true);
        }

        goToItem(-1);
    }

    this->setCursor(Qt::ArrowCursor);
}

void Strings::goToItem(int32_t idx) {
    if(idx < 0) idx = m_selectedIndex;
    if(idx >= ui->listWidget->selectedItems().length()) idx %= ui->listWidget->selectedItems().length();

    ui->listWidget->scrollToItem(ui->listWidget->selectedItems().at(idx));
    ui->listWidget->setCurrentItem(ui->listWidget->selectedItems().at(idx));
}

void Strings::on_searchInput_returnPressed() {
    if(m_searchChanged)
        search(ui->searchInput->text());
    else
        goToItem(m_selectedIndex++);
}
