#include <stdio.h>
#include <malloc.h>
#include "soundcommon.h"

void SoundSystemInit(u32 rate,u32 buffersize,u8 channel,u8 format)
{
	soundsystem->rate = rate;
	
	if(format == 8) 
		soundsystem->buffersize = buffersize;
	else if(format == 16)
		soundsystem->buffersize = buffersize * sizeof(short);

	soundsystem->mixbuffer = (s8*)malloc(soundsystem->buffersize);
	//soundsystem->soundbuffer = soundsystem->mixbuffer;
	soundsystem->format = format;
	soundsystem->channel = channel;
	soundsystem->prevtimer = 0;
	soundsystem->soundcursor = 0;
	soundsystem->numsamples = 0;
	soundsystem->period = 0x1000000 / rate;
	soundsystem->cmd = INIT;
}
void SoundStartMixer(void)
{
	soundsystem->cmd |= MIX;
}
void SendCommandToArm7(u32 command)
{
    while (REG_IPC_FIFO_CR & IPC_FIFO_SEND_FULL);
    if (REG_IPC_FIFO_CR & IPC_FIFO_ERROR)
    {
        REG_IPC_FIFO_CR |= IPC_FIFO_SEND_CLEAR;
    } 
    
    REG_IPC_FIFO_TX = command;
}
