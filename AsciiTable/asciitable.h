#pragma once

#include <QWidget>
#include <QTableView>
#include <QDebug>

#define ASCII_MATRIX_SIZE 16

namespace Ui {
class AsciiTable;
}

class AsciiTable : public QWidget {
    Q_OBJECT

  public:
    explicit AsciiTable(QWidget *parent = nullptr);
    ~AsciiTable();

  protected:
    void paintEvent(QPaintEvent *event);

  private slots:
    void on_tableWidget_currentCellChanged(int, int, int, int);
    void on_font_currentFontChanged(const QFont &);

    void on_size_textEdited(const QString &arg1);

private:
    Ui::AsciiTable *ui;

    uint16_t        m_charWidth;
    uint16_t        m_charHeight;

    void            resize();
};
