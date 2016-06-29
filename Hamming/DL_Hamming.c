// David Cook
// RobotRoom.com
// June 20, 2014
// LoFi project for TheHackADayPrize
// http://hackaday.io/project/1552-LoFi

#include "DL_Hamming.h"

#ifndef null
#define null ((void*) 0)
#endif

// For processors with a Harvard architecture where you want
// to indicate the table should be in program space rather than RAM.
// On other processors, just define __flash as blank.
#ifndef __flash
#define __flash
#endif

// If transmitting/writing only, you don't need to include this file.
// If receiving/reading, then this provides the methods to correct bit errors.

#define UNCORRECTABLE	0xFF
#define ERROR_IN_PARITY	0xFE
#define NO_ERROR		0x00

// Private table. Faster and more compact than multiple if statements.
static __flash byte _hammingCorrect128Syndrome[16] =
{
	NO_ERROR,			// 0
	ERROR_IN_PARITY,	// 1
	ERROR_IN_PARITY,	// 2
	0x01,				// 3
	ERROR_IN_PARITY,	// 4
	0x02,				// 5
	0x04,				// 6
	0x08,				// 7
	ERROR_IN_PARITY,	// 8
	0x10,				// 9
	0x20,				// 10
	0x40,				// 11
	0x80,				// 12
	UNCORRECTABLE,		// 13
	UNCORRECTABLE,		// 14
	UNCORRECTABLE,		// 15
};

// Private method
// Give a pointer to a received byte,
// and given a nibble difference in parity (parity ^ calculated parity)
// this will correct the received byte value if possible.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
static byte DL_HammingCorrect128Syndrome(byte* value, byte syndrome)
{
	// Using only the lower nibble (& 0x0F), look up the bit
	// to correct in a table
	byte correction = _hammingCorrect128Syndrome[syndrome & 0x0F];

	if (correction != NO_ERROR)
	{
		if (correction == UNCORRECTABLE || value == null)
		{
			return 3; // Non-recoverable error
		}
		else
		{
			if ( correction != ERROR_IN_PARITY)
			{
				*value ^= correction;
			}

			return 1; // 1-bit recoverable error;
		}
	}

	return 0; // No errors
}

// Given a pointer to a received byte and the received parity (as a lower nibble),
// this calculates what the parity should be and fixes the recevied value if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
byte DL_HammingCorrect128(byte* value, nibble parity)
{
	byte syndrome;

	if (value == null)
	{
		return 3; // Non-recoverable error
	}

	syndrome = DL_HammingCalculateParity128(*value) ^ parity;

	if (syndrome != 0)
	{
		return DL_HammingCorrect128Syndrome(value, syndrome);
	}

	return 0; // No errors
}


// Given a pointer to a first value and a pointer to a second value and
// their combined given parity (lower nibble first parity, upper nibble second parity),
// this calculates what the parity should be and fixes the values if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 2 means two corrected errors
// 3 means corrections not possible
byte DL_HammingCorrect2416(byte* first, byte* second, byte parity)
{
	byte syndrome;

	if (first == null || second == null)
	{
		return 3; // Non-recoverable error
	}

	syndrome = DL_HammingCalculateParity2416(*first, *second) ^ parity;

	if (syndrome != 0)
	{
		return DL_HammingCorrect128Syndrome(first, syndrome) + DL_HammingCorrect128Syndrome(second, syndrome >> 4);
	}

	return 0; // No errors
}
