/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "UICore/precomp.h"
#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif
#include <fnmatch.h>
#include <unistd.h>
#include "directory_scanner_unix.h"

#include <string.h>

namespace uicore
{

DirectoryScanner_Unix::DirectoryScanner_Unix ()
	: dir_temp (nullptr), entry (nullptr)
{
}

bool DirectoryScanner_Unix::scan (const std::string& arg_path_name)
{
	path_name   = arg_path_name;
	use_pattern = false;

	if (path_name.empty ())
	  path_name = ".";

	if(dir_temp)
		closedir(dir_temp);

	dir_temp = opendir(path_name.c_str());
	if (dir_temp == nullptr)
		return false;
	else
		return true;
}

bool DirectoryScanner_Unix::scan (const std::string& arg_path_name, 
				     const std::string& arg_file_pattern)
{
	path_name    = arg_path_name;
	file_pattern = arg_file_pattern;
	use_pattern  = true;

	if (path_name.empty ())
	  path_name = ".";

	if(dir_temp)
		closedir(dir_temp);

	dir_temp = opendir(path_name.c_str());
	if (dir_temp == nullptr)
		return false;
	else
		return true;
}

DirectoryScanner_Unix::~DirectoryScanner_Unix()
{
	if(dir_temp)
		closedir(dir_temp);
}

std::string DirectoryScanner_Unix::directory_path()
{
	return path_name + "/";
}

int DirectoryScanner_Unix::size()
{
	return (int)statbuf.st_size;
}

std::string DirectoryScanner_Unix::name()
{
	return file_name;
}

std::string DirectoryScanner_Unix::pathname()
{
	return path_name + "/" + file_name;
}

bool DirectoryScanner_Unix::is_directory()
{
	return (statbuf.st_mode & S_IFDIR);
}

bool DirectoryScanner_Unix::is_hidden()
{
	return file_name[0] == '.' && file_name[1] != 0 && file_name[1] != '.';
}

bool DirectoryScanner_Unix::is_readable()
{
	return access((path_name + "/" + file_name).c_str(), R_OK) == 0;
}

bool DirectoryScanner_Unix::is_writable()
{
	return access((path_name + "/" + file_name).c_str(), W_OK) == 0;
}

bool DirectoryScanner_Unix::next()
{	
	if(!dir_temp)
		throw Exception("Directory scanner not initialized");

	entry = readdir(dir_temp);

	if( entry == nullptr )
		return false;

	file_name = entry->d_name;

	if (stat((path_name + "/" + file_name).c_str(), &statbuf) == -1)
		memset(&statbuf, 0, sizeof(statbuf));

	if (use_pattern)
	{
		if (fnmatch(file_pattern.c_str(), file_name.c_str(), FNM_PATHNAME) == 0)
			return true;
		else
			return next();
	} else {
		return true;
	}
}

}
