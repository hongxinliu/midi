#pragma once

#include <string>
#include "midi/proto/file.pb.h"
#include "midi/proto/note.pb.h"

namespace midi {

/**
 * @brief load midi file
 * @param file_path path of midi file
 * @param midi_file loaded proto type midi object
 * @return true on success
 */
bool load(const std::string &file_path, File *midi_file);

/**
 * @brief convert proto type midi to readable notes
 * @param midi_file proto type midi object
 * @param notes converted readable notes
 * @return true on success
 */
bool toNotes(const File &midi_file, Notes *notes);

}  // namespace midi
