#pragma once

#include <iostream>
#include <sstream>
#include <cstdint>
#include <regex>

template<char C>
std::istream& expect(std::istream& in) {
	if((in >> std::ws).peek() == C) in.ignore();
	else in.setstate(std::ios_base::failbit);

	return in;
}

class IPv4 final {
public:
	static const int SIZE = 4;

	// constructor
	IPv4() = default;			// Use compiler-generated default constructor
	IPv4(const uint32_t r) : ip_(r) {}
	IPv4(const std::string r) {
#ifdef DEBUG
	std::cout << "[DEBUG] IPv4 Constructor begin\n";
#endif

	uint32_t a, b, c, d;
	std::istringstream iss(r);

	if(iss >> a >> expect<'.'> >> b >> expect<'.'> >> c >> expect<'.'> >> d) {
		ip_ = (a << 24) bitor (b << 16) bitor (c << 8) bitor d;
		return;
	}

	std::cerr << "Error: Error while converting string to IP" << std::endl;
	
	return;
}

	// assign operator
	// IPv4& operator = (const IPv4& r) { this->ip_ = r.ip_; return *this; }

	// casting operator
	operator uint32_t() const { return ip_; } // default
	explicit operator std::string() const;

	// comparison operator
	bool operator == (const IPv4& r) const { return ip_ == r.ip_; }

	bool isLocalHost() const { // 127.*.*.*
		uint8_t prefix = (ip_ & 0xFF000000) >> 24;
		return prefix == 0x7F;
	}

	bool isBroadcast() const { // 255.255.255.255
		return ip_ == 0xFFFFFFFF;
	}

	bool isMulticast() const { // 224.0.0.0 ~ 239.255.255.255
		uint8_t prefix = (ip_ bitand 0xFF000000) >> 24;
		return prefix >= 0xE0 and prefix < 0xF0;
	}
	
protected:
	uint32_t ip_;
};
