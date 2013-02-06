/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

// language manipulation
// phonemes represented in Arpabet notation

/*
Phonemes:
AA odd     AA D
AE at      AE T
AH hut     HH AH T
AO ought   AO T
AW cow     K  AW
AY hide    HH AY D
B  be      B  IY
CH cheese  CH IY Z
D  dee     D  IY
DH thee    DH IY
EH Ed      EH D
ER hurt    HH ER T
EY ate     EY T
F  fee     F  IY
G  green   G  R  IY N
HH he      HH IY
IH it      IH T
IY eat     IY T
JH gee     JH IY
K  key     K  IY
L  lee     L  IY
M  me      M  IY
N  knee    N  IY
NG ping    P  IH NG
OW oat     OW T
OY toy     T  OY
P  pee     P  IY
R  read    R  IY D
S  sea     S  IY
SH she     SH IY
T  tea     T  IY
TH theta   TH EY T  AH
UH hood    HH UH D
UW two     T  UW
V  vee     V  IY
W  we      W  IY
Y  yield   Y  IY L  D
Z  zee     Z  IY
ZH seizure S  IY ZH ER
*/

#ifndef    page_local_util_lang_hpp
#   define page_local_util_lang_hpp

#	include <string>

namespace page
{
	namespace util
	{
		// phoneme generation
		std::string GetPhonemes(const std::string &);

		// editing
		std::string EditSpeech(const std::string &);
		std::string EditText(const std::string &);
	}
}

#endif
