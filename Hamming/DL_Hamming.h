// David Cook
// RobotRoom.com
// June 20, 2014
// LoFi project for TheHackADayPrize
// http://hackaday.io/project/1552-LoFi

#ifndef _DL_HAMMING_H
#define _DL_HAMMING_H

#ifndef _DL_AVR_H
typedef unsigned char byte;
typedef unsigned char nibble;
#endif

/**** These are needed to transmit and receive ****/

// Given a byte to transmit, this returns the parity as a nibble
nibble DL_HammingCalculateParity128(byte value);

// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte DL_HammingCalculateParity2416(byte first, byte second);



/**** These are needed only to receive ****/

// Given a pointer to a received byte and the received parity (as a lower nibble),
// this calculates what the parity should be and fixes the recevied value if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
byte DL_HammingCorrect128(byte* value, nibble parity);

// Given a pointer to a first value and a pointer to a second value and
// their combined given parity (lower nibble first parity, upper nibble second parity),
// this calculates what the parity should be and fixes the values if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 2 means two corrected errors
// 3 means corrections not possible
byte DL_HammingCorrect2416(byte* first, byte* second, byte parity);

#endif // _DL_HAMMING_H
