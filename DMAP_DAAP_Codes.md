# DMAP / DAAP Codes

I list here the DMAP and DAAP codes in a readable table format. Not all are useful. `minm` seems to correspond to the song title in a consistent fashion, whereas `asal` and `asar` are used for song album and artist respectively.

Shairport-sync emits other `sncc` codes too, see : https://github.com/mikebrady/shairport-sync-metadata-reader/blob/master/README.md

Overall source : https://code.google.com/archive/p/ytrack/wikis/DMAP.wiki

### DMAP codes (Media + technical data)

| tag name | full name | type |
|:-------------|:--------------|:---------|
|miid |dmap.itemid |5 |
|minm |dmap.itemname |9 |
|mikd |dmap.itemkind |7 |
|mper |dmap.persistentid|9 |
|mcon |dmap.container |12 |
|mcti |dmap.containeritemid|5 |
|mpco |dmap.parentcontainerid|5 |
|mstt |dmap.status |5 |
|msts |dmap.statusstring|9 |
|mimc |dmap.itemcount |5 |
|mctc |dmap.containercount|5 |
|mrco |dmap.returnedcount|5 |
|mtco |dmap.specifiedtotalcount|5 |
|f?ch |dmap.haschildcontainers|1 |
|mlcl |dmap.listing |12 |
|mlit |dmap.listingitem|12 |
|mbcl |dmap.bag |12 |
|mdcl |dmap.dictionary|12 |
|msrv |dmap.serverinforesponse|12 |
|msau |dmap.authenticationmethod|1 |
|msas |dmap.authenticationschemes|5 |
|mslr |dmap.loginrequired|1 |
|mpro |dmap.protocolversion|11 |
|msal |dmap.supportsautologout|1 |
|msup |dmap.supportsupdate|1 |
|mspi |dmap.supportspersistentids|1 |
|msex |dmap.supportsextensions|1 |
|msbr |dmap.supportsbrowse|1 |
|msqy |dmap.supportsquery|1 |
|msix |dmap.supportsindex|1 |
|msrs |dmap.supportsresolve|1 |
|mstm |dmap.timeoutinterval|5 |
|msdc |dmap.databasescount|5 |
|mstc |dmap.utctime |10 |
|msto |dmap.utcoffset |6 |
|mlog |dmap.loginresponse|12 |
|mlid |dmap.sessionid |5 |
|mupd |dmap.updateresponse|12 |
|musr |dmap.serverrevision|5 |
|muty |dmap.updatetype|1 |
|mudl |dmap.deletedid |12 |
|msdc |dmap.databasescount|5 |
|mccr |dmap.contentcodesresponse|12 |
|mcnm |dmap.contentcodesnumber|5 |
|mcna |dmap.contentcodesname|9 |
|mcty |dmap.contentcodestype|3 |
|meds |dmap.editcommandssupported|5 |

### DAAP codes (Track related stuff)

| tag name | full name | type |
|:-------------|:--------------|:---------|
|ated |daap.supportsextradata|3 |
|apro |daap.protocolversion|11 |
|avdb |daap.serverdatabases|12 |
|abro |daap.databasebrowse|12 |
|adbs |daap.databasesongs|12 |
|aply |daap.databaseplaylists|12 |
|apso |daap.playlistsongs|12 |
|arsv |daap.resolve |12 |
|arif |daap.resolveinfo|12 |
|abal |daap.browsealbumlisting|12 |
|abar |daap.browseartistlisting|12 |
|abcp |daap.browsecomposerlisting|12 |
|abgn |daap.browsegenrelisting|12 |
|aePP |com.apple.itunes.is-podcast-playlist|1 |
|asal |daap.songalbum |9 |
|asar |daap.songartist|9 |
|asbr |daap.songbitrate|3 |
|ascm |daap.songcomment|9 |
|asco |daap.songcompilation|1 |
|ascp |daap.songcomposer|9 |
|asda |daap.songdateadded|10 |
|asdm |daap.songdatemodified|10 |
|asdc |daap.songdisccount|3 |
|asdn |daap.songdiscnumber|3 |
|aseq |daap.songeqpreset|9 |
|asgn |daap.songgenre |9 |
|asdt |daap.songdescription|9 |
|asrv |daap.songrelativevolume|2 |
|assr |daap.songsamplerate|5 |
|assz |daap.songsize |5 |
|asst |daap.songstarttime|5 |
|assp |daap.songstoptime|5 |
|astm |daap.songtime |5 |
|astc |daap.songtrackcount|3 |
|astn |daap.songtracknumber|3 |
|asur |daap.songuserrating|1 |
|asyr |daap.songyear |3 |
|asfm |daap.songformat|9 |
|asdb |daap.songdisabled|1 |
|asdk |daap.songdatakind|1 |
|asul |daap.songdataurl|9 |
|asbt |daap.songbeatsperminute|3 |
|abpl |daap.baseplaylist|1 |
|agrp |daap.songgrouping|9 |
|ascd |daap.songcodectype|5 |
|ascs |daap.songcodecsubtype|5 |
|apsm |daap.playlistshufflemode|1 |
|aprm |daap.playlistrepeatmode|1 |
|asct |daap.songcategory|9 |
|ascn |daap.songcontentdescription|9 |
|aslc |daap.songlongcontentdescription|9 |
|asky |daap.songkeywords|9 |
|ascr |daap.songcontentrating|1 |
|asgp |daap.songgapless|1 |
|ased |daap.songextradata|" |
|asdr |daap.songdatereleased|10 |
|asdp |daap.songdatepurchased|10 |
|ashp |daap.songhasbeenplayed|1 |
|assn |daap.sortname |9 |
|assa |daap.sortartist|9 |
|assl |daap.sortalbumartist|9 |
|assu |daap.sortalbum |9 |
|assc |daap.sortcomposer|9 |
|asss |daap.sortseriesname|9 |
|asbk |daap.bookmarkable|1 |
|asbo |daap.songbookmark|5 |
|aspu |daap.songpodcasturl|9 |
|asai |daap.songalbumid|7 |
|asls |daap.songlongsize|7 |
|asaa |daap.songalbumartist|9 |

### iTunes specific codes

| tag name | full name | type |
|:-------------|:--------------|:---------|
|aeNV |com.apple.itunes.norm-volume|5 |
|aeSP |com.apple.itunes.smart-playlist|1 |
|aeSI |com.apple.itunes.itms-songid|5 |
|aeAI |com.apple.itunes.itms-artistid|5 |
|aePI |com.apple.itunes.itms-playlistid|5 |
|aeCI |com.apple.itunes.itms-composerid|5 |
|aeGI |com.apple.itunes.itms-genreid|5 |
|aeSF |com.apple.itunes.itms-storefrontid|5 |
|aePC |com.apple.itunes.is-podcast|1 |
|aeHV |com.apple.itunes.has-video|1 |
|aeMK |com.apple.itunes.mediakind|1 |
|aeSN |com.apple.itunes.series-name|9 |
|aeNN |com.apple.itunes.network-name|9 |
|aeEN |com.apple.itunes.episode-num-str|9 |
|aeES |com.apple.itunes.episode-sort|5 |
|aeSU |com.apple.itunes.season-num|5 |
|aeGH |com.apple.itunes.gapless-heur|5 |
|aeGD |com.apple.itunes.gapless-enc-dr|5 |
|aeGU |com.apple.itunes.gapless-dur|7 |
|aeGR |com.apple.itunes.gapless-resy|7 |
|aeGE |com.apple.itunes.gapless-enc-del|5 |
|???? |com.apple.itunes.req-fplay|1 |
|aePS |com.apple.itunes.special-playlist|1 |
|aeCR |com.apple.itunes.content-rating|9 |
|aeSG |com.apple.itunes.saved-genius|1 |
|aeHD |com.apple.itunes.is-hd-video|1 |
|aeSE |com.apple.itunes.store-pers-id|7 |
|aeDR |com.apple.itunes.drm-user-id|7 |
|aeND |com.apple.itunes.non-drm-user-id|7 |
|aeK1 |com.apple.itunes.drm-key1-id|7 |
|aeK2 |com.apple.itunes.drm-key2-id|7 |
|aeDV |com.apple.itunes.drm-versions|5 |
|aeDP |com.apple.itunes.drm-platform-id|5 |
|aeXD |com.apple.itunes.xid|9 |
|aeMk |com.apple.itunes.extended-media-kind|5 |
|aeAD |com.apple.itunes.adam-ids-array|12 |
|aeSV |com.apple.itunes.music-sharing-version|5 |

### Jukebox specific codes

| tag name | full name | type |
|:-------------|:--------------|:---------|
|ceJV |com.apple.itunes.jukebox-vote|5 |
|ceJC |com.apple.itunes.jukebox-client-vote|2 |
|ceJS |com.apple.itunes.jukebox-score|4 |
|ceJI |com.apple.itunes.jukebox-current|5 |

### Types

| type | name | description |
|:---------|:---------|:----------------|
|1 |char | 1-byte value, can be used as a boolean (true if the value is present, false if not)|
|3 | short | 2-byte integer |
|5 | long | 4-byte integer |
|7 | long long | 8-byte integer, tends to be represented in hex rather than numerical form |
|9 | string | string of characters (UTF-8) |
|10 | date | 4-byte integer with seconds since 1970 (standard UNIX time format) |
|11 | version | 2-bytes major version, next byte minor version, last byte patchlevel |
|12 | container | contains a series of other chunks, one after the other |