/* sound.cpp - ZZT music stream
 * Copyright (c) 2000-2006 Sam Steele
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

short drums[10][10] = {
	{   0,   0, 175, 175, 100,  90,  80,  70,  60,  50},  /* 0 */
	{ 500, 300, 520, 320, 540, 340, 550, 350, 540, 340},  /* 1 */
	{1000,1200,1250,1400,1100,1150,1300,1000,1200, 500},  /* 2 */
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  /* 3 (not a sound) */
	{ 950,1950, 750,1750, 550,1550, 350,1350, 150,1150},  /* 4 */
	{ 200, 210, 220, 230, 240, 250, 260, 270, 280, 600},  /* 5 */
	{ 900, 800, 700, 600, 500, 400, 300, 200, 100,   0},  /* 6 */
	{ 300, 200, 290, 190, 280, 180, 270, 170, 260, 160},  /* 7 */
	{ 400, 380, 360, 340, 320, 300, 280, 260, 250, 240},  /* 8 */
	{ 150, 100, 140,  90, 130,  80, 120,  70, 110,  60}   /* 9 */
};

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
	m_drum = -1;
	
	m_locked = false;
}

ZZTMusicStream::~ZZTMusicStream() {
	stop();
}

void ZZTMusicStream::setTune(std::string tune) {
	if(m_locked) return;
	
	stop();
	
	m_tune = tune;
	m_tune_idx = 0;
	m_note_len=0;
	m_octave = 5;
	m_hfreq = 0;
	osc = 1;
	m_drum = -1;
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
			if(m_drum != -1) {
				m_drum_idx++;
				if(m_drum_idx < 10) {
					m_note_freq = drums[m_drum][m_drum_idx];
					m_note_len = 0.002 * (float)m_freq;
					i--;
					continue;
				} else {
					m_drum = -1;
					m_note_freq = 0;
					m_note_len = ((float)m_freq*2.0f / (float)(m_note_duration)) - (0.002 * (float)m_freq);
					i--;
					continue;
				}
			}
			if(m_tune_idx >= m_tune.length()) {
				*numSamples = (i / 2);
				return GDSuccess;
			}
			j=0;
			osc=1;

			if(m_tune[m_tune_idx] >= '0' && m_tune[m_tune_idx] <= '9') {
				m_drum = m_tune[m_tune_idx] - '0';
				m_drum_idx = 0;
				m_tune_idx++;
				i--;
				continue;
			}
			
			switch(m_tune[m_tune_idx++]) {
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
