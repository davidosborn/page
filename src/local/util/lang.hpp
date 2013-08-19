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
