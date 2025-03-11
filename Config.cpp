#include "Config.h"

Config::Config(QObject *parent)
    : QObject{parent}
{}


float Config::outputVolume() const
{
    return m_outputVolume;
}

void Config::setOutputVolume(float newOutputVolume)
{
    if (qFuzzyCompare(m_outputVolume, newOutputVolume))
        return;
    m_outputVolume = newOutputVolume;
    emit outputVolumeChanged();
}

float Config::inputVolume() const
{
    return m_inputVolume;
}

void Config::setInputVolume(float newInputVolume)
{
    if (qFuzzyCompare(m_inputVolume, newInputVolume))
        return;
    m_inputVolume = newInputVolume;
    emit inputVolumeChanged();
}
