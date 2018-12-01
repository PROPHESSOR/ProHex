#include "asciitable.h"
#include "ui_asciitable.h"

AsciiTable::AsciiTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AsciiTable) {
    ui->setupUi(this);

    m_charWidth     = uint16_t(fontMetrics().width(QLatin1Char('9')));
    m_charHeight    = uint16_t(fontMetrics().height());

    ui->tableWidget->setRowCount(ASCII_MATRIX_SIZE);
    ui->tableWidget->setColumnCount(ASCII_MATRIX_SIZE);

    for(short i = 0; i < ASCII_MATRIX_SIZE; i++) {
        QTableWidgetItem *header = new QTableWidgetItem(QString::number(i, 16));

        ui->tableWidget->setHorizontalHeaderItem(i, header);
        ui->tableWidget->setVerticalHeaderItem(i, header);

        for(short j = 0; j < ASCII_MATRIX_SIZE; j++) {
            QTableWidgetItem *tmp = new QTableWidgetItem(QChar(j * ASCII_MATRIX_SIZE + i));

            tmp->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget->setItem(i, j, tmp);
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
    ui->symbolCode->setText(QString::number(currentColumn * ASCII_MATRIX_SIZE + currentRow));
    ui->symbolHexCode->setText(QString::number(currentColumn * ASCII_MATRIX_SIZE + currentRow, 16));
    ui->symbol->setText(QChar(currentColumn * ASCII_MATRIX_SIZE + currentRow));
}

void AsciiTable::resize() {
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
