#ifndef APPDBCONTEXT_H
#define APPDBCONTEXT_H

#include "Config.h"

#include <QFile>
#include <QObject>
#include <SoundInfo.h>

class AppDbContext : public QObject
{
    Q_OBJECT
public:
    explicit AppDbContext(QObject *parent = nullptr);
    ~AppDbContext();
    void initConfig();
    void initData();


public slots:
    void addSound(QString name,QUrl imagePath,QUrl soundPath,QString color);
    QList<SoundInfo*> getAllSounds();
    void deleteSound(QString id);
    Config* getConfig();
    void updateConfig(float outputValue,float inputValue);

signals:
    void soundsChanged();

private:
    QFile* dataFile;
    QFile* configFile;
};

#endif // APPDBCONTEXT_H
