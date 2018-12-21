#pragma once

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QColor>
#include <QInputDialog>
#include <QColorDialog>
#include "../Config/config.h"

#define COLOR_SCHEME_COUNT 6

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

    typedef struct {
        QListWidgetItem *listItem;
        QColor *configColor;
    } colorscheme_t;

  private:
    Ui::Preferences *ui;
    Config          *m_config = nullptr;

    colorscheme_t    m_colorscheme[6];

    // void updateColorScheme();

  protected:
    virtual void closeEvent(QCloseEvent *);

  private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *);
    void on_resetPreferences_clicked();
    void on_themeCombo_currentIndexChanged(int index);
    void on_pushButton_clicked();
};
