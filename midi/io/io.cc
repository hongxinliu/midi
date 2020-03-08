#include "midi/io/io.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace midi {

// last valid midi event status
static uint8_t s_last_midi_status = 0;

inline std::vector<uint8_t> readBytes(std::istream &is,
                                      const size_t num_bytes) {
  if (num_bytes == 0) {
    return {};
  }
  std::vector<uint8_t> data(num_bytes);
  is.read((char *)(data.data()), num_bytes);
  return data;
}

template <typename T>
T as(const std::vector<uint8_t> &data) {
  return *(reinterpret_cast<const T *>(data.data()));
}

inline uint16_t swapEndian(const uint16_t number) {
  return ((number & 0xff) << 8) | ((number & 0xff00) >> 8);
}

inline uint32_t swapEndian(const uint32_t number) {
  return ((number & 0xff) << 24) | ((number & 0xff00) << 8) |
         ((number & 0xff0000) >> 8) | ((number & 0xff000000) >> 24);
}

inline uint32_t readVariableLengthQuantity(std::istream &is) {
  uint32_t quantity = 0;
  uint8_t byte;
  do {
    byte = as<uint8_t>(readBytes(is, 1));
    quantity = (quantity << 7) + (byte & 0x7f);
  } while (byte & 0x80);
  return quantity;
}

static bool loadHeader(std::istream &is, Header *header) {
  // type: 4 bytes
  const auto type = readBytes(is, 4);
  const auto type_str = std::string(type.cbegin(), type.cend());
  if (type_str != "MThd") {
    std::cout << "Type of header is invalid: " << type_str << std::endl;
    return false;
  }

  // length: 4 bytes
  auto length = swapEndian(as<uint32_t>(readBytes(is, 4)));
  if (length != 6) {
    std::cout << "Invalid header length: " << length;
    return false;
  }

  // format: 2 bytes
  auto format =
      static_cast<Header_Format>(swapEndian(as<uint16_t>(readBytes(is, 2))));
  header->set_format(format);

  // num tracks: 2 bytes
  auto num_tracks = swapEndian(as<uint16_t>(readBytes(is, 2)));
  header->set_num_tracks(num_tracks);

  // division: 2 bytes
  auto division = swapEndian(as<uint16_t>(readBytes(is, 2)));
  header->set_division(division);

  return true;
}

static bool loadMIDIEvent(std::istream &is, const uint8_t status,
                          Event *event) {
  auto midi_event = event->mutable_midi_event();

  uint8_t type = (status >> 4) & 0xf;
  uint8_t status_real = status;

  uint8_t first_byte;

  // abbreviation message with no status byte, use last valid status
  bool abbrv = (type < 0x8 || type > 0xe);
  if (abbrv) {
    type = (s_last_midi_status >> 4) & 0xf;
    status_real = s_last_midi_status;
    first_byte = status;
  } else {
    s_last_midi_status = status;
    first_byte = as<uint8_t>(readBytes(is, 1));
  }

  midi_event->set_channel(status_real & 0xf);
  if (type == 0x8) {
    auto note_off = midi_event->mutable_note_off();
    note_off->set_note(first_byte);
    note_off->set_velocity(as<uint8_t>(readBytes(is, 1)));
  } else if (type == 0x9) {
    auto note_on = midi_event->mutable_note_on();
    note_on->set_note(first_byte);
    note_on->set_velocity(as<uint8_t>(readBytes(is, 1)));
  } else if (type == 0xa) {
    auto after_touch = midi_event->mutable_after_touch();
    after_touch->set_note(first_byte);
    after_touch->set_pressure(as<uint8_t>(readBytes(is, 1)));
  } else if (type == 0xb) {
    auto control_change = midi_event->mutable_control_change();
    control_change->set_controller(first_byte);
    control_change->set_value(as<uint8_t>(readBytes(is, 1)));
  } else if (type == 0xc) {
    auto program_change = midi_event->mutable_program_change();
    program_change->set_program(first_byte);
  } else if (type == 0xd) {
    auto channel_pressure = midi_event->mutable_channel_pressure();
    channel_pressure->set_pressure(first_byte);
  } else if (type == 0xe) {
    auto pitch_wheel_change = midi_event->mutable_pitch_wheel_change();
    auto lsb = first_byte;
    auto msb = as<uint8_t>(readBytes(is, 1));
    pitch_wheel_change->set_value(((msb & 0x7f) << 7) | (lsb & 0x7f));
  } else {
    return false;
  }

  return true;
}

static bool loadSysexEvent(std::istream &is, const uint8_t status,
                           Event *event) {
  return true;
}

static bool loadMetaEvent(std::istream &is, const uint8_t status,
                          Event *event) {
  auto type = MetaEventType(as<uint8_t>(readBytes(is, 1)));
  auto data = readBytes(is, readVariableLengthQuantity(is));
  auto meta_event = event->mutable_meta_event();
  meta_event->set_type(type);
  meta_event->set_data(data.data(), data.size());
  return true;
}

static bool loadTrack(std::istream &is, Track *track) {
  // type: 4 bytes
  const auto type = readBytes(is, 4);
  const auto type_str = std::string(type.cbegin(), type.cend());
  if (type_str != "MTrk") {
    std::cout << "Type of track is invalid: " << type_str << std::endl;
    return false;
  }

  // length: 4 bytes
  auto length = swapEndian(as<uint32_t>(readBytes(is, 4)));

  // get data of this track
  auto data = readBytes(is, length);

  // parse
  std::stringstream ss(std::string(data.cbegin(), data.cend()));
  while (ss) {
    auto event = track->add_events();
    event->set_delta_time(readVariableLengthQuantity(ss));

    auto status = as<uint8_t>(readBytes(ss, 1));

    switch (status) {
      case 0xff:
        loadMetaEvent(ss, status, event);
        break;

      case 0xf0:
      case 0xf7:
        loadSysexEvent(ss, status, event);
        break;

      default:
        loadMIDIEvent(ss, status, event);
        break;
    }
  }

  return true;
}

bool load(const std::string &file_path, File *file) {
  if (!file) {
    std::cout << "file == nullptr" << std::endl;
    return false;
  }

  std::ifstream ifs(file_path);
  if (!ifs.is_open()) {
    std::cout << "Failed to open file " << file_path << std::endl;
    return false;
  }

  // load header
  if (!loadHeader(ifs, file->mutable_header())) {
    std::cout << "Failed to load header" << std::endl;
    return false;
  }

  // load tracks
  file->mutable_tracks()->Clear();
  for (uint32_t track_no = 0; track_no < file->header().num_tracks();
       ++track_no) {
    if (!loadTrack(ifs, file->add_tracks())) {
      std::cout << "Failed to load track number " << track_no;
      return false;
    }
  }

  return true;
}

bool toNotes(const File &midi_file, Notes *notes) {
  if (!notes) {
    std::cout << "notes == nullptr" << std::endl;
    return false;
  }

  // setup timestamp of events
  std::unordered_map<const MIDIEvent *, int64_t> map_event_timestamp;
  for (const auto &track : midi_file.tracks()) {
    // setup us_per_tick
    uint64_t us_per_tick;
    const auto division = midi_file.header().division();
    if (division & 0x8000) {
      auto ticks_per_frame = division & 0xff;
      uint32_t frames_per_second;
      uint16_t indicator = ((division >> 8) & 0b1100000) >> 5;
      switch (indicator) {
        case 0:
          frames_per_second = 24;
          break;
        case 1:
          frames_per_second = 25;
          break;
        default:
          frames_per_second = 30;
          break;
      }
      us_per_tick = 1000000 / (ticks_per_frame * frames_per_second);
    } else {
      uint32_t ticks_per_quater_note = division & 0x7fff;
      uint32_t us_per_quater_note = 500000;
      us_per_tick = us_per_quater_note / ticks_per_quater_note;
    }

    // timestamp count from 0
    int64_t timestamp = 0;

    for (const auto &event : track.events()) {
      // check if SetTempo
      if (event.has_meta_event()) {
        const auto &meta = event.meta_event();
        if (meta.type() == MetaEventType::SetTempo && !(division & 0x8000) &&
            meta.data().size() == 3) {
          uint32_t ticks_per_quater_note = division & 0x7fff;
          uint32_t us_per_quater_note = uint8_t(meta.data()[0]) << 16 |
                                        uint8_t(meta.data()[1]) << 8 |
                                        uint8_t(meta.data()[2]);
          us_per_tick = us_per_quater_note / ticks_per_quater_note;
        }
      }

      // current timestamp
      timestamp += event.delta_time() * us_per_tick;

      // set timestamp
      if (event.has_midi_event()) {
        map_event_timestamp[&event.midi_event()] = timestamp;
      }
    }
  }

  // pair notes
  notes->Clear();
  std::unordered_map<uint32_t, std::unordered_map<uint32_t, const MIDIEvent *>>
      buffer;
  for (const auto &track : midi_file.tracks()) {
    for (const auto &event : track.events()) {
      if (event.has_midi_event()) {
        const auto &midi_event = event.midi_event();
        const auto &channel = midi_event.channel();
        if (midi_event.has_note_off()) {
          const auto &note_off = midi_event.note_off();
          auto last = buffer[channel][note_off.note()];
          if (last) {
            auto note = notes->add_notes();
            note->set_timestamp(map_event_timestamp[last]);
            note->set_duration(map_event_timestamp[&midi_event] -
                               note->timestamp());
            note->set_channel(channel);
            note->set_note(note_off.note());
            note->set_velocity(last->note_on().velocity());
          } else {
            std::cout << "Failed to find note " << note_off.note()
                      << " on channel " << channel << std::endl;
          }
          buffer[channel][note_off.note()] = nullptr;
        } else if (midi_event.has_note_on() &&
                   midi_event.note_on().velocity() == 0) {
          const auto &note_on = midi_event.note_on();
          auto last = buffer[channel][note_on.note()];
          if (last) {
            auto note = notes->add_notes();
            note->set_timestamp(map_event_timestamp[last]);
            note->set_duration(map_event_timestamp[&midi_event] -
                               note->timestamp());
            note->set_channel(channel);
            note->set_note(note_on.note());
            note->set_velocity(last->note_on().velocity());
          } else {
            std::cout << "Failed to find note " << note_on.note()
                      << " on channel " << channel << std::endl;
          }
          buffer[channel][note_on.note()] = nullptr;
        } else {
          if (midi_event.has_note_on()) {
            const auto &note_on = midi_event.note_on();
            buffer[channel][note_on.note()] = &midi_event;
          }
        }
      }
    }
  }

  if (notes->notes_size() != 0) {
    // sort notes by timestamp
    std::sort(notes->mutable_notes()->begin(), notes->mutable_notes()->end(),
              [](const Note &lhs, const Note &rhs) {
                return lhs.timestamp() <= rhs.timestamp();
              });
  }

  return true;
}

}  // namespace midi
