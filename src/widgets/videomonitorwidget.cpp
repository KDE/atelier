/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "videomonitorwidget.h"
#include <QDir>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QtMultimediaWidgets/QVideoWidget>
#include <KLocalizedString>


VideoMonitorWidget::VideoMonitorWidget(QWidget *parent) :
    QWidget(parent),
    _mediaplayer(nullptr, QMediaPlayer::VideoSurface)
{    
    auto _layout = new QGridLayout();
    auto _label = new QLabel(i18n("Source url:"));
    _layout->addWidget(_label, 0, 0);
    
    auto _sourceCB = new QComboBox();
    _sourceCB->setEditable(true);
    _sourceCB->setToolTip(i18n("Valid Urls:\n\
        http://www.example.com/stream.avi\n\
        rtp://@:1234\n\
        mms://mms.examples.com/stream.asx\n\
        rtsp://server.example.org:8080/test.sdp"));
    _layout->addWidget(_sourceCB, 0, 1);
    
    auto _playPB = new QPushButton();
    _playPB->setCheckable(true);
    _playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    _layout->addWidget(_playPB, 0, 2);
    
    auto _videoWidget = new QVideoWidget();
    _layout->addWidget(_videoWidget, 1, 0, -1, -1);
    
    _errorlabel = new QLabel;
    _layout->addWidget(_errorlabel, 2, 0, 0, -1);
    
    this->setLayout(_layout);
    
    _mediaplayer.setVideoOutput(_videoWidget);
       
#ifdef Q_OS_LINUX
    QStringList sources;
    sources << QString("video*");
    _sourceCB->addItems(QDir("/dev/")\
        .entryList(sources, QDir::System)\
        .replaceInStrings( QRegExp("^"), "v4l2:///dev/"));
#endif
    
    connect(_playPB, &QPushButton::clicked, [this, _playPB, _sourceCB](bool b){
        if(b){
            _playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            QString source = _sourceCB->currentText();
            _mediaplayer.setMedia(QUrl(source));
            _mediaplayer.play();
        }else{
            _mediaplayer.pause();
            _playPB->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }
    });
    
    typedef void (QMediaPlayer::*ErrorSignal)(QMediaPlayer::Error);
    connect(&_mediaplayer, static_cast<ErrorSignal>(&QMediaPlayer::error),
            this, &VideoMonitorWidget::handleError);
}


VideoMonitorWidget::~VideoMonitorWidget()
{

}

void VideoMonitorWidget::handleError()
{
    const QString errorString = _mediaplayer.errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(_mediaplayer.error()));
    else
        message += errorString;
    _errorlabel->setText(message);
}
