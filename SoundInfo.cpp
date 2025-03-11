#include "SoundInfo.h"

SoundInfo::SoundInfo(QObject *parent)
    : QObject{parent}
{


}

QString SoundInfo::id() const
{
    return m_id;
}

void SoundInfo::setId(QString newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString SoundInfo::name() const
{
    return m_name;
}

void SoundInfo::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QUrl SoundInfo::imagePath() const
{
    return m_imagePath;
}

void SoundInfo::setImagePath(const QUrl &newImagePath)
{
    if (m_imagePath == newImagePath)
        return;
    m_imagePath = newImagePath;
    emit imagePathChanged();
}

QUrl SoundInfo::soundPath() const
{
    return m_soundPath;
}

void SoundInfo::setSoundPath(const QUrl &newSoundPath)
{
    if (m_soundPath == newSoundPath)
        return;
    m_soundPath = newSoundPath;
    emit soundPathChanged();
}

QString SoundInfo::color() const
{
    return m_color;
}

void SoundInfo::setColor(const QString &newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    emit colorChanged();
}
