#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initToolBar();
    initHexView();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initToolBar() {
    // File
    QMenu *file = ui->MenuBar->addMenu("&File");
    file->addSection("File");
    file->addAction("New");
    file->addAction("Open");
    file->addAction("Save");
    file->addAction("Save as...");
    file->addSection("Analyze");
    file->addAction("About the file");
    file->addAction("Open fileripper");
    file->addSeparator();
    file->addAction("Exit");

    // Edit
    QMenu *edit = ui->MenuBar->addMenu("&Edit");
    edit->addSection("History");
    edit->addAction("Undo");
    edit->addAction("Redo");
    edit->addAction("Open history");
    edit->addSection("Search");
    edit->addAction("Find");
    edit->addSection("Replace");
    edit->addSection("Navigation");
    edit->addAction("Goto offset");

    // View
    QMenu *view = ui->MenuBar->addMenu("&View");
    view->addSection("File view");
    view->addAction("Hex");
    view->addAction("Assembler");
    view->addAction("Strings");
    view->addAction("Raw");
    view->addSeparator();
    view->addAction("Save selection preset");
    view->addAction("Load selection preset");
    view->addSection("Settings");
    view->addAction("Preferences");
    view->addAction("Key bindings");

    // About
    QMenu *about = ui->MenuBar->addMenu("&About");
    about->addSection("Help");
    about->addAction("User manual (offline)");
    about->addAction("Online forum");
    about->addSection("About");
    about->addAction("About the program");
    about->addAction("About Qt");
    about->addAction("License");
}

void MainWindow::initHexView() {
    hexview = new QHexView;
    setCentralWidget(hexview);
}
