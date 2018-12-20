#include "finder.h"

Finder::Finder(QWidget *parent, QByteArray *searchArray, QByteArray *replaceArray) :
    QWidget(parent), ui(new Ui::Finder), m_searchArray(searchArray), m_replaceArray(replaceArray) {
    ui->setupUi(this);

    setMode(MODE_SEARCH);
    setMinimumHeight(HEIGHT_SEARCH);
    setMaximumHeight(HEIGHT_REPLACE);
}

Finder::~Finder() {
    delete ui;
}

void Finder::setMode(Finder::ENUM_SEARCH_MODE mode) {
    m_mode = mode;

    switch(mode) {
        case MODE_SEARCH:
            ui->replaceLayout_2->hide();
            ui->openReplace->setText(tr("Show replace"));
            resize(width(), HEIGHT_SEARCH);
            break;
        case MODE_REPLACE:
            ui->replaceLayout_2->show();
            ui->openReplace->setText(tr("Hide replace"));
            resize(width(), HEIGHT_REPLACE);
            break;
    }
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

void Finder::on_openReplace_clicked() {
    qDebug() << "Finder::on_openReplace_clicked" << m_mode;

    switch(m_mode) {
        case MODE_SEARCH:
            setMode(MODE_REPLACE);
            break;
        case MODE_REPLACE:
            setMode(MODE_SEARCH);
            break;
    }
}

void Finder::on_asciiReplace_textEdited(const QString &string) {
    m_replaceArray->clear();
    m_replaceArray->insert(0, QByteArray::fromStdString(string.toStdString()));

    QString output = "";

    for(QChar chr : string) {
        output += QString::number(chr.toLatin1(), 16);
        output += ' ';
    }

    ui->hexReadReplace->setText(output);
}

void Finder::on_hexReplace_textEdited(const QString &string) {
    QRegExp rx("([0-9A-Fa-f][0-9A-Fa-f])\\s?");
    QStringList list;

    int16_t pos = 0;

    while((pos = rx.indexIn(string, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }

    qDebug() << list;

    QByteArray replaceArray;

    QString tmp;

    for(QString str : list) {
        tmp += str;
        tmp += " ";
        replaceArray.push_back(str.toInt(nullptr, 16));
    }

    m_replaceArray->clear();
    m_replaceArray->insert(0, replaceArray);

    ui->hexReadReplace->setText(tmp);
}
