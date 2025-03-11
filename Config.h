#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QQmlEngine>

class Config : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(float outputVolume READ outputVolume WRITE setOutputVolume NOTIFY outputVolumeChanged)
    Q_PROPERTY(float inputVolume READ inputVolume WRITE setInputVolume NOTIFY inputVolumeChanged)
public:
    explicit Config(QObject *parent = nullptr);

    float outputVolume() const;
    void setOutputVolume(float newOutputVolume);

    float inputVolume() const;
    void setInputVolume(float newInputVolume);

signals:
    void outputVolumeChanged();
    void inputVolumeChanged();

private:
    float m_outputVolume;
    float m_inputVolume;
};

#endif // CONFIG_H
