#include "soundmanager.h"

#include "ifilesystem.h"
#include "archivelib.h"
#include "generic/callback.h"

#include <iostream>

namespace sound
{
	// Constructor
	SoundManager::SoundManager ()
	{
	}

	bool SoundManager::playSound (const std::string& fileName)
	{
		// Make a copy of the filename
		std::string name = fileName;

		// Try to open the file as it is
		ArchiveFile* file = GlobalFileSystem().openFile(name.c_str());
		std::cout << "Trying: " << name << std::endl;
		if (file != NULL) {
			// File found, play it
			std::cout << "Found file: " << name << std::endl;
			_soundPlayer.play(*file);
			return true;
		}

		std::string root = name;
		// File not found, try to strip the extension
		if (name.rfind(".") != std::string::npos) {
			root = name.substr(0, name.rfind("."));
		}

		// Try to open the .ogg variant
		name = root + ".ogg";
		std::cout << "Trying: " << name << std::endl;
		file = GlobalFileSystem().openFile(name.c_str());
		if (file != NULL) {
			std::cout << "Found file: " << name << std::endl;
			_soundPlayer.play(*file);
			return true;
		}

		// Try to open the file with .wav extension
		name = root + ".wav";
		std::cout << "Trying: " << name << std::endl;
		file = GlobalFileSystem().openFile(name.c_str());
		if (file != NULL) {
			std::cout << "Found file: " << name << std::endl;
			_soundPlayer.play(*file);
			return true;
		}

		// File not found
		return false;
	}

	void SoundManager::stopSound ()
	{
		_soundPlayer.stop();
	}

} // namespace sound
