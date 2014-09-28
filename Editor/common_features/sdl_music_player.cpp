/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtDebug>

#include "sdl_music_player.h"

// //////////////////////// Music Player //////////////////////////////////////

void PGE_MusPlayer::setVolume(int volume)
{
    MUS_changeVolume(volume);
}


Mix_Music *PGE_MusPlayer::play_mus = NULL;

int PGE_MusPlayer::volume=100;
int PGE_MusPlayer::sRate=44100;

#ifdef USE_QMEDIAPLAYER
bool PGE_MusPlayer::isMediaPlayer=false;
QMediaPlayer * PGE_MusPlayer::musicPlayer=NULL;
QMediaPlaylist * PGE_MusPlayer::playList=NULL;
#endif

void PGE_MusPlayer::MUS_stopMusic()
{
    #ifdef USE_QMEDIAPLAYER
    if(isMediaPlayer)
        musicPlayer->stop();
    else
    #endif
        Mix_HaltMusic();
}

void PGE_MusPlayer::MUS_playMusic()
{
    #ifdef USE_QMEDIAPLAYER
    if(isMediaPlayer)
    {
        qDebug() << QString("Play Music (QMediaPlayer)");
        musicPlayer->play();
    }
    else
    {
    #endif
        if(play_mus)
        {
            qDebug() << QString("Play Music (SDL2_mixer)");
            if(Mix_PlayMusic(play_mus, -1)==-1)
            {
                qDebug() << QString("Mix_PlayMusic: %1").arg(Mix_GetError());
                // well, there's no music, but most games don't break without music...
            }

            qDebug() << QString("Music is %1").arg(Mix_PlayingMusic()==1?"Playing":"Silence");
        }
        else
        {
            qDebug() << QString("Play nothing: Mix_PlayMusic: %1").arg(Mix_GetError());
        }
    #ifdef USE_QMEDIAPLAYER
    }
    #endif
}

void PGE_MusPlayer::MUS_changeVolume(int vlm)
{
    qDebug() << QString("Volume changed %1").arg(vlm);
    volume = vlm;
    #ifdef USE_QMEDIAPLAYER
    if(isMediaPlayer)
    {
        if(volume>100) volume=100;
        musicPlayer->setVolume(volume);
    }
    else
    #endif
        Mix_VolumeMusic(volume);


}

void PGE_MusPlayer::setSampleRate(int sampleRate=44100)
{
    sRate=sampleRate;
    Mix_CloseAudio();
    Mix_OpenAudio(sRate, AUDIO_S16, 2, 4096);
}
int PGE_MusPlayer::sampleRate()
{
    return sRate;
}

int PGE_MusPlayer::currentVolume()
{
    return volume;
}


void PGE_MusPlayer::MUS_openFile(QString musFile)
{
    if(play_mus!=NULL)
    {
        Mix_FreeMusic(play_mus);
        play_mus=NULL;
    }

    #ifdef USE_QMEDIAPLAYER
    if(isMediaPlayer) if(musicPlayer!=NULL)
    {
        musicPlayer->stop();
        delete musicPlayer;
        delete playList;
        isMediaPlayer=false;
    }
    #endif

    #ifdef USE_QMEDIAPLAYER
    //Play MP3-filed with QMediaPlayer
    if(musFile.endsWith(".mp3", Qt::CaseInsensitive))
    {
        isMediaPlayer=true;
        musicPlayer = new QMediaPlayer();
        playList = new QMediaPlaylist();
        playList->clear();
        playList->addMedia(QUrl::fromLocalFile( musFile ));
        playList->setPlaybackMode(QMediaPlaylist::Loop);
        musicPlayer->setPlaylist(playList);
    }
    else
    {
        isMediaPlayer=false;
    #endif

        play_mus = Mix_LoadMUS( musFile.toUtf8() );
        if(!play_mus) {
            qDebug() << QString("Mix_LoadMUS(\"%1\"): %2").arg(musFile).arg(Mix_GetError());
        }

        //Print memory address of pointer
        //qDebug() << "Pointer is " << static_cast<void*>(&play_mus);

        Mix_MusicType type=Mix_GetMusicType(play_mus);
        qDebug() << QString("Music type: %1").arg(
                type==MUS_NONE?"MUS_NONE":
                type==MUS_CMD?"MUS_CMD":
                type==MUS_WAV?"MUS_WAV":
                /*type==MUS_MOD_MODPLUG?"MUS_MOD_MODPLUG":*/
                type==MUS_MOD?"MUS_MOD":
                type==MUS_MID?"MUS_MID":
                type==MUS_OGG?"MUS_OGG":
                type==MUS_MP3?"MUS_MP3":
                type==MUS_MP3_MAD?"MUS_MP3_MAD":
                type==MUS_FLAC?"MUS_FLAC":
                "Unknown");
    #ifdef USE_QMEDIAPLAYER
    }
    #endif
}




// //////////////////////// Sound Player //////////////////////////////////////



#ifdef USE_QMEDIAPLAYER
QMediaPlayer * PGE_Sounds::mp3Play=NULL;
#else
Mix_Music *PGE_Sounds::mp3_sound = NULL;
#endif
bool PGE_Sounds::isMp3=false;

Mix_Chunk *PGE_Sounds::sound = NULL;
QString PGE_Sounds::current = "";

void PGE_Sounds::SND_PlaySnd(QString sndFile)
{
    if(current!=sndFile)
    {
        isMp3=false;
        if(sound) { Mix_FreeChunk(sound); sound=NULL; }
        #ifdef USE_QMEDIAPLAYER
        if(mp3Play) { mp3Play->stop(); delete mp3Play; mp3Play=NULL; }
        #else
        if(mp3_sound) { Mix_FreeMusic(mp3_sound); mp3_sound=NULL; }
        #endif

        if(sndFile.endsWith(".mp3", Qt::CaseInsensitive))
        {
            isMp3=true;
            #ifdef USE_QMEDIAPLAYER
            mp3Play = new QMediaPlayer();
            mp3Play->setMedia(QMediaContent(QUrl(sndFile)));
            mp3Play->setVolume(100);
            #else
            mp3_sound = Mix_LoadMUS( sndFile.toUtf8() );
            if(!mp3_sound)
            {
                qDebug() << QString("Mix_LoadMUS(\"%1\"): %2").arg(sndFile).arg(Mix_GetError());
            }
            #endif
        }
        else
        {
            sound = Mix_LoadWAV(sndFile.toUtf8() );
            if(!sound) {
                qDebug() << QString("Mix_LoadWAV: %s").arg(Mix_GetError());
                // handle error
            }
        }

    }

    if(isMp3)
    {
        #ifdef USE_QMEDIAPLAYER
        qDebug() << QString("Play Sound (QMediaPlayer)");
        if(!mp3Play)
        {
            qDebug() << QString("QMediaPlayer is null");
        }
        else
            mp3Play->play();
        #else
        if(mp3_sound)
        {
            qDebug() << QString("Play Sound (SDL2_mixer, LibMAD)");
            if(Mix_PlayMusic(mp3_sound, 1)==-1)
            {
                qDebug() << QString("Mix_PlayMusic: %1").arg(Mix_GetError());
            }
        }
        #endif
    }
    else
    {
        qDebug() << QString("Play Sound (SDL2_mixer)");
        if(Mix_PlayChannel( -1, sound, 0 )==-1)
        {
            qDebug() << QString("Mix_PlayChannel: %1").arg(Mix_GetError());
        }
    }

}
