#pragma once

#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

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

    uint8_t getLanguage();

    // Setters
    void    setViewShowAddress(bool);
    void    setViewShowHex(bool);
    void    setViewShowAscii(bool);

    void    setLanguage(uint8_t);

    static QString getConfigFilePath();
  private:
    bool    v_show_address;
    bool    v_show_hex;
    bool    v_show_ascii;
    uint8_t a_language;
};
