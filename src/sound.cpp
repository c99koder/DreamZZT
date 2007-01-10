/* sound.cpp - ZZT music stream
 * Copyright (C) 2000 - 2007 Sam Steele
 *
 * This file is part of DreamZZT.
 *
 * DreamZZT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * DreamZZT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */ 

#include <assert.h>
#include <string.h>
#include <Tiki/tiki.h>
#include <Tiki/stream.h>

using namespace Tiki;
using namespace Tiki::Audio;

#include "sound.h"
#include "drums.h"

#include <machine/endian.h>
#include <algorithm>

#define ByteSwap5(x) ByteSwap((unsigned char *) &x,sizeof(x))   

inline void ByteSwap(void * b, int n)
{
#if BYTE_ORDER == LITTLE_ENDIAN
	register int i = 0;
	register int j = n-1;
	while (i<j)
	{
		std::swap(((unsigned char *)b)[i], ((unsigned char *)b)[j]);
		i++, j--;
	}
#endif
}

ZZTMusicStream::ZZTMusicStream() {
	setFrequency(44100);
	setChannelCount(1);
	
	for(int x=0; x<128; x++) {
		m_note_table[x] = 8.1758 * pow((float)2,float((float)x/12.0f));
	}
	
	osc = 1;

	m_tune = "";
	m_tune_idx = 0;
	
	m_note_len=0;
	m_octave = 4;
	m_hfreq = 0;
	m_drum = NULL;
	
	m_locked = false;
	Stream::create();	
}

ZZTMusicStream::~ZZTMusicStream() {
	stop();
}

void ZZTMusicStream::setTune(std::string tune) {
	if(m_locked) return;
#if TIKI_PLAT == TIKI_DC
	return;
#endif
	stop();
	
	m_tune = tune;
	m_tune_idx = 0;
	m_note_len=0;
	m_octave = 5;
	m_hfreq = 0;
	osc = 1;
	m_drum = NULL;
	m_note_duration = 32;
}

void ZZTMusicStream::appendTune(std::string tune) {
#if TIKI_PLAT == TIKI_DC
	return;
#endif
	m_tune += "@"; //Magic reset code
	m_tune += tune;
}

ZZTMusicStream::GetDataResult ZZTMusicStream::getData(uint16 * buffer, int * numSamples) {
	static int j=0;
	int note;

	memset(buffer, 0, *numSamples * 2);
	
	if(m_tune.length() == 0) {
		return GDEOS;
	}
	
	if(m_tune_idx >= m_tune.length()) {
		m_tune = "";
		return GDSuccess;
	}
	
	for(int i = 0; i < *numSamples; i++) {
		if(m_drum != NULL) {
			if(m_drum_idx < (drumsample_size / 2)) {
				buffer[i] = m_drum[m_drum_idx++];
				ByteSwap(&buffer[i],2);
				continue;
			} else {
				m_drum = NULL;
				m_note_freq = 0;
				m_note_len = ((float)m_freq*2.0f / (float)(m_note_duration)) - (drumsample_size / 2);
			}
		}
		
		if(m_note_len--<=0) {
			if(m_tune_idx >= m_tune.length()) {
				*numSamples = (i / 2);
				return GDSuccess;
			}
			j=0;
			osc=1;

			switch(m_tune[m_tune_idx++]) {
				case '0':
					m_drum = (uint16 *)drumsample0;
					m_drum_idx = 0;
					i--;
					continue;
				case '1':
					m_drum = (uint16 *)drumsample1;
					m_drum_idx = 0;
					i--;
					continue;
				case '2':
					m_drum = (uint16 *)drumsample2;
					m_drum_idx = 0;
					i--;
					continue;					
				/*case '3':
					m_drum = (uint16 *)drumsample3;
					m_drum_idx = 0;
					i--;
					continue;*/
				case '4':
					m_drum = (uint16 *)drumsample4;
					m_drum_idx = 0;
					i--;
					continue;
				case '5':
					m_drum = (uint16 *)drumsample5;
					m_drum_idx = 0;
					i--;
					continue;
				case '6':
					m_drum = (uint16 *)drumsample6;
					m_drum_idx = 0;
					i--;
					continue;					
				case '7':
					m_drum = (uint16 *)drumsample7;
					m_drum_idx = 0;
					i--;
					continue;
				case '8':
					m_drum = (uint16 *)drumsample8;
					m_drum_idx = 0;
					i--;
					continue;
				case '9':
					m_drum = (uint16 *)drumsample9;
					m_drum_idx = 0;
					i--;
					continue;
				case '@':
					m_note_len=0;
					m_octave = 5;
					m_hfreq = 0;
					osc = 1;
					m_note_duration = 32;
					i--;
					continue;
				case 'c':
					note = (m_octave * 12); 
					break;
				case 'd':
					note = (m_octave * 12) + 2; 
					break;
				case 'e':
					note = (m_octave * 12) + 4; 
					break;
				case 'f':
					note = (m_octave * 12) + 5; 
					break;
				case 'g':
					note = (m_octave * 12) + 7; 
					break;
				case 'a':
					note = (m_octave * 12) + 9; 
					break;
				case 'b':
					note = (m_octave * 12) + 11; 
					break;
				case '+':
					m_octave++;
					i--;
					continue;
				case '-':
					m_octave--;
					i--;
					continue;
				case 't': 
					m_note_duration=32;
					i--;
					continue;
				case 's': 
					m_note_duration=16;
					i--;
					continue;
				case 'i': 
					m_note_duration=8;
					i--;
					continue;
				case 'q':
					m_note_duration=4;
					i--;
					continue;
				case 'h': 
					m_note_duration=2;
					i--;
					continue;
				case 'w': 
					m_note_duration=1;
					i--;
					continue;
				case '3': 
					m_note_duration/=3.0f;
					i--;
					continue;
				case '.':	
					m_note_duration-=((float)m_note_duration/4.0f);
					i--;
					continue;
				case 'x':
					m_note_freq = 0;
					m_note_len = (float)m_freq*2.0f / (float)(m_note_duration);
					i--;
					continue;
				default:
					i--;
					continue;
			}
			if(m_tune[m_tune_idx] == '#') {
				note++;
				m_tune_idx++;
			}
			if(m_tune[m_tune_idx] == '!') {
				note--;
				m_tune_idx++;
			}
			m_note_freq = m_note_table[note];
			m_note_len = (float)m_freq*2.0f / (float)(m_note_duration);
		}
		
		if(m_note_freq == 0) {
			buffer[i] = 0;
			continue;
		}
		
		m_hfreq = m_freq / m_note_freq / 2.0f;
		
		if(j++>m_hfreq) {
			osc ^= 1;
			j = 0;
		}
		if(osc) {
			buffer[i] = U16_1;
		} else {
			buffer[i] = U16_0;
		}
	}
	return GDSuccess;
}
