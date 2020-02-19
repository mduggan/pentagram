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

#ifndef INIFILE_H
#define INIFILE_H

#include <set>
#include <list>

class INIFile
{
public:
	INIFile();
	INIFile(const std::string &fname, const Pentagram::istring &root);
	~INIFile();

	bool readConfigFile(const std::string &fname);

	//! read configuration from a string s. Lines must be separated by \n
	bool readConfigString(const std::string &s);

	void clear(const Pentagram::istring &root);

	std::string dump() const;
	void write() const;

	void setReadonly() { readonly = true; }
	bool isReadonly() const { return readonly; }

	bool hasSection(const Pentagram::istring &section) const;
	bool hasKey(const Pentagram::istring &key) const;
	bool checkRoot(const Pentagram::istring &key) const;

	// get value
	bool value(const Pentagram::istring &key, std::string &ret) const;
	bool value(const Pentagram::istring &key, int &ret) const;
	bool value(const Pentagram::istring &key, bool &ret) const;

	// set value
	void set(const Pentagram::istring &key, const std::string &value);
	void set(const Pentagram::istring &key, const char* value);
	void set(const Pentagram::istring &key, int value);
	void set(const Pentagram::istring &key, bool value);

	// remove key
	void unset(const Pentagram::istring &key);

	void listKeys(std::set<Pentagram::istring>& keys,
				  const Pentagram::istring &section,
				  bool longformat=false) const;

	void listSections(std::set<Pentagram::istring>& sections,
					  bool longformat=false) const;

	void listKeyValues(std::map<Pentagram::istring,std::string>& keyvalues,
					   const Pentagram::istring &section,
					   bool longformat = false) const;

private:
	std::string filename;
	Pentagram::istring root;
	bool is_file;
	bool readonly;

	struct KeyValue {
		Pentagram::istring key;
		std::string value;
		std::string comment;
	};

	struct Section {
		Pentagram::istring name;
		std::list<KeyValue> keys;
		std::string comment;

		bool hasKey(const Pentagram::istring &key) const;
		KeyValue* getKey(const Pentagram::istring &key);
		KeyValue const* getKey(const Pentagram::istring &key) const;
		void setKey(const Pentagram::istring &key, const std::string &value);
		void unsetKey(const Pentagram::istring &key);

		std::string dump() const;
	};

	std::list<Section> sections;


	bool stripRoot(Pentagram::istring& key) const;
	Section* getSection(const Pentagram::istring &section);
	Section const* getSection(const Pentagram::istring &section) const;
	bool splitKey(const Pentagram::istring &key, Pentagram::istring& section,
				  Pentagram::istring& sectionkey) const;

};

#endif
