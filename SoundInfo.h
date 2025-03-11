#ifndef SOUNDINFO_H
#define SOUNDINFO_H

#include <QObject>
#include <QQmlEngine>

class SoundInfo : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged REQUIRED)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QUrl imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(QUrl soundPath READ soundPath WRITE setSoundPath NOTIFY soundPathChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged REQUIRED)

public:
    explicit SoundInfo(QObject *parent = nullptr);

    QString id() const;
    void setId(QString newId);

    QString name() const;
    void setName(const QString &newName);

    QUrl imagePath() const;
    void setImagePath(const QUrl &newImagePath);

    QUrl soundPath() const;
    void setSoundPath(const QUrl &newSoundPath);

    QString color() const;
    void setColor(const QString &newColor);


signals:
    void idChanged();
    void nameChanged();

    void imagePathChanged();

    void soundPathChanged();

    void colorChanged();

private:
    QString m_id;
    QString m_name;
    QUrl m_imagePath;
    QUrl m_soundPath;
    QString m_color;
};

#endif // SOUNDINFO_H
