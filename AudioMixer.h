#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H

#include <QObject>
#include <QAudioSource>
#include <QAudioSink>
#include <QIODevice>
#include <QTimer>
#include <QMutex>
#include <QByteArray>
#include <QAudioDevice>
#include <QtMinMax>

class AudioMixer : public QObject
{
    Q_OBJECT
public:
    explicit AudioMixer(QObject *parent = nullptr,float inputVolume = 1.0f,float pitch = 1.0f);
    void startMixing(const QAudioDevice& micDevice,const QAudioDevice& outputDevice);
    void stopMixing();
    void addSoundpadData(const QByteArray &soundpadData);
    void setInputVolume(float volume);
    void clearBuffer();
    void setPitch(float pitch);


signals:
    void mixingStarted();
    void mixingStopped();

private slots:
    void processMixedAudio();

private:
    QByteArray mixAudioData(const QByteArray &audio1, const QByteArray &audio2);
    void adjustVolumeAndPitch(QByteArray &audioData, float volume,float freq);
    float interpolate(const qint16* samples, int sampleCount, float pos);

    QAudioSource *m_audioInput = nullptr;
    QIODevice *m_micBuffer = nullptr;
    QAudioSink *m_audioOutput = nullptr;
    QIODevice *m_outputDevice = nullptr;
    QTimer *m_processTimer = nullptr;
    QByteArray m_soundpadBuffer;
    QMutex m_soundpadMutex;

    float m_inputVolume;
    float m_pitch;
};

#endif // AUDIOMIXER_H
