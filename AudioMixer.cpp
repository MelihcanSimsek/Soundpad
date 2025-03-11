#include "AudioMixer.h"
#include <QDebug>
#include <QtMath>


AudioMixer::AudioMixer(QObject *parent,float inputVolume,float pitch)
    : QObject{parent}
{
    m_inputVolume = inputVolume;
    m_pitch = pitch;
    m_processTimer = new QTimer(this);
    connect(m_processTimer, &QTimer::timeout, this, &AudioMixer::processMixedAudio);
}

void AudioMixer::startMixing(const QAudioDevice& micDevice,const QAudioDevice& outputDevice)
{
    stopMixing();

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioInput = new QAudioSource(micDevice, format, this);
    m_micBuffer = m_audioInput->start();

    if (!m_micBuffer) {
        qWarning() << "Failed to start audio input";
        return;
    }

    m_audioOutput = new QAudioSink(outputDevice, format, this);
    m_outputDevice = m_audioOutput->start();

    if (!m_outputDevice) {
        qWarning() << "Failed to start audio output";
        stopMixing();
        return;
    }

    m_processTimer->start(1);

    emit mixingStarted();
}

void AudioMixer::stopMixing()
{
    if (m_processTimer->isActive()) {
        m_processTimer->stop();
    }

    if (m_audioInput) {
        m_audioInput->stop();
        delete m_audioInput;
        m_audioInput = nullptr;
        m_micBuffer = nullptr;
    }

    if (m_audioOutput) {
        m_audioOutput->stop();
        delete m_audioOutput;
        m_audioOutput = nullptr;
        m_outputDevice = nullptr;
    }

    emit mixingStopped();
}

void AudioMixer::addSoundpadData(const QByteArray &soundpadData)
{
    QMutexLocker locker(&m_soundpadMutex);
    m_soundpadBuffer.append(soundpadData);
}

void AudioMixer::setInputVolume(float volume)
{
    m_inputVolume = volume;
}

void AudioMixer::clearBuffer()
{
    QMutexLocker locker(&m_soundpadMutex);
    m_soundpadBuffer.clear();
}

void AudioMixer::setPitch(float pitch)
{
    m_pitch = pitch;
}

void AudioMixer::processMixedAudio()
{
    if (!m_micBuffer || !m_outputDevice) {
        return;
    }

    const qint64 bytesAvailable = m_micBuffer->bytesAvailable();
    if (bytesAvailable <= 0) {
        return;
    }

    QByteArray micData = m_micBuffer->read(bytesAvailable);


    adjustVolumeAndPitch(micData, m_inputVolume,m_pitch);

    QByteArray soundpadData;
    {
        QMutexLocker locker(&m_soundpadMutex);


        int bytesToTake = qMin(m_soundpadBuffer.size(), micData.size());
        if (bytesToTake > 0) {
            soundpadData = m_soundpadBuffer.left(bytesToTake);
            m_soundpadBuffer.remove(0, bytesToTake);
        }
    }


    QByteArray mixedData;
    if (!soundpadData.isEmpty()) {

        adjustVolumeAndPitch(soundpadData, m_inputVolume,m_pitch);


        mixedData = mixAudioData(micData, soundpadData);
    } else {

        mixedData = micData;
    }


    if (m_outputDevice->bytesToWrite() < 32768) {
        m_outputDevice->write(mixedData);
    }
}

QByteArray AudioMixer::mixAudioData(const QByteArray &audio1, const QByteArray &audio2)
{
    int sampleCount = qMin(audio1.size(), audio2.size()) / sizeof(qint16);
    QByteArray result(sampleCount * sizeof(qint16), 0);

    const qint16 *samples1 = reinterpret_cast<const qint16*>(audio1.constData());
    const qint16 *samples2 = reinterpret_cast<const qint16*>(audio2.constData());
    qint16 *outSamples = reinterpret_cast<qint16*>(result.data());

    for (int i = 0; i < sampleCount; ++i) {
        qint32 mixed = static_cast<qint32>(samples1[i]) + static_cast<qint32>(samples2[i]);
        mixed = qBound<qint32>(-32768, mixed, 32767);
        outSamples[i] = static_cast<qint16>(mixed);
    }

    return result;
}



void AudioMixer::adjustVolumeAndPitch(QByteArray &audioData, float volume, float freq)
{
    if (audioData.isEmpty()) {
        return;
    }

    qint16 *samples = reinterpret_cast<qint16 *>(audioData.data());
    int sampleCount = audioData.size() / sizeof(qint16);


    for (int i = 0; i < sampleCount; ++i) {
        float sample = samples[i] * volume;
        samples[i] = qBound(-32768, static_cast<int>(sample), 32767);
    }


    if (qFuzzyCompare(freq, 1.0f)) {
        return;
    }


    QVector<qint16> tempBuffer(sampleCount);
    const qint16* originalSamples = samples;

    for (int i = 0; i < sampleCount; ++i) {
        float pos = i * freq;
        float sample = interpolate(originalSamples, sampleCount, pos);
        tempBuffer[i] = qBound(-32768, static_cast<int>(sample), 32767);
    }

    for (int i = 0; i < sampleCount; ++i) {
        samples[i] = tempBuffer[i];
    }
}


float AudioMixer::interpolate(const qint16* samples, int sampleCount, float pos) {
    if (pos < 0) {
        return samples[0];
    }
    if (pos >= sampleCount - 1) {
        return samples[sampleCount - 1];
    }
    int int_pos = static_cast<int>(pos);
    float frac = pos - int_pos;
    float sample1 = static_cast<float>(samples[int_pos]);
    float sample2 = static_cast<float>(samples[int_pos + 1]);
    return sample1 * (1.0f - frac) + sample2 * frac;
}
