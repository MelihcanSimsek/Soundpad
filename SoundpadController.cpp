#include "SoundpadController.h"
#include <QMediaDevices>
#include <QAudioOutput>
#include <QInputDevice>
#include <QUrl>
#include <QFile>
#include <QUuid>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonDocument>

SoundpadController::SoundpadController(QObject *parent)
    : QAbstractListModel{parent}
{
    m_soundList = dbContext.getAllSounds();
    config = dbContext.getConfig();
    m_mixer = new AudioMixer(this,config->inputVolume());

    if(!QMediaDevices::audioOutputs().isEmpty())
    {
        m_audioOutput = new QAudioOutput(QMediaDevices::defaultAudioOutput());
        m_mediaPlayer.setAudioOutput(m_audioOutput);
        m_audioOutput->setVolume(config->outputVolume());


    }

    if (!QMediaDevices::audioInputs().isEmpty()) {

        m_realMicDevice = new QAudioDevice(QMediaDevices::defaultAudioInput());
    }

    if (!QMediaDevices::audioOutputs().isEmpty()) {
        QAudioDevice virtualMicDevice;
        for (const QAudioDevice &device : QMediaDevices::audioOutputs()) {
            if (device.description().contains("VB", Qt::CaseInsensitive)) {
                m_virtualMicDevice = new QAudioDevice(device);
                break;
            }
        }

        if (m_virtualMicDevice == nullptr) {
            qDebug() << "Please download Mic Virtualization Tool for better usage (VB Cable)";
            m_virtualMicDevice =new QAudioDevice(QMediaDevices::defaultAudioInput());
        }
    }

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioDecoder = new QAudioDecoder(this);
    m_audioDecoder->setAudioFormat(format);
    connect(m_audioDecoder, &QAudioDecoder::bufferReady, this, &SoundpadController::handleDecodedAudio);

    connect(&dbContext,&AppDbContext::soundsChanged,this,&SoundpadController::getAllSounds);
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &SoundpadController::onMediaStatusChanged);


    m_mixer->startMixing(*m_realMicDevice,*m_virtualMicDevice);
}

SoundpadController::~SoundpadController()
{
    m_audioOutput->deleteLater();
}


void SoundpadController::handleDecodedAudio()
{
    if (!m_currentSound || !m_realMicDevice) {
        return;
    }

    if (m_audioDecoder->bufferAvailable()) {
        QAudioBuffer buffer = m_audioDecoder->read();

        if (!buffer.isValid() || buffer.byteCount() == 0) {
            return;
        }

        const auto* rawData = buffer.constData<quint8>();
        QByteArray audioData(reinterpret_cast<const char*>(rawData), buffer.byteCount());

        m_mixer->addSoundpadData(audioData);
    }
}

void SoundpadController::setPitch(float pitch)
{
    m_mixer->setPitch(pitch);
}

void SoundpadController::changeDeletingMode()
{

    m_deletingMode = !m_deletingMode;
    emit deletingModeChanged(m_deletingMode);

}

int SoundpadController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_soundList.length();
}

QVariant SoundpadController::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() < m_soundList.length()) {
        SoundInfo *soundInfo = m_soundList[index.row()];

        switch((Role) role) {
        case SoundId:
            return soundInfo->id();
        case SoundName:
            return soundInfo->name();
        case SoundImagePath:
            return soundInfo->imagePath();
        case SoundAudioPath:
            return soundInfo->soundPath();
        case SoundColor:
            return soundInfo->color();
        }
    }

    return {};
}

QHash<int, QByteArray> SoundpadController::roleNames() const
{
    QHash<int, QByteArray> result;

    result[SoundId] = "soundId";
    result[SoundName] = "soundName";
    result[SoundImagePath] = "imagePath";
    result[SoundAudioPath] = "soundPath";
    result[SoundColor] = "soundColor";

    return result;
}

void SoundpadController::getAllSounds()
{
    beginResetModel();
    qDeleteAll(m_soundList);
    m_soundList.clear();
    m_soundList = dbContext.getAllSounds();
    endResetModel();
}

void SoundpadController::add(QString name, QUrl imageUrl, QUrl soundUrl, QString color)
{
    QFile oldImage(imageUrl.toLocalFile());
    QFile oldSound(soundUrl.toLocalFile());

    if (!oldImage.exists()) {
        qWarning() << "Source not found " << oldImage.fileName();
        return;
    }

    if (!oldSound.exists()) {
        qWarning() << "Source not found " << oldSound.fileName();
        return;
    }
    QString imageDir = QCoreApplication::applicationDirPath() + "/upload/images/";
    QString imagePath = imageDir + QUuid::createUuid().toString(QUuid::WithoutBraces) + "." +QFileInfo(oldImage).suffix();

    QString soundDir = QCoreApplication::applicationDirPath() + "/upload/sounds/";
    QString soundPath = soundDir + QUuid::createUuid().toString(QUuid::WithoutBraces) + "." +QFileInfo(oldSound).suffix();

    QDir dir;
    if(!dir.exists(imageDir) && !dir.mkpath(imageDir)){
        qWarning() << "FAIL image dir can not be init";
        return;
    }

    if(!dir.exists(soundDir) && !dir.mkpath(soundDir)){
        qWarning() << "FAIL sound dir can not be init";
        return;
    }

    if(!QFile::copy(oldImage.fileName(),imagePath))
    {
        qWarning() << "FAIL image can not be copy";
        return;
    }

    if(!QFile::copy(oldSound.fileName(),soundPath))
    {
        qWarning() << "FAIL sound can not be copy";
        QFile::remove(imagePath);
        return;
    }

    dbContext.addSound(name,QUrl::fromLocalFile(imagePath), QUrl::fromLocalFile(soundPath),color);
}


void SoundpadController::deleteSound(QString id,QUrl imageUrl, QUrl soundUrl)
{
    changeDeletingMode();
    dbContext.deleteSound(id);
    QFile::remove(imageUrl.toLocalFile());
    QFile::remove(soundUrl.toLocalFile());

}


SoundInfo *SoundpadController::currentSound() const
{
    return m_currentSound;
}

void SoundpadController::setCurrentSound(SoundInfo* newCurrentSound)
{
    if (m_currentSound != nullptr && m_currentSound->id() == newCurrentSound->id())
    {
        m_mediaPlayer.stop();
        m_audioDecoder->stop();
        m_mixer->clearBuffer();
        m_currentSound = nullptr;
    }else{
        m_mediaPlayer.stop();
        m_audioDecoder->stop();
        m_mixer->clearBuffer();
        m_mediaPlayer.setSource(newCurrentSound->soundPath());
        m_audioDecoder->setSource(newCurrentSound->soundPath());
        m_currentSound = newCurrentSound;
        m_mediaPlayer.play();
        m_audioDecoder->start();

    }

    emit currentSoundChanged();
}



void SoundpadController::setCurrentSound(const QString &soundId,
                                         const QString &soundName,
                                         const QString &imagePath,
                                         const QString &soundPath,
                                         const QString &soundColor)
{
    SoundInfo* newCurrentSound = new SoundInfo();
    newCurrentSound->setId(soundId);
    newCurrentSound->setName(soundName);
    newCurrentSound->setImagePath(imagePath);
    newCurrentSound->setSoundPath(soundPath);
    newCurrentSound->setColor(soundColor);

    setCurrentSound(newCurrentSound);
}

void SoundpadController::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        m_currentSound = nullptr;
        m_mediaPlayer.stop();
        m_mixer->clearBuffer();
        emit currentSoundChanged();
    }
}

void SoundpadController::changeOutputVolume(int volume)
{
    m_audioOutput->setVolume((float)(volume/100.0));
}

int SoundpadController::getOutputValue()
{
    return (int)config->outputVolume()*100;
}

int SoundpadController::getInputValue()
{
    return (int)config->inputVolume()*100;
}

void SoundpadController::updateVolume(int outputVolume, float inputVolume)
{
    float realOutputVolume = (float)(outputVolume/100.0);
    float realInputVolume = (float)(inputVolume/100.0);

    config->setOutputVolume(realOutputVolume);
    config->setInputVolume(realInputVolume);
    m_mixer->setInputVolume(realInputVolume);

    dbContext.updateConfig(realOutputVolume,realInputVolume);
}

void SoundpadController::openVolumeDialog()
{
    emit volumeSliderChanged(config->outputVolume()*100,config->inputVolume()*100);
}
