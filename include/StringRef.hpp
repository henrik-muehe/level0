/*
Copyright 2013 Henrik Mühe and Florian Funke

This file is part of libdistance.

libdistance is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libdistance is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with libdistance.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <cstdint>
#include <string>
#include <cstring>


/// A non-owning string reference
class StringRef {
	/// A pointer to the data of the string
	const char* str;
	/// The length of the string
	uint32_t len;

public:
	/// Implicit constructor from std::string
	StringRef(const std::string& str) : str(str.data()),len(str.length()) {}
	/// Constructor
	StringRef(const char* str,uint32_t len) : str(str),len(len) {}
	/// Constructor
	StringRef(const char* str) : str(str),len(strlen(str)) {}
	/// Explicit conversion to std::string
	explicit operator std::string() const { return std::string(str,len); }
	/// Return pointer to data
	const char* data() const { return str; }
	/// Return length
	uint32_t length() const { return len; }
	/// Return begin iterator
	const char* begin() const { return str; }
	/// Return end iterator
	const char* end() const { return str+len; }
	/// Return a new referece that only references a substring
	StringRef substring(uint32_t pos) const {
		return StringRef(str+pos,len-pos);
	}
	/// Return a new referece that only references a substring
	StringRef substring(uint32_t pos,uint32_t len) const {
		return StringRef(str+pos,len);
	}
	/// Index operator
	char operator[](uint32_t index) const { return str[index]; }
	/// Compare
	bool operator == (const StringRef& other) const { return len==other.len && memcmp(str, other.str, len)==0; }
};
