#ifndef SOUNDPADCONTROLLER_H
#define SOUNDPADCONTROLLER_H

#include <QObject>
#include <QAbstractListModel>
#include <QMediaPlayer>
#include <QAudioSource>
#include <QAudioSink>
#include <QAudioDecoder>
#include <QAudioDevice>

#include "SoundInfo.h"
#include "AudioMixer.h"
#include "AppDbContext.h"

class SoundpadController : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(SoundInfo* currentSound READ currentSound WRITE setCurrentSound NOTIFY currentSoundChanged )
public:
    enum Role {
        SoundId = Qt::UserRole + 1,
        SoundName,
        SoundImagePath,
        SoundAudioPath,
        SoundColor
    };
    explicit SoundpadController(QObject *parent = nullptr);
    ~SoundpadController();
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    SoundInfo *currentSound() const;
    void setCurrentSound(SoundInfo* newCurrentSound);


public slots:
    void getAllSounds();
    void add(QString name,QUrl imageUrl, QUrl soundUrl,QString color);
    void deleteSound(QString id, QUrl imageUrl, QUrl soundUrl);
    void setCurrentSound(const QString &soundId,
                         const QString &soundName,
                         const QString &imagePath,
                         const QString &soundPath,
                         const QString &soundColor);

    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void changeOutputVolume(int volume);
    int getOutputValue();
    int getInputValue();
    void updateVolume(int outputVolume,float inputVolume);
    void openVolumeDialog();
    void handleDecodedAudio();
    void setPitch(float pitch);
    void changeDeletingMode();
signals:

    void currentSoundChanged();
    void volumeSliderChanged(int outputVolume,int inputVolume);
    void deletingModeChanged(bool mode);


private:
    AppDbContext dbContext;
    AudioMixer* m_mixer = nullptr;
    QMediaPlayer m_mediaPlayer;
    QList<SoundInfo*> m_soundList;
    Config* config=nullptr;
    QAudioOutput* m_audioOutput=nullptr;
    SoundInfo *m_currentSound = nullptr;
    QAudioDecoder* m_audioDecoder= nullptr;
    QAudioDevice* m_realMicDevice=nullptr;
    QAudioDevice* m_virtualMicDevice= nullptr;
    bool m_deletingMode = false;
};



#endif // SOUNDPADCONTROLLER_H
