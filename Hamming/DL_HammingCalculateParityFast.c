// David Cook
// RobotRoom.com
// June 20, 2014
// LoFi project for TheHackADayPrize
// http://hackaday.io/project/1552-LoFi

#include "DL_Hamming.h"

// For processors with a Harvard architecture where you want
// to indicate the table should be in program space rather than RAM.
// On other processors, just define __flash as blank.
#ifndef __flash
#define __flash
#endif


// This contains all of the precalculated parity values for a byte (8 bits).
// This is very fast, but takes up more program space than calculating on the fly.
__flash byte _hammingCalculateParityFast128[256]=
{
	 0,  3,  5,  6,  6,  5,  3,  0,  7,  4,  2,  1,  1,  2,  4,  7,
	 9, 10, 12, 15, 15, 12, 10,  9, 14, 13, 11,  8,  8, 11, 13, 14,
	10,  9, 15, 12, 12, 15,  9, 10, 13, 14,  8, 11, 11,  8, 14, 13,
	 3,  0,  6,  5,  5,  6,  0,  3,  4,  7,  1,  2,  2,  1,  7,  4,
	11,  8, 14, 13, 13, 14,  8, 11, 12, 15,  9, 10, 10,  9, 15, 12,
	 2,  1,  7,  4,  4,  7,  1,  2,  5,  6,  0,  3,  3,  0,  6,  5,
	 1,  2,  4,  7,  7,  4,  2,  1,  6,  5,  3,  0,  0,  3,  5,  6,
	 8, 11, 13, 14, 14, 13, 11,  8, 15, 12, 10,  9,  9, 10, 12, 15,
	12, 15,  9, 10, 10,  9, 15, 12, 11,  8, 14, 13, 13, 14,  8, 11,
	 5,  6,  0,  3,  3,  0,  6,  5,  2,  1,  7,  4,  4,  7,  1,  2,
	 6,  5,  3,  0,  0,  3,  5,  6,  1,  2,  4,  7,  7,  4,  2,  1,
	15, 12, 10,  9,  9, 10, 12, 15,  8, 11, 13, 14, 14, 13, 11,  8,
	 7,  4,  2,  1,  1,  2,  4,  7,  0,  3,  5,  6,  6,  5,  3,  0,
	14, 13, 11,  8,  8, 11, 13, 14,  9, 10, 12, 15, 15, 12, 10,  9,
	13, 14,  8, 11, 11,  8, 14, 13, 10,  9, 15, 12, 12, 15,  9, 10,
	 4,  7,  1,  2,  2,  1,  7,  4,  3,  0,  6,  5,  5,  6,  0,  3,
};

// Given a byte to transmit, this returns the parity as a nibble
nibble DL_HammingCalculateParity128(byte value)
{
	return _hammingCalculateParityFast128[value];
}

// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte DL_HammingCalculateParity2416(byte first, byte second)
{
	return (_hammingCalculateParityFast128[second]<<4) | _hammingCalculateParityFast128[first];
}
