#include <QtGui> 
#include "metadataWindow.h"

void initialise_decoding_table() {
    size_t i;
    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

// pass in a pointer to the data, its length, a pointer to the output buffer and a pointer to an int containing its maximum length
// the actual length will be returned.

char *base64_encode(const unsigned char *data,
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
int base64_decode(const char *data,
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

metadataWindow::metadataWindow(QWidget *parent) : QWidget(parent)
{
    initialise_decoding_table();

    // Initialize the Track object with empty strings and the like
    initialise_track_object();
    
    this->setupUI();

    // Connect to STDIN signal
    FILE* f = fopen("/tmp/shairport-sync-metadata", "r");
    QSocketNotifier *streamReader = new QSocketNotifier(fileno(f), QSocketNotifier::Read, qApp);
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
    QTextStream qin(stdin);
    QString line = qin.readLine();
    emit dataReceived(line);
}

void metadataWindow::dataReceived(QString message)
{
    uint32_t type,code,length;
    char tagend[1024];
    int ret = sscanf(message.toAscii().data(),"<item><type>%8x</type><code>%8x</code><length>%u</length>",&type,&code,&length);
    if (ret==3) {
        // now, think about processing the tag.
        // basically, we need to get hold of the base-64 data, if any
        size_t outputlength=0;
        char payload[32769];
//        if (length>0) {
//            // get the next line, which should be a data tag
//            char datatagstart[64],datatagend[64];
//            memset(datatagstart,0,64);
//            int rc = fscanf(stdin,"<data encoding=\"base64\">");
//            if (rc==0) {
//                // now, read in that big (possibly) base64 buffer
//                int c = fgetc(stdin);
//                uint32_t b64size = 4*((length+2)/3);
//                char * b64buf = malloc(b64size+1);
//                memset(b64buf,0,b64size+1);
//                if (b64buf) {
//                    if (fgets(b64buf, b64size+1, stdin)!=NULL) {
//                        // it looks like we got it
//                        // printf("Looks like we got it, with a buffer size of %u.\n",b64size);
//                        //puts(b64buf);
//                        //printf("\n");
//                        // now, if it's not a picture, let's try to decode it.
//                        if (code!='PICT') {
//                            int inputlength=32678;
//                            if (b64size<inputlength)
//                                inputlength=b64size;
//                            outputlength=32768;
//                            if (base64_decode(b64buf,inputlength,payload,&outputlength)!=0) {
//                                printf("Failed to decode it.\n");
//                            }
//                        }
//                    }
//                    free(b64buf);
//                } else {
//                    printf("couldn't allocate memory for base-64 stuff\n");
//                }
//                rc = fscanf(stdin,"%64s",datatagend);
//                if (strcmp(datatagend,"</data></item>")!=0)
//                    printf("End data tag not seen, \"%s\" seen instead.\n",datatagend);
//            }
//        }
//        
//        // printf("Got it decoded. Length of decoded string is %u bytes.\n",outputlength);
//        payload[outputlength]=0;
        
        // this has more information about tags, which might be relevant:
        // https://code.google.com/p/ytrack/wiki/DMAP
        switch (code) {
            case 'asal':
                track.release = payload;
                printf("Album Name: \"%s\".\n",payload);
                break;
            case 'asar':
                track.artist = payload;
                printf("Artist: \"%s\".\n",payload);
                break;
            case 'minm':
                track.title = payload;
                printf("Title: \"%s\".\n",payload);
                break;
            case 'asdt':
                file_type = payload;
                printf("File kind: \"%s\".\n",payload);
                break;
            case 'PICT':
                printf("Picture received, length %u bytes.\n",length);
                break;
            case 'clip':
                client_ip = payload;
                printf("Client's IP: \"%s\".\n",payload);
                break;
            case 'snam':
                client_name = payload;
                printf("Device Name: \"%s\".\n",payload);
                break;
            case 'snua':
                client_name = payload;
                printf("User Agent: \"%s\".\n",payload);
                break;
            default: if (type=='ssnc') {
                char typestring[5];
                *(uint32_t*)typestring = htonl(type);
                typestring[4]=0;
                char codestring[5];
                *(uint32_t*)codestring = htonl(code);
                codestring[4]=0;
                printf("\"%s\" \"%s\": \"%s\".\n",typestring,codestring,payload);
            }
        }
    } else {
        printf("\nXXX Could not decipher: \"%s\".\n",message.toAscii().data());
    }
    
    updateUI();
}
