/* TODO
 * Добавление массива поиска (лучше QByteArray) в поле MainWindow
 * Реализовать ASCII поиск
 * Реализовать HEX поиск
 */

#include "finder.h"

Finder::Finder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Finder) {
    ui->setupUi(this);
}

Finder::~Finder() {
    delete ui;
}

void Finder::on_lineEdit_2_textEdited(const QString &string) {
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
        searchArray.push_back(char(str.toInt()));
    }

    ui->hexReadLabel->setText(tmp);


}

void Finder::on_hexFindNext_clicked() {

}
