#include "RtAudio.h"
#include <stdio.h>
#include <cstdlib>

struct OutputData {
  FILE *fp;
  unsigned int channels;
};

int output (void *outputbuffer, void *inputbuffer,
  unsigned int nBufferFrames, double streamTime, 
  RtAudioStreamStatus status, void *data)
{
  std::cout << "output" << std::endl;
}

int main (int argc, char *argv[]) {
  RtAudio ra(RtAudio::LINUX_PULSE);
  unsigned int srate;
  unsigned int bufferframes;
  RtAudio::StreamParameters oParam;
  OutputData data;

  if (argc < 2) {
    std::cout << "No input file\n";
    exit (1);
  }

  data.fp = fopen (argv[1], "rb");
  if ( !data.fp ) {
    std::cout << "Unable to find or open file\n";
    exit (1);
  }

  /* default*/
  srate = 44100;
  bufferframes = 256;
  oParam.deviceId = 0;
  oParam.nChannels = 2;

  try {
    ra.openStream (&oParam, NULL, RTAUDIO_SINT16, srate, &bufferframes, &output, &data);
    ra.startStream ();
  }
  catch (RtAudioError& e) {
    std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    goto cleanup;
  } 

cleanup:
  ra.closeStream ();
}
