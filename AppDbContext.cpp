#include "AppDbContext.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUuid>
#include <QStandardPaths>
#include <QDir>
#include "SoundInfo.h"

AppDbContext::AppDbContext(QObject *parent) : QObject(parent)
{
    initData();
    initConfig();
}

AppDbContext::~AppDbContext()
{
    delete dataFile;
    dataFile = nullptr;
}

void AppDbContext::initConfig()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName =  "/config.json";
    QDir dir;
    configFile = new QFile(filePath+fileName);

    if(!dir.exists(filePath) && !dir.mkpath(filePath))
    {
        qDebug() << "Error occurred when creating path";
        return;
    }

    if (!configFile->exists()) {
        QJsonArray initArray;
        QJsonObject config;
        config["outputVolume"] = 0.5;
        config["inputVolume"] = 0.5;
        initArray << config;
        QJsonDocument doc(initArray);
        if (configFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            configFile->write(doc.toJson());
            configFile->close();
            qDebug() << "Created new config.json " << configFile->fileName();
        } else {
            qDebug() << "Failed to create config.json!";
        }
    }
}

void AppDbContext::initData()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName =  "/data.json";
    QDir dir;
    dataFile = new QFile(filePath+fileName);

    if(!dir.exists(filePath) && !dir.mkpath(filePath))
    {
        qDebug() << "Error occurred when creating path";
        return;
    }

    if (!dataFile->exists()) {
        QJsonArray emptyArray;
        QJsonDocument doc(emptyArray);
        if (dataFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            dataFile->write(doc.toJson());
            dataFile->close();
            qDebug() << "Created new data.json " << dataFile->fileName();
        } else {
            qDebug() << "Failed to create data.json!";
        }
    }
}

void AppDbContext::addSound(QString name, QUrl imagePath, QUrl soundPath, QString color)
{
    if (!dataFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading!";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(dataFile->readAll());
    QJsonArray soundsArray = doc.array();
    dataFile->close();

    QJsonObject newSound;
    newSound["id"] = QUuid::createUuid().toString(QUuid::WithoutBraces);
    newSound["name"] = name;
    newSound["imagePath"] = imagePath.toString();
    newSound["soundPath"] = soundPath.toString();
    newSound["color"] = color;
    soundsArray.append(newSound);

    if (!dataFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to write to file!";
        return;
    }

    dataFile->write(QJsonDocument(soundsArray).toJson());
    dataFile->close();
    emit soundsChanged();
    qDebug() << "Sound added successfully.";
}

QList<SoundInfo*> AppDbContext::getAllSounds()
{
    if (!dataFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading!";
        return {};
    }

    QJsonDocument doc = QJsonDocument::fromJson(dataFile->readAll());
    QJsonArray soundsArray = doc.array();
    dataFile->close();

    QList<SoundInfo*> sounds;
    for (const QJsonValue &value : soundsArray) {
        QJsonObject obj = value.toObject();
        SoundInfo* sound = new SoundInfo();
        sound->setId(obj["id"].toString());
        sound->setName(obj["name"].toString());
        sound->setImagePath(QUrl(obj["imagePath"].toString()));
        sound->setSoundPath(QUrl(obj["soundPath"].toString()));
        sound->setColor(obj["color"].toString());
        sounds.append(sound);
    }

    return sounds;
}

void AppDbContext::deleteSound(QString id)
{
    if (!dataFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading!";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(dataFile->readAll());
    QJsonArray soundsArray = doc.array();
    dataFile->close();

    bool found = false;
    for (int i = 0; i < soundsArray.size(); ++i) {
        if (soundsArray[i].toObject()["id"].toString() == id) {
            soundsArray.removeAt(i);
            found = true;
            break;
        }
    }

    if (!found) {
        qDebug() << "Sound ID not found!";
        return;
    }

    if (!dataFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to write to file!";
        return;
    }

    dataFile->write(QJsonDocument(soundsArray).toJson());
    dataFile->close();
    emit soundsChanged();
    qDebug() << "Sound deleted successfully.";
}

Config *AppDbContext::getConfig()
{
    if (!configFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading!";
        return {};
    }

    QJsonDocument doc = QJsonDocument::fromJson(configFile->readAll());
    QJsonArray configArray = doc.array();
    configFile->close();

    Config* config = new Config;
    for (const QJsonValue &value : configArray) {
        QJsonObject obj = value.toObject();
        config->setOutputVolume(obj["outputVolume"].toDouble());
        config->setInputVolume(obj["inputVolume"].toDouble());
    }

    return config;
}

void AppDbContext::updateConfig(float outputValue, float inputValue)
{
    if (configFile->exists()) {
        QJsonArray configArray;
        QJsonObject config;
        config["outputVolume"] = outputValue;
        config["inputVolume"] = inputValue;
        configArray << config;
        QJsonDocument doc(configArray);
        if (configFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            configFile->write(doc.toJson());
            configFile->close();
            qDebug() << "Updated new config.json " << configFile->fileName();
        } else {
            qDebug() << "Failed to update config.json!";
        }
    }
}
