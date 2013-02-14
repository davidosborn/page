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

#include <algorithm> // remove
#include <cctype> // isalnum
#include <iostream> // cout
#include <iterator> // back_inserter
#include <memory> // unique_ptr
#include <string>
#include <unordered_map>
#include <vector>

#include "../err/report.hpp" // ReportWarning, std::exception
#include "../log/Indenter.hpp"
#include "../res/Index.hpp" // GetIndex, Index::Open
#include "../res/Stream.hpp" // Stream::{~Stream,GetLine}
#include "functional/locale.hpp" // isspace_function
#include "string.hpp" // Partition, Split

namespace page
{
	namespace util
	{
		namespace
		{
			// text-to-phoneme automatic conversion rules
			// based on "Automatic Translation of English Text to Phonetics by
			// Means of Letter-to-Sound Rules," NRL Report 7948, published by
			// National Technical Information Service as document AD/A021 929
			// based on public domain english2phoneme by John A. Wasser
			// http://www.speech.cs.cmu.edu/comp.speech/Section5/Synth/text.phoneme.3.html
			// modified to produce Arpabet phonemes (AX=AH, J=JH, WH=W)
			const char
				anything[] = "",
				nothing[] = " ",
				pause[] = " ",
				silent[] = "";
			struct Rule
			{
				const char *prefix, *match, *suffix, *phoneme;
			};
			const Rule punctRules[] =
			{
				anything, " ", anything, pause,
				anything, "-", anything, silent,
				".", "'S", anything, "Z",
				"#:.E", "'S", anything, "Z",
				"#", "'S", anything, "Z",
				anything, "'", anything, silent,
				anything, ", ", anything, pause,
				anything, ".", anything, pause,
				anything, "?", anything, pause,
				anything, "!", anything, pause,
				anything, 0, anything, silent
			};
			const Rule aRules[] =
			{
				anything, "A", nothing, "AH",
				nothing, "ARE", nothing, "AAR",
				nothing, "AR", "O", "AHR",
				anything, "AR", "#", "EHR",
				"^", "AS", "#", "EYS",
				anything, "A", "WA", "AH",
				anything, "AW", anything, "AO",
				" :", "ANY", anything, "EHNIY",
				anything, "A", "^+#", "EY",
				"#:", "ALLY", anything, "AHLIY",
				nothing, "AL", "#", "AHL",
				anything, "AGAIN", anything, "AHGEHN",
				"#:", "AG", "E", "IHJH",
				anything, "A", "^+:#", "AE",
				" :", "A", "^+ ", "EY",
				anything, "A", "^%", "EY",
				nothing, "ARR", anything, "AHR",
				anything, "ARR", anything, "AER",
				" :", "AR", nothing, "AAR",
				anything, "AR", nothing, "ER",
				anything, "AR", anything, "AAR",
				anything, "AIR", anything, "EHR",
				anything, "AI", anything, "EY",
				anything, "AY", anything, "EY",
				anything, "AU", anything, "AO",
				"#:", "AL", nothing, "AHL",
				"#:", "ALS", nothing, "AHLZ",
				anything, "ALK", anything, "AOK",
				anything, "AL", "^", "AOL",
				" :", "ABLE", anything, "EYBAHL",
				anything, "ABLE", anything, "AHBAHL",
				anything, "ANG", "+", "EYNJH",
				anything, "A", anything, "AE",
				anything, 0, anything, silent
			};
			const Rule bRules[] =
			{
				nothing, "BE", "^#", "BIH",
				anything, "BEING", anything, "BIYIHNG",
				nothing, "BOTH", nothing, "BOWTH",
				nothing, "BUS", "#", "BIHZ",
				anything, "BUIL", anything, "BIHL",
				anything, "B", anything, "B",
				anything, 0, anything, silent
			};
			const Rule cRules[] =
			{
				nothing, "CH", "^", "K",
				"^E", "CH", anything, "K",
				anything, "CH", anything, "CH",
				" S", "CI", "#", "SAY",
				anything, "CI", "A", "SH",
				anything, "CI", "O", "SH",
				anything, "CI", "EN", "SH",
				anything, "C", "+", "S",
				anything, "CK", anything, "K",
				anything, "COM", "%", "KAHM",
				anything, "C", anything, "K",
				anything, 0, anything, silent
			};
			const Rule dRules[] =
			{
				"#:", "DED", nothing, "DIHD",
				".E", "D", nothing, "D",
				"#:^E", "D", nothing, "T",
				nothing, "DE", "^#", "DIH",
				nothing, "DO", nothing, "DUW",
				nothing, "DOES", anything, "DAHZ",
				nothing, "DOING", anything, "DUWIHNG",
				nothing, "DOW", anything, "DAW",
				anything, "DU", "A", "JHUW",
				anything, "D", anything, "D",
				anything, 0, anything, silent
			};
			const Rule eRules[] =
			{
				"#:", "E", nothing, silent,
				"':^", "E", nothing, silent,
				" :", "E", nothing, "IY",
				"#", "ED", nothing, "D",
				"#:", "E", "D ", silent,
				anything, "EV", "ER", "EHV",
				anything, "E", "^%", "IY",
				anything, "ERI", "#", "IYRIY",
				anything, "ERI", anything, "EHRIH",
				"#:", "ER", "#", "ER",
				anything, "ER", "#", "EHR",
				anything, "ER", anything, "ER",
				nothing, "EVEN", anything, "IYVEHN",
				"#:", "E", "W", silent,
				"T", "EW", anything, "UW",
				"S", "EW", anything, "UW",
				"R", "EW", anything, "UW",
				"D", "EW", anything, "UW",
				"L", "EW", anything, "UW",
				"Z", "EW", anything, "UW",
				"N", "EW", anything, "UW",
				"J", "EW", anything, "UW",
				"TH", "EW", anything, "UW",
				"CH", "EW", anything, "UW",
				"SH", "EW", anything, "UW",
				anything, "EW", anything, "YUW",
				anything, "E", "O", "IY",
				"#:S", "ES", nothing, "IHZ",
				"#:C", "ES", nothing, "IHZ",
				"#:G", "ES", nothing, "IHZ",
				"#:Z", "ES", nothing, "IHZ",
				"#:X", "ES", nothing, "IHZ",
				"#:J", "ES", nothing, "IHZ",
				"#:CH", "ES", nothing, "IHZ",
				"#:SH", "ES", nothing, "IHZ",
				"#:", "E", "S ", silent,
				"#:", "ELY", nothing, "LIY",
				"#:", "EMENT", anything, "MEHNT",
				anything, "EFUL", anything, "FUHL",
				anything, "EE", anything, "IY",
				anything, "EARN", anything, "ERN",
				nothing, "EAR", "^", "ER",
				anything, "EAD", anything, "EHD",
				"#:", "EA", nothing, "IYAH",
				anything, "EA", "SU", "EH",
				anything, "EA", anything, "IY",
				anything, "EIGH", anything, "EY",
				anything, "EI", anything, "IY",
				nothing, "EYE", anything, "AY",
				anything, "EY", anything, "IY",
				anything, "EU", anything, "YUW",
				anything, "E", anything, "EH",
				anything, 0, anything, silent
			};
			const Rule fRules[] =
			{
				anything, "FUL", anything, "FUHL",
				anything, "F", anything, "F",
				anything, 0, anything, silent
			};
			const Rule gRules[] =
			{
				anything, "GIV", anything, "GIHV",
				nothing, "G", "I^", "G",
				anything, "GE", "T", "GEH",
				"SU", "GGES", anything, "GJHEHS",
				anything, "GG", anything, "G",
				" B#", "G", anything, "G",
				anything, "G", "+", "JH",
				anything, "GREAT", anything, "GREYT",
				"#", "GH", anything, silent,
				anything, "G", anything, "G",
				anything, 0, anything, silent
			};
			const Rule hRules[] =
			{
				nothing, "HAV", anything, "HAEV",
				nothing, "HERE", anything, "HIYR",
				nothing, "HOUR", anything, "AWER",
				anything, "HOW", anything, "HAW",
				anything, "H", "#", "H",
				anything, "H", anything, silent,
				anything, 0, anything, silent
			};
			const Rule iRules[] =
			{
				nothing, "IN", anything, "IHN",
				nothing, "I", nothing, "AY",
				anything, "IN", "D", "AYN",
				anything, "IER", anything, "IYER",
				"#:R", "IED", anything, "IYD",
				anything, "IED", nothing, "AYD",
				anything, "IEN", anything, "IYEHN",
				anything, "IE", "T", "AYEH",
				" :", "I", "%", "AY",
				anything, "I", "%", "IY",
				anything, "IE", anything, "IY",
				anything, "I", "^+:#", "IH",
				anything, "IR", "#", "AYR",
				anything, "IZ", "%", "AYZ",
				anything, "IS", "%", "AYZ",
				anything, "I", "D%", "AY",
				"+^", "I", "^+", "IH",
				anything, "I", "T%", "AY",
				"#:^", "I", "^+", "IH",
				anything, "I", "^+", "AY",
				anything, "IR", anything, "ER",
				anything, "IGH", anything, "AY",
				anything, "ILD", anything, "AYLD",
				anything, "IGN", nothing, "AYN",
				anything, "IGN", "^", "AYN",
				anything, "IGN", "%", "AYN",
				anything, "IQUE", anything, "IYK",
				anything, "I", anything, "IH",
				anything, 0, anything, silent
			};
			const Rule jRules[] =
			{
				anything, "J", anything, "JH",
				anything, 0, anything, silent
			};
			const Rule kRules[] =
			{
				nothing, "K", "N", silent,
				anything, "K", anything, "K",
				anything, 0, anything, silent
			};
			const Rule lRules[] =
			{
				anything, "LO", "C#", "LOW",
				"L", "L", anything, silent,
				"#:^", "L", "%", "AHL",
				anything, "LEAD", anything, "LIYD",
				anything, "L", anything, "L",
				anything, 0, anything, silent
			};
			const Rule mRules[] =
			{
				anything, "MOV", anything, "MUWV",
				anything, "M", anything, "M",
				anything, 0, anything, silent
			};
			const Rule nRules[] =
			{
				"E", "NG", "+", "NJH",
				anything, "NG", "R", "NGG",
				anything, "NG", "#", "NGG",
				anything, "NGL", "%", "NGGAHL",
				anything, "NG", anything, "NG",
				anything, "NK", anything, "NGK",
				nothing, "NOW", nothing, "NAW",
				anything, "N", anything, "N",
				anything, 0, anything, silent
			};
			const Rule oRules[] =
			{
				anything, "OF", nothing, "AHV",
				anything, "OROUGH", anything, "EROW",
				"#:", "OR", nothing, "ER",
				"#:", "ORS", nothing, "ERZ",
				anything, "OR", anything, "AOR",
				nothing, "ONE", anything, "WAHN",
				anything, "OW", anything, "OW",
				nothing, "OVER", anything, "OWVER",
				anything, "OV", anything, "AHV",
				anything, "O", "^%", "OW",
				anything, "O", "^EN", "OW",
				anything, "O", "^I#", "OW",
				anything, "OL", "D", "OWL",
				anything, "OUGHT", anything, "AOT",
				anything, "OUGH", anything, "AHF",
				nothing, "OU", anything, "AW",
				"H", "OU", "S#", "AW",
				anything, "OUS", anything, "AHS",
				anything, "OUR", anything, "AOR",
				anything, "OULD", anything, "UHD",
				"^", "OU", "^L", "AH",
				anything, "OUP", anything, "UWP",
				anything, "OU", anything, "AW",
				anything, "OY", anything, "OY",
				anything, "OING", anything, "OWIHNG",
				anything, "OI", anything, "OY",
				anything, "OOR", anything, "AOR",
				anything, "OOK", anything, "UHK",
				anything, "OOD", anything, "UHD",
				anything, "OO", anything, "UW",
				anything, "O", "E", "OW",
				anything, "O", nothing, "OW",
				anything, "OA", anything, "OW",
				nothing, "ONLY", anything, "OWNLIY",
				nothing, "ONCE", anything, "WAHNS",
				anything, "ON'T", anything, "OWNT",
				"C", "O", "N", "AA",
				anything, "O", "NG", "AO",
				" :^", "O", "N", "AH",
				"I", "ON", anything, "AHN",
				"#:", "ON", nothing, "AHN",
				"#^", "ON", anything, "AHN",
				anything, "O", "ST ", "OW",
				anything, "OF", "^", "AOF",
				anything, "OTHER", anything, "AHDHER",
				anything, "OSS", nothing, "AOS",
				"#:^", "OM", anything, "AHM",
				anything, "O", anything, "AA",
				anything, 0, anything, silent
			};
			const Rule pRules[] =
			{
				anything, "PH", anything, "F",
				anything, "PEOP", anything, "PIYP",
				anything, "POW", anything, "PAW",
				anything, "PUT", nothing, "PUHT",
				anything, "P", anything, "P",
				anything, 0, anything, silent
			};
			const Rule qRules[] =
			{
				anything, "QUAR", anything, "KWAOR",
				anything, "QU", anything, "KW",
				anything, "Q", anything, "K",
				anything, 0, anything, silent
			};
			const Rule rRules[] =
			{
				nothing, "RE", "^#", "RIY",
				anything, "R", anything, "R",
				anything, 0, anything, silent
			};
			const Rule sRules[] =
			{
				anything, "SH", anything, "SH",
				"#", "SION", anything, "ZHAHN",
				anything, "SOME", anything, "SAHM",
				"#", "SUR", "#", "ZHER",
				anything, "SUR", "#", "SHER",
				"#", "SU", "#", "ZHUW",
				"#", "SSU", "#", "SHUW",
				"#", "SED", nothing, "ZD",
				"#", "S", "#", "Z",
				anything, "SAID", anything, "SEHD",
				"^", "SION", anything, "SHAHN",
				anything, "S", "S", silent,
				".", "S", nothing, "Z",
				"#:.E", "S", nothing, "Z",
				"#:^##", "S", nothing, "Z",
				"#:^#", "S", nothing, "S",
				"U", "S", nothing, "S",
				" :#", "S", nothing, "Z",
				nothing, "SCH", anything, "SK",
				anything, "S", "C+", silent,
				"#", "SM", anything, "ZM",
				"#", "SN", "'", "ZAHN",
				anything, "S", anything, "S",
				anything, 0, anything, silent
			};
			const Rule tRules[] =
			{
				nothing, "THE", nothing, "DHAH",
				anything, "TO", nothing, "TUW",
				anything, "THAT", nothing, "DHAET",
				nothing, "THIS", nothing, "DHIHS",
				nothing, "THEY", anything, "DHEY",
				nothing, "THERE", anything, "DHEHR",
				anything, "THER", anything, "DHER",
				anything, "THEIR", anything, "DHEHR",
				nothing, "THAN", nothing, "DHAEN",
				nothing, "THEM", nothing, "DHEHM",
				anything, "THESE", nothing, "DHIYZ",
				nothing, "THEN", anything, "DHEHN",
				anything, "THROUGH", anything, "THRUW",
				anything, "THOSE", anything, "DHOWZ",
				anything, "THOUGH", nothing, "DHOW",
				nothing, "THUS", anything, "DHAHS",
				anything, "TH", anything, "TH",
				"#:", "TED", nothing, "TIHD",
				"S", "TI", "#N", "CH",
				anything, "TI", "O", "SH",
				anything, "TI", "A", "SH",
				anything, "TIEN", anything, "SHAHN",
				anything, "TUR", "#", "CHER",
				anything, "TU", "A", "CHUW",
				nothing, "TWO", anything, "TUW",
				anything, "T", anything, "T",
				anything, 0, anything, silent
			};
			const Rule uRules[] =
			{
				nothing, "UN", "I", "YUWN",
				nothing, "UN", anything, "AHN",
				nothing, "UPON", anything, "AHPAON",
				"T", "UR", "#", "UHR",
				"S", "UR", "#", "UHR",
				"R", "UR", "#", "UHR",
				"D", "UR", "#", "UHR",
				"L", "UR", "#", "UHR",
				"Z", "UR", "#", "UHR",
				"N", "UR", "#", "UHR",
				"J", "UR", "#", "UHR",
				"TH", "UR", "#", "UHR",
				"CH", "UR", "#", "UHR",
				"SH", "UR", "#", "UHR",
				anything, "UR", "#", "YUHR",
				anything, "UR", anything, "ER",
				anything, "U", "^ ", "AH",
				anything, "U", "^^", "AH",
				anything, "UY", anything, "AY",
				" G", "U", "#", silent,
				"G", "U", "%", silent,
				"G", "U", "#", "W",
				"#N", "U", anything, "YUW",
				"T", "U", anything, "UW",
				"S", "U", anything, "UW",
				"R", "U", anything, "UW",
				"D", "U", anything, "UW",
				"L", "U", anything, "UW",
				"Z", "U", anything, "UW",
				"N", "U", anything, "UW",
				"J", "U", anything, "UW",
				"TH", "U", anything, "UW",
				"CH", "U", anything, "UW",
				"SH", "U", anything, "UW",
				anything, "U", anything, "YUW",
				anything, 0, anything, silent
			};
			const Rule vRules[] =
			{
				anything, "VIEW", anything, "VYUW",
				anything, "V", anything, "V",
				anything, 0, anything, silent
			};
			const Rule wRules[] =
			{
				nothing, "WERE", anything, "WER",
				anything, "WA", "S", "WAA",
				anything, "WA", "T", "WAA",
				anything, "WHERE", anything, "WEHR",
				anything, "WHAT", anything, "WAAT",
				anything, "WHOL", anything, "HOWL",
				anything, "WHO", anything, "HUW",
				anything, "WH", anything, "W",
				anything, "WAR", anything, "WAOR",
				anything, "WOR", "^", "WER",
				anything, "WR", anything, "R",
				anything, "W", anything, "W",
				anything, 0, anything, silent
			};
			const Rule xRules[] =
			{
				anything, "X", anything, "KS",
				anything, 0, anything, silent
			};
			const Rule yRules[] =
			{
				anything, "YOUNG", anything, "YAHNG",
				nothing, "YOU", anything, "YUW",
				nothing, "YES", anything, "YEHS",
				nothing, "Y", anything, "Y",
				"#:^", "Y", nothing, "IY",
				"#:^", "Y", "I", "IY",
				" :", "Y", nothing, "AY",
				" :", "Y", "#", "AY",
				" :", "Y", "^+:#", "IH",
				" :", "Y", "^#", "AY",
				anything, "Y", anything, "IH",
				anything, 0, anything, silent
			};
			const Rule zRules[] =
			{
				anything, "Z", anything, "Z",
				anything, 0, anything, silent
			};
			const Rule *rules[] =
			{
				punctRules,
				aRules, bRules, cRules, dRules, eRules, fRules, gRules,
				hRules, iRules, jRules, kRules, lRules, mRules, nRules,
				oRules, pRules, qRules, rRules, sRules, tRules, uRules,
				vRules, wRules, xRules, yRules, zRules
			};

			// automatically generate phonemes from rules
			std::string GenPhonemes(const std::string &word)
			{
				std::string phonemes;
				// FIXME: implement
				return phonemes;
			}

			// phoneme dictionary
			// one line per key/value entry
			// keys and values separated by whitespace
			// compatible with the CMU Pronouncing Dictionary (cmudict)
			typedef std::unordered_map<std::string, std::string> Dict;
			Dict MakeDict()
			{
				Dict dict;
				std::cout << "loading phonetic dictionary" << std::endl;
				log::Indenter indenter;
				try
				{
					const std::unique_ptr<res::Stream> stream(res::GetIndex().Open("lang/english.dict"));
					for (std::string line;;)
					{
						// ignore comments and empty lines
						std::string line(Trim(Partition(stream->GetLine(), '#').first));
						if (line.empty())
							if (stream->Eof()) break;
							else continue;
						auto entry(Partition(line));
						// ignore punctuation and other symbols
						if (!std::isalnum(entry.first[0])) continue;
						// ignore alternative pronounciations
						if (entry.first.end()[-1] == ')') continue;
						// remove whitespace between phonemes
						entry.second.erase(
							std::remove_if(
								entry.second.begin(), entry.second.end(),
								isspace_function<char>()),
							entry.second.end());
						dict.insert(entry);
					}
				}
				catch (const std::exception &e)
				{
					err::ReportWarning(e);
				}
				return dict;
			}
			const Dict &GetDict()
			{
				static Dict dict(MakeDict());
				return dict;
			}
		}

		// phoneme generation
		std::string GetPhonemes(const std::string &s)
		{
			std::string phonemes;
			typedef std::vector<std::string> Words;
			Words words;
			Split(s, std::back_inserter(words));
			for (Words::const_iterator word(words.begin());; ++word)
			{
				// FIXME: handle punctuation
				Dict::const_iterator entry(GetDict().find(*word));
				if (entry == GetDict().end())
				{
					// FIXME: generate phonemes from text automatically
				}
				else phonemes += entry->second;
				if (word == words.end()) break;
				phonemes.push_back(' ');
			}
			return phonemes;
		}

		// editing
		std::string EditSpeech(const std::string &s)
		{
			// FIXME: implement
			return s;
		}
		std::string EditText(const std::string &s)
		{
			// FIXME: implement
			return s;
		}
	}
}
