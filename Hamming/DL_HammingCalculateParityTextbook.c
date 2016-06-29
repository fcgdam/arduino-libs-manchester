// David Cook
// RobotRoom.com
// June 20, 2014
// LoFi project for TheHackADayPrize
// http://hackaday.io/project/1552-LoFi

#include "DL_Hamming.h"

// This performs poorly on a processor that can't do multiple shifts in one instruction
#define BitToBool(byte, n) ((byte>>(n-1)) & 1)

// This is a private method that calculates half the parity
// while shifting over any previously calculated parity for another byte.
// This was designed for processors that cannot shift more than one bit place at a time
nibble DL_HammingCalculateParity2416Half(byte value, byte paritySoFar)
{
	// Calculate the most significant bit
	paritySoFar |= BitToBool(value, 5) ^ BitToBool(value, 6) ^ BitToBool(value, 7) ^ BitToBool(value, 8);
	// Shift it over
	paritySoFar <<= 1;

	// Calculate the next most significant bit
	paritySoFar |= BitToBool(value, 2) ^ BitToBool(value, 3) ^ BitToBool(value, 4) ^ BitToBool(value, 8);
	// Shift it over, as well as the previously calculated bit
	paritySoFar <<= 1;

	// Calculate the next most significant bit
	paritySoFar |= BitToBool(value, 1) ^ BitToBool(value, 3) ^ BitToBool(value, 4) ^ BitToBool(value, 6) ^ BitToBool(value, 7);
	// Shift it over, as well as the previously calculated bits
	paritySoFar <<= 1;

	// Calculate the least significant bit
	paritySoFar |= BitToBool(value, 1) ^ BitToBool(value, 2) ^ BitToBool(value, 4) ^ BitToBool(value, 5) ^ BitToBool(value, 7);

	return paritySoFar;
}

// Given a byte to transmit, this returns the parity as a nibble
nibble DL_HammingCalculateParity128(byte value)
{
	return DL_HammingCalculateParity2416Half(value, 0);
}

// If your processor can shift multiple bit places in a single instruction, then set this to 1.
// It will be twice as fast.
// If your processor cannot, then set this to 0, otherwise it will be slow and large.
#define CPU_HAS_MULTIPLE_SHIFT_INSTRUCTION 0

#if CPU_HAS_MULTIPLE_SHIFT_INSTRUCTION
// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte DL_HammingCalculateParity2416(byte first, byte second)
{
	// This is the textbook way to calculate hamming parity.
	return	((BitToBool(first, 1) ^ BitToBool(first, 2) ^ BitToBool(first, 4) ^ BitToBool(first, 5) ^ BitToBool(first, 7))) +
			((BitToBool(first, 1) ^ BitToBool(first, 3) ^ BitToBool(first, 4) ^ BitToBool(first, 6) ^ BitToBool(first, 7))<<1) +
			((BitToBool(first, 2) ^ BitToBool(first, 3) ^ BitToBool(first, 4) ^ BitToBool(first, 8))<<2) +
			((BitToBool(first, 5) ^ BitToBool(first, 6) ^ BitToBool(first, 7) ^ BitToBool(first, 8))<<3) +

			((BitToBool(second, 1) ^ BitToBool(second, 2) ^ BitToBool(second, 4) ^ BitToBool(second, 5) ^ BitToBool(second, 7))<<4) +
			((BitToBool(second, 1) ^ BitToBool(second, 3) ^ BitToBool(second, 4) ^ BitToBool(second, 6) ^ BitToBool(second, 7))<<5) +
			((BitToBool(second, 2) ^ BitToBool(second, 3) ^ BitToBool(second, 4) ^ BitToBool(second, 8))<<6) +
			((BitToBool(second, 5) ^ BitToBool(second, 6) ^ BitToBool(second, 7) ^ BitToBool(second, 8))<<7);
}
#else
// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte DL_HammingCalculateParity2416(byte first, byte second)
{
	// This makes two calls, one for each byte.
	// It passes the result of the second byte into the calculation for the first
	// such that the four shift instructions and the 'or' instruction are free.
	return DL_HammingCalculateParity2416Half(first, DL_HammingCalculateParity2416Half(second, 0) << 1);
}
#endif


