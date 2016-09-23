#pragma once

namespace Filesystem {
	enum class FileCreateMode {
		CreateAlways, // rewrite or create new file
		CreateNew, // fail if exists or create new file
		CreateUnique, // creates new or generates new name for file
		OpenAlways, // open existing or create new file
		OpenExisting, // fail if not exists or open existing file
	};

	enum class FolderCreateMode {
		OpenAlways, // open existing or create new folder
	};
}