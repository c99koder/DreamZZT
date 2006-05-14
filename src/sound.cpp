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
		m_note_table[x] = 8.1758 * pow(2,float((float)x/12.0f));
	}
	
	osc = 1;

	m_tune = "";
	m_tune_idx = 0;
	
	m_note_len=0;
	m_octave = 4;
	m_hfreq = 0;
	
	Stream::create();
}

ZZTMusicStream::~ZZTMusicStream() {
	stop();
}

void ZZTMusicStream::setTune(std::string tune) {
	m_tune += tune;
}

void ZZTMusicStream::start() {
	m_tune_idx = 0;
	m_note_len=0;
	m_octave = 4;
	m_hfreq = 0;
	osc = 1;
	
	Stream::start();
}

ZZTMusicStream::GetDataResult ZZTMusicStream::getData(uint16 * buffer, int * numSamples) {
	static int j=0;
	int note;
	
	if(m_tune_idx >= m_tune.length()) {
		m_tune = "";
		stop();
		return GDSuccess;
	}
	
	for(int i = 0; i < *numSamples; i++) {
		if(m_note_len--<=0) {
			if(m_tune_idx >= m_tune.length()) {
				*numSamples = i;
				printf("End of stream reached (%i samples)\n", i);
				return GDSuccess;
			}
			j=0;
			printf("[%i / %i] Playing: %c",m_tune_idx,m_tune.length(),m_tune[m_tune_idx]);
			switch(m_tune[m_tune_idx++]) {
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
					printf("\n");
					continue;
				case '-':
					m_octave--;
					i--;
					printf("\n");
					continue;
				default:
					printf("Bad note\n");
					i--;
					continue;
			}
			if(m_tune[m_tune_idx] == '#') {
				note++;
				printf("#"); 
				m_tune_idx++;
			}
			if(m_tune[m_tune_idx] == '!') {
				note--;
				printf("!");
				m_tune_idx++;
			}
			printf("\n");
			m_note_freq = m_note_table[note];
			m_note_len = (1.760 / 32.0) * (float)m_freq ;
			printf("Octave: %i Note: %i Len: %i\n",m_octave, note, m_note_len);
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
	printf("Buffer filled\n");
	return GDSuccess;
}

void ZZTMusicStream::stop() {
	if (!isPlaying())
		return;
	
	Stream::stop();
}