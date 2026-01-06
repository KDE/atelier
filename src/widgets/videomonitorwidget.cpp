/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Lays Rodrigues - lays.rodrigues@kde.org

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
#include <KLocalizedString>
#include <QComboBox>
#include <QDir>
#include <QGridLayout>
#include <QPushButton>
#include <QVideoWidget>

using namespace Qt::StringLiterals;

VideoMonitorWidget::VideoMonitorWidget(QWidget *parent)
    : QWidget(parent)
    , _errorlabel(new QLabel(this))
    , _mediaplayer(this)
{
    auto _layout = new QGridLayout();
    auto _label = new QLabel(i18n("Source url:"), this);
    _layout->addWidget(_label, 0, 0);

    auto _sourceCB = new QComboBox(this);
    _sourceCB->setEditable(true);
    _sourceCB->setToolTip(
        i18n("Valid Urls:\n\
        http://www.example.com/stream.avi\n\
        rtp://@:1234\n\
        mms://mms.examples.com/stream.asx\n\
        rtsp://server.example.org:8080/test.sdp"));
    _layout->addWidget(_sourceCB, 0, 1);

    auto _playPB = new QPushButton(this);
    _playPB->setCheckable(true);
    _playPB->setIcon(QIcon::fromTheme(u"media-playback-start"_s, style()->standardIcon(QStyle::SP_MediaPlay)));
    _layout->addWidget(_playPB, 0, 2);

    auto _videoWidget = new QVideoWidget(this);
    _layout->addWidget(_videoWidget, 1, 0, -1, -1);

    _layout->addWidget(_errorlabel, 2, 0, 0, -1);

    this->setLayout(_layout);

    _mediaplayer.setVideoOutput(_videoWidget);

#ifdef Q_OS_LINUX
    QStringList sources;
    sources << u"video*"_s;
    _sourceCB->addItems(QDir(u"/dev/"_s).entryList(sources, QDir::System).replaceInStrings(QRegularExpression(u"^"_s), u"v4l2:///dev/"_s));
#endif

    connect(_playPB, &QPushButton::clicked, this, [this, _playPB, _sourceCB, _videoWidget](bool b) {
        if (b) {
            if (_mediaplayer.playbackState() != QMediaPlayer::PausedState) {
                QString source = _sourceCB->currentText();
                _mediaplayer.setSource(QUrl(source));
            }
            _playPB->setIcon(QIcon::fromTheme(u"media-playback-pause"_s, style()->standardIcon(QStyle::SP_MediaPause)));
            _mediaplayer.play();
        } else {
            _mediaplayer.pause();
            _playPB->setIcon(QIcon::fromTheme(u"media-playback-start"_s, style()->standardIcon(QStyle::SP_MediaPlay)));
        }
        _videoWidget->setVisible(b);
    });
    connect(&_mediaplayer, &QMediaPlayer::errorChanged, this, &VideoMonitorWidget::handleError);
}

void VideoMonitorWidget::handleError()
{
    const QString errorString = _mediaplayer.errorString();
    QString message = i18n("Error: ");
    if (errorString.isEmpty()) {
        message += u" #"_s + QString::number(int(_mediaplayer.error()));
    } else {
        message += errorString;
    }
    _errorlabel->setText(message);
}
