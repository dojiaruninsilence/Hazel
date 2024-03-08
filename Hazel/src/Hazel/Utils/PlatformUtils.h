// open save dialogs --- created file during vid
// header only for all plats
#pragma once

#include <string>

namespace Hazel {

	class FileDialogs
	{
	public:
		// these returns empty string if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}