#pragma once

#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QColor>
#include <QStandardPaths>
#include <QDir>


#define TMPLANGS 1

class Config {
  public:
    Config();

    // TODO: Getter/setter
    QColor c_address_area;
    QColor c_selection;
    QColor c_cursor;

    QColor c_hex;
    QColor c_address;
    QColor c_ascii;

    QColor c_active_window;


    bool    load();
    bool    save();
    bool    loadFromJson(QJsonObject);
    bool    saveToJson(QJsonObject &);
    void    reset();

    // Getters
    bool    getViewShowAddress();
    bool    getViewShowHex();
    bool    getViewShowAscii();

    QColor  getColorAddressArea();
    QColor  getColorSelection();
    QColor  getColorCursor();
    QColor  getColorHex();
    QColor  getColorAddress();
    QColor  getColorAscii();
    QColor  getColorActiveWindow();

    uint8_t getWindowTheme();

    QString getLanguage();

    // Setters
    void    setViewShowAddress(bool);
    void    setViewShowHex(bool);
    void    setViewShowAscii(bool);

    void    setWindowTheme(uint8_t);

    void    setLanguage(QString);

    static QString getConfigFilePath();
  private:
    bool    v_show_address;
    bool    v_show_hex;
    bool    v_show_ascii;

    QString a_language;
    uint8_t a_window_theme;
};
