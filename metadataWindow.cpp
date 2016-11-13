#include <QtGui> 
#include "metadataWindow.h"

QString metadataWindow::base64_encode(QString string){
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}
 
QString metadataWindow::base64_decode(QString string){
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

metadataWindow::metadataWindow(QWidget *parent) : QWidget(parent)
{
    //initialise_decoding_table();

    // Initialize the Track object with empty strings and the like
    initialise_track_object();
    
    this->setupUI();

    // Connect to the metadata pipe
    FILE *fd = fopen(metadata_file, "r");
    if (fd==NULL)
    {
        printf("No metadata file found");
    }

    // Use for actual reads
    pipe = new QTextStream(fd);

    // Use for events when the pipe is filled
    streamReader = new QSocketNotifier(fileno(fd), QSocketNotifier::Read, qApp);
    QObject::connect(streamReader, SIGNAL(activated(int)), this, SLOT(onData()));
    streamReader->setEnabled(true);
}


void metadataWindow::setupUI()
{
    // Create Fonts
    standard_font = new QFont("Courier New");
    standard_font->setPixelSize(12);
    
    em_font = new QFont("Courier New");
    em_font->setPixelSize(12);
    em_font->setItalic(true);
    
    // Add image
    image = new QPixmap(100,100);
    image->fill(QColor("cyan"));
    image_label  = new QLabel(this);
        image_label->setPixmap(*image);

    // Add labels
    title_label = new QLabel("Titre", this);
    title_label->setFont(*standard_font);
    artist_label = new QLabel("Artiste", this);
    artist_label->setFont(*standard_font);
    release_label = new QLabel("Album", this);
    release_label->setFont(*standard_font);
    file_type_label = new QLabel("Fichier MP3", this);
    file_type_label->setFont(*standard_font);
    client_ip_label = new QLabel("En streaming depuis 192.168.1.24", this);
    client_ip_label->setFont(*standard_font);
    
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    QHBoxLayout *hbl = new QHBoxLayout(this);

    QVBoxLayout *vbl = new QVBoxLayout(this);
    vbl->addWidget(title_label);
    vbl->addWidget(artist_label);
    vbl->addWidget(release_label);
    vbl->addWidget(file_type_label);

    hbl->addWidget(image_label);
    hbl->addLayout(vbl);

    main_layout->addLayout(hbl);
    main_layout->addWidget(client_ip_label);

    // Add "quit" button
    //quit_button = new QPushButton("Quit" , this);
    //QObject::connect(quit_button , SIGNAL(clicked()), qApp , SLOT(quit()));
    //vbl->addWidget(quit_button);
}

void metadataWindow::initialise_track_object()
{
    track.title.clear();
    track.artist.clear();
    track.release.clear();
    client_ip.clear();
    client_name.clear();
    file_type.clear();
}

void metadataWindow::updateUI()
{
    cout << "Updating UI\n";
    title_label->setText(QString::fromStdString(track.title));
    artist_label->setText(QString::fromStdString(track.artist));
    release_label->setText(QString::fromStdString(track.release));
    
    if (file_type.length() != 0) {
        file_type_label->setText("Fichier " + QString::fromStdString(file_type));
    } else {
        file_type_label->setText("");
    }
    
    if (client_ip.length() != 0) {
        client_ip_label->setText("En streaming depuis " + QString::fromStdString(client_ip));
        client_ip_label->setFont(*standard_font);
    } else {
        client_ip_label->setText("Pas de streaming en cours");
        client_ip_label->setFont(*em_font);
    }

    image->convertFromImage(track.image);
    image_label->setScaledContents(true);
    image_label->update();

}

void metadataWindow::onData()
{
    emit dataReceived();
}

void metadataWindow::dataReceived()
{
    cout << "\nProcessing new metadata ...\n";

    if (pipe->atEnd()) {
        cout << " > No more data to read ...\n";
        return;
    }

    QString line = pipe->readLine();
    if (line.isEmpty() || line.isNull()) {
        cout << " > Skipping empty line.\n";
        return;
    }

    if (line == "<data encoding=\"base64\">") {
        cout << "How did we get here ???\n";
    }

    const char * message = line.toStdString().c_str();

    uint32_t type, code, length;
    QImage base64_image;
    bool ret_pic = false;

    cout << "Message: [" << message << "]\n";
    int ret = sscanf(message,"<item><type>%8x</type><code>%8x</code><length>%u</length>",&type,&code,&length);
    
    // Get code and type strings
    char typestring[5];
    *(uint32_t*)typestring = htonl(type);
    typestring[4] = 0;
    char codestring[5];
    *(uint32_t*)codestring = htonl(code);
    codestring[4] = 0;

    cout << " > Found tag: " << typestring << " / " << codestring << " of length " << length << "\n";

    if (type > 0 && code > 0 && ret == 3) {
        // now, think about processing the tag.
        // basically, we need to get hold of the base-64 data, if any
        QString payload;

        if (length > 0) {
            cout << " > Extracting data ..." << "\n";

            // get the next line, which should be a data tag
            QString line = pipe->readLine();

            if (line == "<data encoding=\"base64\">") {
                // now, read in that big (possibly) base64 buffer
                cout << " > Data is base64 buffer" << "\n";

                QString line = pipe->readLine();
                if (line.size() > 14) {
                    line.chop(14); // remove "</data></item>"
                    if (code != 'PICT') {
                        payload = base64_decode(line);
                    } else {
                        cout << " > ## trying to create image !\n";
                        //cout << line;
                        QByteArray base64Data = line.toAscii();
                        ret_pic = base64_image.loadFromData(QByteArray::fromBase64(base64Data));
                    }
                } else {
                    cout << " > Looks like a bad payload" << "\n";
                }
            }

            cout << " > Got it decoded. Length of decoded string is " << payload.size() << " bytes.\n";
        }
       
        // this has more information about tags, which might be relevant:
        // https://code.google.com/p/ytrack/wiki/DMAP
        if (code == 'asal') {
            track.release = payload.toStdString();
            cout << "Album Name: " << payload.toStdString() << "\n";
        } else if (code == 'asar') {
            track.artist = payload.toStdString();
            cout << "Artist: " << payload.toStdString() << "\n";
        } else if (code == 'minm') {
            track.title = payload.toStdString();
            cout << "Title: " << payload.toStdString() << "\n";
        } else if (code == 'asdt') {
            file_type = payload.toStdString();
            cout << "File kind: " << payload.toStdString() << "\n";
        } else if (code == 'PICT' && ret_pic) {
            track.image = base64_image;
            cout << "Picture received, length " << length << " bytes." << "\n";
        } else if (code == 'clip') {
            client_ip = payload.toStdString();
            cout << "Client's IP: " << payload.toStdString() << "\n";
        } else if (code == 'snam') {
            client_name = payload.toStdString();
            cout << "Device Name: " << payload.toStdString() << "\n";
        } else if (code == 'snua') {
            client_name = payload.toStdString();
            cout << "User Agent: " << payload.toStdString() << "\n";
        } else if (code == 'prms' || code =='pbeg') {
            track.playing = true;
            cout << "Started playing" << "\n";
        } else if (code == 'pend') {
            track.playing = false;
            cout << "Stopped playing" << "\n";
        } else if (type=='ssnc') {
            cout << "SSNC Stuff : " << typestring << "/" << codestring << " : " << payload.toStdString() << "\n";
        } else {
            cout << "Other Stuff : " << typestring << "/" << codestring << " : " << payload.toStdString() << "\n";
        }

    } else {
        cout << "Could not decipher the message.\n";;
    }
    
    pipe->flush();
    updateUI();
}
