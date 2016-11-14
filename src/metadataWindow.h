#pragma once

#ifndef METADATAWINDOW_H
#define METADATAWINDOW_H

#include <QtGui>

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>

using std::string;
using std::cout;

struct Track {
    string title;
    string artist;
    string release;
    QImage image;
    bool changeImage;
    bool resetImage;
    bool playing;
    bool pending;
};

class metadataWindow : public QWidget
{
  Q_OBJECT

  public:
    metadataWindow(QWidget *parent = 0);

  private slots:
    void onData();

  private:
    static const char *metadata_file;

    QString base64_encode(QString string);
    QString base64_decode(QString string);

    void setupUI();
    void updateUI();

    void initialise_track_object();
    void dataReceived();

    QTextStream *pipe;
    QSocketNotifier *streamReader;

    QLabel *title_label;
    QLabel *artist_label;
    QLabel *release_label;
    QLabel *file_type_label;
    QLabel *status_label;
    QLabel *status_label_icon;

    QFont* standard_font;
    QFont* title_font;
    QFont* title_em_font;
    QFont* em_font;

    QLabel *image_label;
    QPixmap *image;
    QPixmap *default_image;
    QSize *size;

    Track track;

    string client_ip;
    string client_name;
};

#endif