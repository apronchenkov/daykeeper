// *****************************************************************************
// CRCHash Routines (Version 1.0)
// Copyright c 2004-2005 Mira Software, Inc.
// Author: Andrey Klimoff
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// o Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// o Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
// o The names of the authors may not be used to endorse or promote
//   products derived from this software without specific prior written
//   permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *****************************************************************************

// Revision: September 15, 2006

/*******************************************************************************
   Cyclic Redundancy Check (CRC)

   A CRC is a numerical value related to a block of data. This value provides
information that allows an application to determine if the block of data has
been modified. A CRC is similar to a Checksum, but is considered a stronger
error-checking mechanism.

   CRC is based on binary polynomial calculation.

   Standard CRC-8 generator polynomial:
       Name               : CRC-8 Standard
       Standards          : -
       References         : -
       Initializing value : FF
       Finalizing value   : FF
       Polynomial value   : 31 (Mirror value = 8C)
       Polynom            : x^8 + x^5 + x^4 + 1

   Standard CRC-16 generator polynomial:
       Name               : CRC-16 Standard
       Standards          : ITU X.25/T.30
       References         : LHA
       Initializing value : 0000
       Finalizing value   : 0000
       Polynomial value   : 8005 (Mirror value = A001)
       Polynom            : x^16 + x^15 + x^2 + 1

   CRC-16 CCITT generator polynomial:
       Name               : CRC-16 CCITT
       Standards          : CRC-CCITT
       References         : ITU X.25/T.30, ADCCP, SDLC/HDLC
       Initializing value : FFFF
       Finalizing value   : 0000
       Polynomial value   : 1021 (Mirror value = 8408)
       Polynom            : x^16 + x^12 + x^5 + 1

   CRC-16 XModem generator polynomial:
       Name               : CRC-16 XModem
       Standards          : CRC-XModem
       References         : -
       Initializing value : 0000
       Finalizing value   : 0000
       Polynomial value   : 8408 (Mirror value = 1021)
       Polynom            : x^16 + x^12 + x^5 + 1

   Standard CRC-32 generator polynomial:
       Name               : CRC-32 Standard
       Standards          : ISO 3309, ITU-T V.42, ANSI X3.66, FIPS PUB 71
       References         : ZIP, RAR, Ethernet, AUTODIN II, FDDI
       Initializing value : FFFFFFFF
       Finalizing value   : FFFFFFFF
       Polynomial value   : 04C11DB7 (Mirror value = EDB88320)
       Polynom            : x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 +
                            x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1

   Standard CRC-64 generator polynomial:
       Name               : CRC-64 Standard
       Standards          : ISO 3309
       References         : -
       Initializing value : FFFFFFFFFFFFFFFF
       Finalizing value   : FFFFFFFFFFFFFFFF
       Polynomial value   : 000000000000001B (Mirror value = D800000000000000)
       Polynom            : x^64 + x^4 + x^3 + x + 1

//******************************************************************************/
// Demo:

#include <cstdio>
#include <cstddef>
#include <cstring>
#include <cerrno>

#include "CRCHash.h"

using namespace std;

int main(int argc, char * argv[]) {
	if( 1 == argc ) {
		fprintf(stderr, "usage: %s file ...\n\n", argv[0]);
		return -1;
	}

	CRC32Hash crcHash;

	char buf[4096];
	int ret = 0;
	
	for(size_t i = 1; i < argc; ++i) {
		FILE * file = fopen(argv[i], "rb");
		if( NULL == file ) {
			ret = -1;
			fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
			continue;
		}
		
		crcHash.Reset();

		while( !ferror(file) && !feof(file) ) {
			const size_t count = fread(buf, 1, sizeof(buf), file);

			crcHash.Update(buf, count);
		}

		if( ferror(file) ) {
			ret = -1;
			fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
			fclose(file);
			continue;
		}

		fclose(file);
		
		printf("%08lX\t%s\n", crcHash.Evaluate(), argv[i]);
	}

	return ret;
}	

/*******************************************************************************/
