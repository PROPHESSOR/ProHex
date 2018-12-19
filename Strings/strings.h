#pragma once

#include <QWidget>
#include <QByteArray>
#include <QList>
#include <QListWidgetItem>

#include "../DataStorage/datastorage.h"

#define LARGELIST 100

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
    void on_searchInput_textEdited(const QString &arg1);

    void on_searchInput_returnPressed();

  private:
    Ui::Strings             *ui;

    QList<QListWidgetItem *> m_searchlist;
    int32_t                  m_selectedIndex;
    bool                     m_searchChanged; // Using for optimized "Enter" search

    void clearSelection();
    void search(const QString &);
    void goToItem(int32_t idx = -1);
};
