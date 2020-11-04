#pragma once

#include <QWidget>
#include <QByteArray>
#include <QList>
#include <QListWidgetItem>

#include "../DataStorage/datastorage.h"

#define LARGELIST 10000000
// TODO: Add to settings

namespace Ui {
class Strings;
}

class Strings : public QWidget {
    Q_OBJECT

  public:
    explicit Strings(QWidget *parent = nullptr);
    ~Strings();

    void generateList(DataStorage *);

  private slots:
    void on_filterInput_textEdited(const QString &);
    void on_filterInput_returnPressed();

  private:
    Ui::Strings             *ui;

    QList<QString> m_list;

    void filter(const QString &);
};
