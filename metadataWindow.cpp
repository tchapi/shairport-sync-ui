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
/*
void metadataWindow::initialise_decoding_table() {
    size_t i;
    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

// pass in a pointer to the data, its length, a pointer to the output buffer and a pointer to an int containing its maximum length
// the actual length will be returned.

char *metadataWindow::base64_encode(const unsigned char *data,
                    size_t input_length,
                    char *encoded_data,
                    size_t *output_length) {
    
    size_t calculated_output_length = 4 * ((input_length + 2) / 3);
    if (calculated_output_length> *output_length)
        return(NULL);
    *output_length = calculated_output_length;
    
    size_t i,j;
    for (i = 0, j = 0; i < input_length;) {
        
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;
        
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    
    int k;
    for (k = 0; k < mod_table[input_length % 3]; k++)
        encoded_data[*output_length - 1 - k] = '=';
    
    return encoded_data;
}


// pass in a pointer to the data, its length, a pointer to the output buffer and a pointer to an int containing its maximum length
// the actual length will be returned.
int metadataWindow::base64_decode(const char *data,
                  size_t input_length,
                  unsigned char *decoded_data,
                  size_t *output_length) {
    
    //remember somewhere to call initialise_decoding_table();
    
    if (input_length % 4 != 0) return -1;
    
    size_t calculated_output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') calculated_output_length--;
    if (data[input_length - 2] == '=') calculated_output_length--;
    if (calculated_output_length> *output_length)
        return(-1);
    *output_length = calculated_output_length;
    
    size_t i,j;
    for (i = 0, j = 0; i < input_length;) {
        
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        
        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);
        
        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    
    return 0;
}
*/
metadataWindow::metadataWindow(QWidget *parent) : QWidget(parent)
{
    //initialise_decoding_table();

    // Initialize the Track object with empty strings and the like
    initialise_track_object();
    
    this->setupUI();

    // Connect to the metadata pipe
    fd = fopen("/tmp/shairport-sync-metadata", "r");
    if (fd==NULL)
    {
        printf("No metadata file found");
    }

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
    
    QVBoxLayout *vbl = new QVBoxLayout(this);
    vbl->addWidget(title_label);
    vbl->addWidget(artist_label);
    vbl->addWidget(release_label);
    vbl->addWidget(file_type_label);
    vbl->addWidget(client_ip_label);

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
    track.progress = -1; // No metadata
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
}

void metadataWindow::onData()
{
    QTextStream qin(fd);
    emit dataReceived(&qin);
}

void metadataWindow::dataReceived(QTextStream *qin)
{
    cout << "\nProcessing new metadata ...\n";
    QString line = qin->readLine();
    const char * message = line.toStdString().c_str();

    uint32_t type,code,length;
    //char tagend[1024];
    cout << "Message: " << message << "\n";
    int ret = sscanf(message,"<item><type>%8x</type><code>%8x</code><length>%u</length>",&type,&code,&length);
    
    // Get code and type strings
    char typestring[5];
    *(uint32_t*)typestring = htonl(type);
    typestring[4] = 0;
    char codestring[5];
    *(uint32_t*)codestring = htonl(code);
    codestring[4] = 0;

    cout << " > Found tag: " << typestring << " / " << codestring << " of length " << length << "\n";

    if (ret == 3) {
        // now, think about processing the tag.
        // basically, we need to get hold of the base-64 data, if any
        QString payload;

        if (length > 0) {
            cout << " > Extracting data ..." << "\n";

            // get the next line, which should be a data tag
            QString line = qin->readLine();

            cout << " > Got line : " << line.toStdString() << "\n";

            if (line == "<data encoding=\"base64\">") {
                // now, read in that big (possibly) base64 buffer
                cout << " > Data is base64 buffer" << "\n";

                QString line = qin->readLine();
                if (line.size() > 14) {
                    line.chop(14); // remove "</data></item>"
                    if (code != 'PICT') {
                        payload = base64_decode(line);
                    }
                } else {
                    cout << " > Looks like a bad payload" << "\n";
                }

               // int c = fgetc(fd);
               // uint32_t b64size = 4*((length+2)/3);
               // char * b64buf = new char[b64size+1]; //malloc(b64size+1);
               // memset(b64buf,0,b64size+1);
               // if (b64buf) {
                   //if (fgets(b64buf, b64size+1, fd)!=NULL) {
                       // it looks like we got it
                       //printf(" > Looks like we got it, with a buffer size of %u.\n",b64size);
                       //puts(b64buf);
                       //printf("\n");
                       // now, if it's not a picture, let's try to decode it.
                       // if (code != 'PICT') {
                       //     int inputlength=32678;
                       //     if (b64size<inputlength)
                       //         inputlength=b64size;
                       //     outputlength=32768;
                       //     if (base64_decode(b64buf,inputlength,payload,&outputlength)!=0) {
                       //         printf(" > Failed to decode it.\n");
                       //     }
                       // }
                   // }
                   // free(b64buf);
               // } else {
               //     cout << "Couldn't allocate memory for base-64 stuff\n";
               // }
               // rc = fscanf(fd,"%64s",datatagend);
               // if (strcmp(datatagend,"</data></item>")!=0)
               //     cout << "End data tag not seen, " << datatagend << " seen instead.\n";
           }
        }
       
        cout << " > Got it decoded. Length of decoded string is " << payload.size() << " bytes.\n";

        //payload[outputlength] = 0;
        
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
        } else if (code == 'PICT') {
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
        } else if (type=='ssnc') {
            cout << "SSNC Stuff : " << typestring << codestring << " : " << payload.toStdString();
        }

    } else {
        cout << "Could not decipher the message.\n";;
    }
    
    updateUI();
}
