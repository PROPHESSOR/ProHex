#pragma once

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QColor>
#include <QInputDialog>
#include <QColorDialog>
#include "../Config/config.h"

namespace Ui {
class Preferences;
}

class Preferences : public QWidget {
    Q_OBJECT

  public:
    explicit Preferences(QWidget *parent = nullptr, Config *config = nullptr);
    ~Preferences();
    void showTab(uint8_t);
    void load();
    void save();

  private:
    Ui::Preferences *ui;
    Config          *m_config = nullptr;

    // void updateColorScheme();

  protected:
    virtual void closeEvent(QCloseEvent *);

  private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *);
};
