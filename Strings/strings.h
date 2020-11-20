#pragma once

#include <QWidget>
#include <QByteArray>
#include <QList>
#include <QListWidgetItem>

#include "../DataStorage/datastorage.h"
#include "../Config/config.h"

#define STRINGS_SEPARATOR " - "

namespace Ui {
class Strings;
}

class Strings : public QWidget {
    Q_OBJECT

  public:
    explicit Strings(QWidget *parent = nullptr, Config *config = nullptr);
    ~Strings();

    void generateList(DataStorage *);

  public slots:
  signals:
    void offsetChanged(int64_t newOffset);

  private slots:
    void on_filterInput_textEdited(const QString &);
    void on_filterInput_returnPressed();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_stringLength_valueChanged(int arg1);

private:
    Ui::Strings    *ui;
    Config         *m_config = nullptr;

    QList<QString> m_list;
    uint8_t minStringLength;

    void filter(const QString &);
};
