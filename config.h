#pragma once

#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QColor>

#define TMPLANGS 1

class Config {
  public:
    Config();

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

    QString getLanguage();

    // Setters
    void    setViewShowAddress(bool);
    void    setViewShowHex(bool);
    void    setViewShowAscii(bool);

    void    setLanguage(QString);

    static QString getConfigFilePath();
  private:
    bool    v_show_address;
    bool    v_show_hex;
    bool    v_show_ascii;

    QColor c_address_area;//  0xd4, 0xd4, 0xd4, 0xff
    QColor c_selection;//     0x6d, 0x9e, 0xff, 0xff
    QColor c_cursor;//        0xcc, 0xcc, 0xcc, 0xff

    QColor c_hex;//              0x00, 0x00, 0x00, 0xff
    QColor c_address;//          0x00, 0x00, 0x00, 0xff
    QColor c_ascii;//            0x00, 0x00, 0x00, 0xff

    QColor c_active_window;//    0x00, 0x00, 0xff, 0xff

    QString a_language;
};
