// David Cook
// RobotRoom.com
// June 20, 2014
// LoFi project for TheHackADayPrize
// http://hackaday.io/project/1552-LoFi

#include "DL_Hamming.h"

// This is slower than using a table, but 10 times faster than
// using the textbook method.

// Given a byte to transmit, this returns the parity as a nibble
nibble DL_HammingCalculateParity128(byte value)
{
	// Exclusive OR is associative and commutative, so order of operations and values does not matter.
	nibble parity;

	if ( ( value & 1 ) != 0 )
	{
		parity = 0x3;
	}
	else
	{
		parity = 0x0;
	}

	if ( ( value & 2 ) != 0 )
	{
		parity ^= 0x5;
	}

	if ( ( value & 4 ) != 0 )
	{
		parity ^= 0x6;
	}

	if ( ( value & 8 ) != 0 )
	{
		parity ^= 0x7;
	}

	if ( ( value & 16 ) != 0 )
	{
		parity ^= 0x9;
	}

	if ( ( value & 32 ) != 0 )
	{
		parity ^= 0xA;
	}

	if ( ( value & 64 ) != 0 )
	{
		parity ^= 0xB;
	}

	if ( ( value & 128 ) != 0 )
	{
		parity ^= 0xC;
	}

	return parity;
}

// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte DL_HammingCalculateParity2416(byte first, byte second)
{
	return (DL_HammingCalculateParity128(second) << 4) | DL_HammingCalculateParity128(first);
}

