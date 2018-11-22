#pragma once

#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

class Config {
  public:
    Config();

    bool load();
    bool save();
    bool loadFromJson(QJsonObject);
    bool saveToJson(QJsonObject &);

    static QString getConfigFilePath();

    // Getters
    bool getViewShowAddress();
    bool getViewShowHex();
    bool getViewShowAscii();

    // Setters
    void setViewShowAddress(bool);
    void setViewShowHex(bool);
    void setViewShowAscii(bool);
  private:
    bool v_show_address;
    bool v_show_hex;
    bool v_show_ascii;
};
