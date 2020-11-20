#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent, Config *config) :
    QWidget(parent), ui(new Ui::Preferences), m_config(config) {
    ui->setupUi(this);

    load();
}

Preferences::~Preferences() {
    delete ui;
}

void Preferences::showTab(uint8_t tab) {
    // TODO: Validation

    ui->tabWidget->setCurrentIndex(tab);
    show();
}

void Preferences::load() {
    qDebug() << "Preferences::load()";

    ui->languageCombo->setCurrentText(m_config->getLanguage());
    ui->themeCombo->setCurrentIndex(m_config->getWindowTheme());

    ui->listWidget->clear();

    m_colorscheme[0].listItem = new QListWidgetItem(tr("Address area color"));
    m_colorscheme[0].configColor = &m_config->c_address_area;

    m_colorscheme[1].listItem = new QListWidgetItem(tr("Selection color"));
    m_colorscheme[1].configColor = &m_config->c_selection;

    m_colorscheme[2].listItem = new QListWidgetItem(tr("Rectangle cursor color"));
    m_colorscheme[2].configColor = &m_config->c_cursor;

    m_colorscheme[3].listItem = new QListWidgetItem(tr("HEX color"));
    m_colorscheme[3].configColor = &m_config->c_hex;

    m_colorscheme[4].listItem = new QListWidgetItem(tr("Address color"));
    m_colorscheme[4].configColor = &m_config->c_address;

    m_colorscheme[5].listItem = new QListWidgetItem(tr("ASCII color"));
    m_colorscheme[5].configColor = &m_config->c_ascii;

    for(uint8_t i = 0; i < COLOR_SCHEME_COUNT; i++) {
        QColor textcolor = QColor(255 - m_colorscheme[i].configColor->red(), 255 - m_colorscheme[i].configColor->green(), 255 - m_colorscheme[i].configColor->blue());
        m_colorscheme[i].listItem->setBackgroundColor(*m_colorscheme[i].configColor);
        m_colorscheme[i].listItem->setTextColor(textcolor);

        ui->listWidget->addItem(m_colorscheme[i].listItem);
    }

    ui->largeListSpin->setValue(m_config->getLargelist());
}

void Preferences::save() {
    qDebug() << "Preferences::save()";

    m_config->setLanguage(ui->languageCombo->currentText());
    m_config->save();
}

// void Preferences::updateColorScheme() {}

void Preferences::closeEvent(QCloseEvent *event) {
    qDebug() << "onPreferencesClose" << event;

    QMessageBox::StandardButton confirm = QMessageBox::question(this, tr("Save"), tr("Save preferences?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    switch(confirm) {
        case QMessageBox::Yes:
            save();
            break;
        default:
            break;
    }
}

void Preferences::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    QColor tmp          = QColorDialog::getColor();
    QColor textcolor    = QColor(255 - tmp.red(), 255 - tmp.green(), 255 - tmp.blue());
    uint8_t index       = ui->listWidget->currentRow();

    m_colorscheme[index].configColor->setRed(tmp.red());
    m_colorscheme[index].configColor->setGreen(tmp.green());
    m_colorscheme[index].configColor->setBlue(tmp.blue());
    item->setBackgroundColor(tmp);
    item->setTextColor(textcolor);
}

void Preferences::on_resetPreferences_clicked() {
    QMessageBox::StandardButton confirm = QMessageBox::question(this, tr("Reset"), tr("Are you sure to reset preferences?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    switch(confirm) {
        case QMessageBox::Yes:
            m_config->reset();
            save();
            hide();
            break;
        default:
            break;
    }
}

void Preferences::on_themeCombo_currentIndexChanged(int index) {
    m_config->setWindowTheme(index);
}

void Preferences::on_pushButton_clicked() {
    save();
    hide();
}

void Preferences::on_largeListSpin_valueChanged(int value) {
    m_config->setLargelist(value);
}
