#pragma once

#ifdef _MSC_VER
#include "../../../CPlusPlus/OSC-light.Test/Print.h"
#else
#include <Print.h>
#endif

#include "OSCData.h"
#include "OSCMatch.h"

// static helpers
static OSCMatch _matchHelper = OSCMatch();

class OSCMessage
{
public:
	OSCMessage();
	~OSCMessage();

	// Sets the address of the message.
	void setAddress(const char * address);

	// Reserves the specified amount of OSCData elements to avoid multiple reallocations of the buffer.
	// Deletes the current OSCDatas and creates new ones. Use before add<T>()!
	void reserve(int count);

	// Reserves the specified amount of OSCData elements to avoid multiple reallocations of the buffer.
	// Deletes the current OSCDatas and creates new ones, only when current reserved count is smaller than given count. Use before add<T>()!
	void reserveAtLeast(int count);

	// Removes all data from the OSCData buffer.
	void empty();

	// Gets the value at the given data position.
	float getFloat(int position) {
		if (position < _dataCount) {
			return _data[position].data.f;
		}

		return 0.0;
	}
	// Gets the value at the given data position.
	int getInt(int position) {
		if (position < _dataCount) {
			return _data[position].data.i;
		}

		return 0;
	}

	// Sets the value in a new data position. 
	// To improve performance, this new position should be reserved first.
	template <typename T>
	void add(T datum) {
		if (_dataCount >= _reservedCount) {
			reserve(1);
		}

		_data[_dataCount++].set(datum);
	}

	// Evaluates wheter the given pattern is a valid route for the message.
	bool route(const char * pattern) {
		return _matchHelper.isMatch(_address, pattern);
	}

	// Sends the data using the given Print object.
	void send(Print * print);

	// Fills the data with the given data buffer.
	// To improve performance, do not destroy instances of OSCMessage but use fill() multiple times.
	void fill(const char * data, int dataLength);
private:
	char * _address;

	OSCData * _data;

	int _reservedCount = 0;
	int _dataCount = 0;

	char * _dataBuffer;
	int _bufferLength = 0;
	
	static inline int _padSize(int bytes) { return (4 - (bytes & 03)) & 3; }
};
