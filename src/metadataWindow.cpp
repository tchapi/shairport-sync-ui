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

static void SetTextToLabel(QLabel *label, QString text)
{
    QFontMetrics metrix(label->font());
    int width = label->width() - 10;
    QString clippedText = metrix.elidedText(text, Qt::ElideRight, width);
    label->setText(clippedText);
}

metadataWindow::metadataWindow(QWidget *parent) : QWidget(parent)
{
    const char *metadata_file = "/tmp/shairport-sync-metadata";

    // Initialize the Track object with empty strings and the like
    initialise_track_object();
    
    this->setupUI();

    // Connect to the metadata pipe
    FILE *fd = fopen(metadata_file, "r");
    if (fd==NULL)
    {
        cout << "No metadata file found, testing UI." << "\n";
    } else {
        // Use for actual reads
        pipe = new QTextStream(fd);

        // Use for events when the pipe is filled
        streamReader = new QSocketNotifier(fileno(fd), QSocketNotifier::Read, qApp);
        QObject::connect(streamReader, SIGNAL(activated(int)), this, SLOT(onData()));
        streamReader->setEnabled(true);

        // Now disable cursor
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }
}


void metadataWindow::setupUI()
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    // Create Fonts
    standard_font = new QFont("Droid Sans");
    standard_font->setPixelSize(14);

    title_font = new QFont("Droid Sans");
    title_font->setPixelSize(22);
    
    title_em_font = new QFont("Droid Sans");
    title_em_font->setPixelSize(22);
    title_em_font->setItalic(true);
    
    em_font = new QFont("Droid Sans");
    em_font->setPixelSize(16);
    em_font->setItalic(true);
    
    // Add cover image
    size = new QSize(120,120);
    image = new QPixmap(":/images/default_cover");
    image_label  = new QLabel();
        image_label->setPixmap(*image);
        image_label->setFixedHeight(size->height());
        image_label->setFixedWidth(size->width());

    // Add labels
    QPixmap note(":/icons/note");
    QLabel *title_label_icon = new QLabel();
    title_label_icon->setPixmap(note);
    title_label_icon->setFixedWidth(20);

    title_label = new QLabel("N/A");
    title_label->setFont(*title_em_font);
    title_label->setFixedHeight(50);
    title_label->setStyleSheet("background-color: #123456; padding-right:6px;");

    QHBoxLayout *title_label_layout = new QHBoxLayout();
    title_label_layout->addWidget(title_label_icon);
    title_label_layout->addWidget(title_label);

    // Artist widget
    QPixmap artist(":/icons/artist");
    QLabel *artist_label_icon = new QLabel();
    artist_label_icon->setPixmap(artist);
    artist_label_icon->setFixedWidth(20);

    artist_label = new QLabel("N/A");
    artist_label->setFont(*em_font);
    artist_label->setWordWrap(true);
    artist_label->setStyleSheet("background-color: #35467A; padding-right:6px;");

    QHBoxLayout *artist_label_layout = new QHBoxLayout();
    artist_label_layout->addWidget(artist_label_icon);
    artist_label_layout->addWidget(artist_label);

    // Album widget
    QPixmap cd(":/icons/cd");
    QLabel *release_label_icon = new QLabel();
    release_label_icon->setPixmap(cd);
    release_label_icon->setFixedWidth(20);

    release_label = new QLabel("N/A");
    release_label->setFont(*em_font);
    release_label->setWordWrap(true);
    release_label->setStyleSheet("background-color: #C593E2; padding-right:6px;");

    QHBoxLayout *release_label_layout = new QHBoxLayout();
    release_label_layout->addWidget(release_label_icon);
    release_label_layout->addWidget(release_label);

    // Status bar
    QMovie *load = new QMovie(":/icons/load");
    status_label_icon = new QLabel();
    status_label_icon->setMovie(load);
    load->start();
    status_label_icon->setFixedWidth(20);

    status_label = new QLabel("En attente de données ...");
    status_label->setStyleSheet("background-color: #177AC3; padding-right:6px;");
    status_label->setFixedHeight(30);
    status_label->setFont(*standard_font);

    QHBoxLayout *status_label_layout = new QHBoxLayout();
    status_label_layout->addWidget(status_label_icon);
    status_label_layout->addWidget(status_label);


    // Make the whole layout
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(2,12,2,12);

    QHBoxLayout *hbl = new QHBoxLayout();

    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->addLayout(artist_label_layout);
    vbl->addLayout(release_label_layout);

    hbl->addWidget(image_label);
    hbl->addLayout(vbl);

    main_layout->addLayout(title_label_layout);
    main_layout->addLayout(hbl);
    main_layout->addLayout(status_label_layout);

    this->setLayout(main_layout);
}

void metadataWindow::initialise_track_object()
{
    track.title.clear();
    track.artist.clear();
    track.release.clear();
    track.playing = false;
    client_ip.clear();
    client_name.clear();
}

void metadataWindow::updateUI()
{
    if (track.title.empty() || track.playing == false) {
        title_label->setFont(*title_em_font);
        title_label->setText("N/A");
    } else {
        title_label->setFont(*title_font);
        SetTextToLabel(title_label,QString::fromStdString(track.title));
    }

    if (track.artist.empty() || track.playing == false) {
        artist_label->setFont(*em_font);
        artist_label->setText("N/A");
    } else {
        artist_label->setText(QString::fromStdString(track.artist));
        artist_label->setFont(*standard_font);
    }

    if (track.release.empty() || track.playing == false) {
        release_label->setFont(*em_font);
        release_label->setText("N/A");
    } else {
        release_label->setText(QString::fromStdString(track.release));
        release_label->setFont(*standard_font);
    }
    
    if (client_name.length() != 0 && track.playing) {
        status_label->setFont(*standard_font);
        SetTextToLabel(status_label, "Lecture depuis " + QString::fromStdString(client_name));
        QPixmap device(":/icons/yt");
        status_label_icon->setPixmap(device);
    } else if (client_name.length() != 0) {
        status_label->setFont(*standard_font);
        SetTextToLabel(status_label, "En pause depuis " + QString::fromStdString(client_name));
        QPixmap device(":/icons/yt");
        status_label_icon->setPixmap(device);
    } else if (client_ip.length() != 0 && track.playing) {
        status_label->setFont(*standard_font);
        SetTextToLabel(status_label, "Lecture depuis " + QString::fromStdString(client_ip));
        QPixmap device(":/icons/device");
        status_label_icon->setPixmap(device);
    } else if (client_ip.length() != 0) {
        status_label->setFont(*standard_font);
        SetTextToLabel(status_label, "En pause depuis " + QString::fromStdString(client_ip));
        QPixmap device(":/icons/device");
        status_label_icon->setPixmap(device);
    } else if (track.playing) {
        status_label->setFont(*standard_font);
        status_label->setText("Lecture en cours");
        QPixmap collection(":/icons/collection");
        status_label_icon->setPixmap(collection);
    } else {
        status_label->setFont(*em_font);
        status_label->setText("Pas de streaming en cours");
        QPixmap load_full(":/icons/load_full");
        status_label_icon->setPixmap(load_full);
    }

    if (track.playing && !track.image.isNull()) {
        image->convertFromImage(track.image);
        *image = image->scaled(*size, Qt::KeepAspectRatioByExpanding);
        image_label->setPixmap(*image);
    } else {
        image = new QPixmap(":/images/default_cover");
        image_label->setPixmap(*image);
    }

    image_label->update();

}

void metadataWindow::onData()
{
    emit dataReceived();
}

void metadataWindow::dataReceived()
{
    //cout << "\nProcessing new metadata ...\n";

    if (pipe->atEnd()) {
        //cout << " > No more data to read ...\n";
        return;
    }

    QString line = pipe->readLine();
    if (line.isEmpty() || line.isNull()) {
        //cout << " > Skipping empty line.\n";
        return;
    }

    if (line == "<data encoding=\"base64\">") {
        cout << "How did we get here ???\n";
    }

    const char * message = line.toStdString().c_str();

    uint32_t type, code, length;
    QImage base64_image;
    bool ret_pic = false;

    //cout << "Message: [" << message << "]\n";
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
            //cout << " > Extracting data ..." << "\n";

            // get the next line, which should be a data tag
            QString line = pipe->readLine();

            if (line == "<data encoding=\"base64\">") {
                // now, read in that big (possibly) base64 buffer
                //cout << " > Data is base64 buffer" << "\n";

                QString line = pipe->readLine();
                if (line.size() > 14) {
                    line.chop(14); // remove "</data></item>"
                    if (code != 'PICT') {
                        payload = base64_decode(line);
                    } else {
                        QByteArray base64Data = line.toAscii();
                        ret_pic = base64_image.loadFromData(QByteArray::fromBase64(base64Data));
                    }
                } else {
                    //cout << " > Looks like a bad payload" << "\n";
                }
            }

            //cout << " > Got it decoded. Length of decoded string is " << payload.size() << " bytes.\n";
        }
       
        // this has more information about tags, which might be relevant:
        // https://code.google.com/p/ytrack/wiki/DMAP
        if (code == 'asal') {
            track.release = payload.toStdString();
            //cout << "Album Name: " << payload.toStdString() << "\n";
        } else if (code == 'asar' || (code == 'asaa' && !payload.isEmpty())) {
            track.artist = payload.toStdString();
            //cout << "Artist: " << payload.toStdString() << "\n";
        } else if (code == 'minm') {
            track.title = payload.toStdString();
            //cout << "Title: " << payload.toStdString() << "\n";
        } else if (code == 'PICT' && ret_pic) {
            track.image = base64_image;
            //cout << "Picture received, length " << length << " bytes." << "\n";
        } else if (code == 'clip') {
            client_ip = payload.toStdString();
            //cout << "Client's IP: " << payload.toStdString() << "\n";
        } else if (code == 'snam') {
            client_name = payload.toStdString();
            //cout << "Device Name: " << payload.toStdString() << "\n";
        } else if (code == 'snua') {
            client_name = payload.toStdString();
            //cout << "User Agent: " << payload.toStdString() << "\n";
        } else if (code == 'prsm' || code =='pbeg') {
            track.playing = true;
            //cout << "Started playing" << "\n";
        } else if (code == 'pend') {
            track.playing = false;
            //cout << "Stopped playing" << "\n";
        } else if (type=='ssnc') {
            //cout << "SSNC Stuff : " << typestring << "/" << codestring << " : " << payload.toStdString() << "\n";
        } else {
            //cout << "Other Stuff : " << typestring << "/" << codestring << " : " << payload.toStdString() << "\n";
        }

    } else {
        //cout << "Could not decipher the message.\n";;
    }
    
    pipe->flush();
    updateUI();
}
