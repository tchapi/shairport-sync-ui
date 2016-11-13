#pragma once

#ifndef METADATAWINDOW_H
#define METADATAWINDOW_H

#include <QtGui>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

using std::string;

struct Track {
    string title;
    string artist;
    string release;
    double progress; // between 0 and 1
};

// From Stack Overflow, with thanks:
// http://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
// minor mods to make independent of C99.
// more significant changes make it not malloc memory
// needs to initialise the docoding table first
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'};
static int decoding_table[256]; // an incoming char can range over ASCII, but by mistake could be all 8 bits.

static int mod_table[] = {0, 2, 1};

class metadataWindow : public QWidget
{
  Q_OBJECT

  public:
    metadataWindow(QWidget *parent = 0);

  private slots:
    void onData();

  private:
    void setupUI();
    void updateUI();

    void initialise_track_object();
    void dataReceived(QString message);

    QSocketNotifier *streamReader;
    
    QLabel *title_label;
    QLabel *artist_label;
    QLabel *release_label;
    QLabel *file_type_label;
    QLabel *client_ip_label;

    QFont* standard_font;
    QFont* em_font;

    QPushButton *quit_button;

    Track track;

    string client_ip;
    string client_name;
    string file_type;
};

#endif