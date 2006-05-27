/*
 *  sound.cpp
 *  DreamZZT
 *
 *  Created by Sam Steele on 5/13/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#include <assert.h>
#include <string.h>
#include <Tiki/tiki.h>
#include <Tiki/stream.h>

using namespace Tiki;
using namespace Tiki::Audio;

#include "sound.h"

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
}

ZZTMusicStream::~ZZTMusicStream() {
	stop();
}

void ZZTMusicStream::setTune(std::string tune) {
	stop();
	
	m_tune = tune;
	m_tune_idx = 0;
	m_note_len=0;
	m_octave = 5;
	m_hfreq = 0;
	osc = 1;
	m_note_duration = 32;
	
	Stream::create();	
}

void ZZTMusicStream::appendTune(std::string tune) {
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
		if(m_note_len--<=0) {
			if(m_tune_idx >= m_tune.length()) {
				*numSamples = (i / 2);
				return GDSuccess;
			}
			j=0;
			osc=1;

			switch(m_tune[m_tune_idx++]) {
				case '@':
					m_note_duration = 32;
					m_octave = 5;
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
