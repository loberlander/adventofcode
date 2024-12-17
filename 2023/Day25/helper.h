#pragma once

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>

struct Stringable
{
	virtual std::string asString() const = 0;
	inline operator std::string() const { return asString(); }
};


inline std::ostream& operator<<(std::ostream& os, const Stringable& x) { return os << x.asString(); }


inline std::string asString(bool x) { return x ? "true" : "false"; }


// Define specialization for std::vector<unsigned char>
inline std::ostream& operator<<(std::ostream& os, const std::vector<unsigned char>& v)
{
	os << "[";
	bool first = true;
	for (const auto& i : v)
	{
		os << (!first ? ", " : "");
		os << "0x" << std::hex << std::internal << std::setfill('0') << std::setw(2) << (int)i;
		first = false;
	}
	os << "]";
	return os;
}


template <typename T>
inline std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
	os << '[';
	bool first = true;
	for (const auto& i : v)
	{
		os << (!first ? ", " : "");
		os << i;
		first = false;
	}
	os << ']';
	return os;
}


template <typename T>
inline std::ostream& operator<<(std::ostream& os, std::set<T> const& v)
{
	os << '{';
	bool first = true;
	for (const auto& i : v)
	{
		os << (!first ? ", " : "");
		os << i;
		first = false;
	}
	os << '}';
	return os;
}


// Define specialization for std::unordered_map<std::string, <T>>
template <typename T>
inline std::ostream& operator<<(std::ostream& os, std::unordered_map<std::string, T> const& v)
{
	os << '{';
	bool first = true;
	for (const auto& i : v)
	{
		os << (!first ? ", " : "");
		os << i.first << ": " << i.second;
		first = false;
	}
	os << '}';
	return os;
}


// Define specialization for std::map<std::string, <T>>
template <typename T>
inline std::ostream& operator<<(std::ostream& os, std::map<std::string, T> const& v)
{
	os << '{';
	bool first = true;
	for (const auto& i : v)
	{
		os << (!first ? ", " : "");
		os << i.first << ": " << i.second;
		first = false;
	}
	os << '}';
	return os;
}


// Define specialization for std::map<int, <T>>
template <typename T>
inline std::ostream& operator<<(std::ostream& os, std::map<int, T> const& v)
{
	os << '{';
	bool first = true;
	for (const auto& i : v)
	{
		os << (!first ? ", " : "");
		os << i.first << ": " << i.second;
		first = false;
	}
	os << '}';
	return os;
}

template<typename T>
std::string asString(T t)
{
	std::ostringstream os;
	os << t;
	return os.str();
}


template< typename T >
std::string asHexString(T i)
{
	std::ostringstream os;
	os << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;
	return os.str();
}
