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

    // Generate table

    //ui->tableWidget->setColumnCount(4);
    ui->listWidget->addItem(tr("Address area color"));
    ui->listWidget->addItem(tr("Selection color"));
    ui->listWidget->addItem(tr("Rectangle cursor color"));
    ui->listWidget->addItem(tr("HEX color"));
    ui->listWidget->addItem(tr("Address color"));
    ui->listWidget->addItem(tr("ASCII color"));
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
    QString itemName    = item->text().split("-")[0];
    QColor tmp          = QColorDialog::getColor();
    item->setTextColor(tmp);

    // item->setText(itemName + " - " + tmp);
}
