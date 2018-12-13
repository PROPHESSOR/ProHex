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
    ui->listWidget->addItem("Address area color");
    ui->listWidget->addItem("Selection color");
    ui->listWidget->addItem("Rectangle cursor color");
    ui->listWidget->addItem("HEX color");
    ui->listWidget->addItem("Address color");
    ui->listWidget->addItem("ASCII color");
}

void Preferences::save() {
    qDebug() << "Preferences::save()";

    m_config->setLanguage(ui->languageCombo->currentText());
    m_config->save();
}

// void Preferences::updateColorScheme() {}

void Preferences::closeEvent(QCloseEvent *event) {
    qDebug() << "onPreferencesClose" << event;

    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Save", "Save preferences?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    switch(confirm) {
        case QMessageBox::Yes:
            qDebug() << "Yes";
            save();
            break;
        case QMessageBox::No:
            qDebug() << "No";
            break;
        default:
            qDebug() << "Unknown result";
            break;
    }
}

void Preferences::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    QString itemName    = item->text().split("-")[0];
    QColor tmp          = QColorDialog::getColor();
    item->setTextColor(tmp);

    // item->setText(itemName + " - " + tmp);
}
