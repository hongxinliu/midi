# MIDI

This project was inspired by [Patrik's piano show](https://www.youtube.com/watch?v=zKroWMAIFYA&list=RDMMzKroWMAIFYA&start_radio=1) and learned from [kosua20's nice work](https://github.com/kosua20/MIDIVisualizer).

## Features
* MIDI file I/O (only input for now)
* MIDI to readable proto conversion
* MIDI visualization

![midi_show](https://res.cloudinary.com/dal4petdi/image/upload/v1583660025/Blog/midi.gif)

Demo show made from this project: [https://www.bilibili.com/video/av94792318/](https://www.bilibili.com/video/av94792318/)

## Dependencies
* Qt5
* OpenGL
* Protobuf
* Boost

## Build
```bash
git clone https://github.com/hongxinliu/midi.git
cd midi
./build.sh
```

Note that, if Qt5 is not found, set Qt5_DIR to where your Qt5 installed.
