#include "asciitable.h"
#include "ui_asciitable.h"

AsciiTable::AsciiTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AsciiTable) {
    ui->setupUi(this);

    m_charWidth     = uint16_t(fontMetrics().horizontalAdvance(QLatin1Char('9')));
    m_charHeight    = uint16_t(fontMetrics().height());

    ui->tableWidget->setRowCount(ASCII_MATRIX_SIZE);
    ui->tableWidget->setColumnCount(ASCII_MATRIX_SIZE);

    for(short row = 0; row < ASCII_MATRIX_SIZE; row++) {
        QTableWidgetItem *header = new QTableWidgetItem(QString::number(row, 16));

        ui->tableWidget->setHorizontalHeaderItem(row, header);
        ui->tableWidget->setVerticalHeaderItem(row, header);

        for(short column = 0; column < ASCII_MATRIX_SIZE; column++) {
            QTableWidgetItem *tmp = new QTableWidgetItem(QChar(row * ASCII_MATRIX_SIZE + column));

            tmp->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget->setItem(row, column, tmp);
        }
    }

}

AsciiTable::~AsciiTable() {
    delete ui;
}

void AsciiTable::paintEvent(QPaintEvent *event) {
    resize();
}


void AsciiTable::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn) {
    short charCode = currentRow * ASCII_MATRIX_SIZE + currentColumn;

    ui->symbolCode->setText(QString::number(charCode));
    ui->symbolHexCode->setText(QString::number(charCode, 16));
    ui->symbol->setText(QChar(charCode));
}

void AsciiTable::resize() {
    m_charWidth     = uint16_t(fontMetrics().horizontalAdvance(QLatin1Char('9')));
    m_charHeight    = uint16_t(fontMetrics().height());

    uint16_t cellWidth = uint16_t(ui->tableWidget->width() / ASCII_MATRIX_SIZE) - m_charWidth / 2;
    uint16_t cellHeight = uint16_t(ui->tableWidget->height() / ASCII_MATRIX_SIZE) - m_charHeight / 2;

    for(short i = 0; i < ASCII_MATRIX_SIZE; i++) {
        ui->tableWidget->setColumnWidth(i, cellWidth);
        ui->tableWidget->setRowHeight(i, cellHeight);
    }
}

void AsciiTable::on_font_currentFontChanged(const QFont &newFont) {
    QFont font = ui->symbol->font();

    font.setFamily(newFont.family());

    ui->symbol->setFont(font);

    font = ui->tableWidget->font();

    font.setFamily(newFont.family());

    ui->tableWidget->setFont(font);
}

void AsciiTable::on_size_textEdited(const QString &value) {
    int size = value.toInt();
    QFont font = ui->tableWidget->font();
    font.setPointSize(size);

    ui->tableWidget->setFont(font);

    resize();
}
