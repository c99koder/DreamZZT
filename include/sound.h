/* 
	Sound.h
 
	A ZZT music player
 
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
				
			virtual void stop();
			virtual void start();
			
			void setTune(std::string tune);
			
protected:
			virtual GetDataResult getData(uint16 * buffer, int * numSamples);

			float m_note_table[128];
			
private:
			long m_note_len;
			int m_note_freq;
			int m_octave;
			int osc;
			std::string m_tune;
			int m_tune_idx;
			int m_hfreq;
};

#endif	/* __SOUND_H */

