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

static const char *metadata_file = "/tmp/shairport-sync-metadata";

struct Track {
    string title;
    string artist;
    string release;
    QImage image;
    bool playing;
};

class metadataWindow : public QWidget
{
  Q_OBJECT

  public:
    metadataWindow(QWidget *parent = 0);

  private slots:
    void onData();

  private:
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
    QLabel *client_ip_label;

    QFont* standard_font;
    QFont* em_font;

    QLabel *image_label;
    QPixmap *image;
    QSize *size;

    Track track;

    string client_ip;
    string client_name;
    string file_type;
};

#endif