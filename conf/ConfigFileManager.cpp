/*
Copyright (C) 2004 The Pentagram team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "pent_include.h"

#include "ConfigFileManager.h"
#include "INIFile.h"

#include <set>

using Pentagram::istring;
using std::string;

ConfigFileManager* ConfigFileManager::configfilemanager = 0;

ConfigFileManager::ConfigFileManager()
{
	con.Print(MM_INFO, "Creating ConfigFileManager...\n");

	assert(configfilemanager == 0);
	configfilemanager = this;
}

ConfigFileManager::~ConfigFileManager()
{
	con.Print(MM_INFO, "Destroying ConfigFileManager...\n");

	clear();
	configfilemanager = 0;
}

bool ConfigFileManager::readConfigFile(const string &fname, const istring &root,
									   bool readonly)
{
	INIFile* inifile = new INIFile();
	inifile->clear(root);
	if (!inifile->readConfigFile(fname)) {
		delete inifile;
		return false;
	}
	if (readonly)
		inifile->setReadonly();

	inifiles.push_back(inifile);
	return true;
}

bool ConfigFileManager::readConfigString(const string &config, const istring &root,
										 bool readonly)
{
	INIFile* inifile = new INIFile();
	inifile->clear(root);
	if (!inifile->readConfigString(config)) {
		delete inifile;
		return false;
	}
	if (readonly)
		inifile->setReadonly();

	inifiles.push_back(inifile);
	return true;
}

void ConfigFileManager::write(const istring &root)
{
	for (std::vector<INIFile*>::iterator i = inifiles.begin();
		 i != inifiles.end(); ++i)
	{
		if (!(*i)->isReadonly() && (root == "" || (*i)->checkRoot(root)))
			(*i)->write();
	}
}

void ConfigFileManager::clear()
{
	for (std::vector<INIFile*>::iterator i = inifiles.begin();
		 i != inifiles.end(); ++i)
	{
		delete (*i);
	}
	inifiles.clear();
}

void ConfigFileManager::clearRoot(const Pentagram::istring &root)
{
	std::vector<INIFile*>::iterator i = inifiles.begin();

	while (i != inifiles.end()) {
		if ((*i)->checkRoot(root)) {
			delete (*i);
			i = inifiles.erase(i);
		}
		else {
			++i;
		}
	}
}

bool ConfigFileManager::exists(const istring &key) const
{
	return (findKeyINI(key) != 0);
}

bool ConfigFileManager::get(const istring &key, string& ret) const
{
	INIFile* ini = findKeyINI(key);
	if (!ini) return false;

	ini->value(key, ret);
	return true;
}


bool ConfigFileManager::get(const istring &key, int& ret) const
{
	INIFile* ini = findKeyINI(key);
	if (!ini) return false;

	ini->value(key, ret);
	return true;
}

bool ConfigFileManager::get(const istring &key, bool& ret) const
{
	INIFile* ini = findKeyINI(key);
	if (!ini) return false;

	ini->value(key, ret);
	return true;
}

void ConfigFileManager::set(const istring &key, const string &val)
{
	INIFile* ini = findWriteINI(key);
	if (!ini) return;

	ini->set(key, val);
}

void ConfigFileManager::set(const istring &key, const char* val)
{
	INIFile* ini = findWriteINI(key);
	if (!ini) return;

	ini->set(key, val);
}

void ConfigFileManager::set(const istring &key, int val)
{
	INIFile* ini = findWriteINI(key);
	if (!ini) return;

	ini->set(key, val);
}

void ConfigFileManager::set(const istring &key, bool val)
{
	INIFile* ini = findWriteINI(key);
	if (!ini) return;

	ini->set(key, val);
}

void ConfigFileManager::unset(const istring &key)
{
	INIFile* ini = findWriteINI(key);
	if (!ini) return;

	ini->unset(key);
}



std::vector<istring> ConfigFileManager::listKeys(const istring &section,
												 bool longformat) const
{
	std::vector<istring> keys;

	std::set<istring> keyset;
	std::set<istring>::const_iterator iter;

	for (std::vector<INIFile*>::const_iterator i = inifiles.begin();
		 i != inifiles.end(); ++i)
	{
		if ((*i)->checkRoot(section)) {
			(*i)->listKeys(keyset, section, longformat);
		}
	}

	for (iter = keyset.begin(); iter != keyset.end(); ++iter)
	{
		keys.push_back(*iter);
	}

	return keys;
}

std::vector<istring> ConfigFileManager::listSections(const istring &root,
													 bool longformat) const
{
	std::vector<istring> sections;

	std::set<istring> sectionset;
	std::set<istring>::iterator iter;

	for (std::vector<INIFile*>::const_iterator i = inifiles.begin();
		 i != inifiles.end(); ++i)
	{
		if ((*i)->checkRoot(root)) {
			(*i)->listSections(sectionset, longformat);
		}
	}

	for (iter = sectionset.begin(); iter != sectionset.end(); ++iter)
	{
		sections.push_back(*iter);
	}

	return sections;
}

std::map<istring,std::string> ConfigFileManager::listKeyValues(const istring &section,
															   bool longformat) const
{
	std::map<istring, std::string> values;

	for (std::vector<INIFile*>::const_iterator i = inifiles.begin();
		 i != inifiles.end(); ++i)
	{
		if ((*i)->checkRoot(section)) {
			(*i)->listKeyValues(values, section, longformat);
		}
	}

	return values;
}


INIFile* ConfigFileManager::findKeyINI(const istring &key) const
{
	for (std::vector<INIFile*>::const_reverse_iterator i = inifiles.rbegin();
		 i != inifiles.rend(); ++i)
	{
		if ((*i)->hasKey(key))
			return (*i);
	}

	return 0;
}

INIFile* ConfigFileManager::findWriteINI(const istring &key)
{
	for (std::vector<INIFile*>::const_reverse_iterator i = inifiles.rbegin();
		 i != inifiles.rend(); ++i)
	{
		if (!(*i)->isReadonly() && (*i)->checkRoot(key))
			return (*i);
	}

	return 0;
}
