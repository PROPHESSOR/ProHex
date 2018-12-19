#include "finder.h"

Finder::Finder(QWidget *parent, QByteArray *searchArray) :
    QWidget(parent),
    ui(new Ui::Finder) {
    ui->setupUi(this);

    m_searchArray = searchArray;
}

Finder::~Finder() {
    delete ui;
}

void Finder::on_hexSearch_textEdited(const QString &string) {
    QRegExp rx("([0-9A-Fa-f][0-9A-Fa-f])\\s?");
    QStringList list;

    int16_t pos = 0;

    while((pos = rx.indexIn(string, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }

    qDebug() << list;

    QByteArray searchArray;

    QString tmp;

    for(QString str : list) {
        tmp += str;
        tmp += " ";
        searchArray.push_back(str.toInt(nullptr, 16));
    }

    m_searchArray->clear();
    m_searchArray->insert(0, searchArray);

    ui->hexReadLabel->setText(tmp);
}

void Finder::on_asciiSearch_textEdited(const QString &string) {
    m_searchArray->clear();
    m_searchArray->insert(0, QByteArray::fromStdString(string.toStdString()));

    QString output = "";

    for(QChar chr : string) {
        output += QString::number(chr.toLatin1(), 16);
        output += ' ';
    }

    ui->hexReadLabel->setText(output);
}
