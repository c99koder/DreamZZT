/* sound.h - ZZT music stream
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

#ifndef __SOUND_H
#define __SOUND_H

#include <stdio.h>
#include <Tiki/stream.h>

#define U16_0		28927
#define U16_1		36607

class ZZTMusicStream : public Tiki::Audio::Stream {
public:
			ZZTMusicStream();
			virtual ~ZZTMusicStream();
				
			void setTune(std::string tune);
			void appendTune(std::string tune);
			
			void lock() { m_locked = true; }
			void unlock() { m_locked = false; }
			bool isLocked() { return m_locked; }
			
protected:
			virtual GetDataResult getData(uint16 * buffer, int * numSamples);

			float m_note_table[128];
			
private:
			double m_note_len;
			float m_note_freq;
			float m_note_duration;
			int m_octave;
			int osc;
			std::string m_tune;
			int m_tune_idx;
			int m_hfreq;
			bool m_locked;
			int m_drum_idx;
			int m_drum;
};

#endif	/* __SOUND_H */

