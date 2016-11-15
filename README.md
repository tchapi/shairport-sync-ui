# Shairport-sync UI
A simple metadata UI for [shairport-sync](https://github.com/mikebrady/shairport-sync), in C++ using Qt4.

See [this article](http://www.foobarflies.io/shairport-is-back) for further reference.

_Based on [shairport-sync-metadata-reader](https://github.com/mikebrady/shairport-sync-metadata-reader) by [Mike Brady](https://github.com/mikebrady)_

![The interface compiled on Mac Os X](https://github.com/tchapi/shairport-sync-ui/blob/master/screenshots/on_os_x.png)

## Compiling

You need to create a Makefile for your specific device with `qmake`, and then build the program:

```
qmake
make
```

Or simply :

```
./compile.sh && ./run-gui.sh
```

### Codes

I created a readable list of all DMAP and DAAP codes in markdown format here : [DMAP_DAAP_Codes.md](https://github.com/tchapi/shairport-sync-ui/blob/master/DMAP_DAAP_Codes.md)

## License

MIT.