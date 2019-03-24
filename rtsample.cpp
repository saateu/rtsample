#include "RtAudio.h"
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )

typedef signed short  MY_TYPE;

struct OutputData {
  FILE *fp;
  unsigned int channels;
};

int output (void *outputbuffer, void *inputbuffer,
  unsigned int nBufferFrames, double streamTime, 
  RtAudioStreamStatus status, void *data)
{
  OutputData *odata = (OutputData *) data;

  unsigned int count = fread (outputbuffer, odata->channels * sizeof (MY_TYPE), nBufferFrames, odata->fp);
  if (count < nBufferFrames) {
      unsigned int bytes = (nBufferFrames - count) * odata->channels * sizeof( MY_TYPE );
    unsigned int startByte = count * odata->channels * sizeof( MY_TYPE );
    memset( (char *)(outputbuffer)+startByte, 0, bytes );
    return 1;
  }
  return 0;
}

int main (int argc, char *argv[]) {
  RtAudio ra(RtAudio::LINUX_PULSE);
  unsigned int srate;
  unsigned int bufferframes;
  RtAudio::StreamParameters oParam;
  OutputData data;
  char *file;

  if (argc < 2) {
    std::cout << "No input file\n";
    exit (1);
  }

  file = argv[1];
  printf ("file = %s\n", file);
  data.fp = fopen (file, "rb");
  if ( !data.fp ) {
    std::cout << "Unable to find or open file\n";
    exit (1);
  }
  data.channels = 2;

  /* default*/
  srate = 44100;
  bufferframes = 256;
  oParam.deviceId = 0;
  oParam.nChannels = 2;
  oParam.firstChannel = 0;

  try {
    ra.openStream (&oParam, NULL, RTAUDIO_SINT16, srate, &bufferframes, &output, (void *)&data);
    ra.startStream ();
  }
  catch (RtAudioError& e) {
    std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    goto cleanup;
  } 

  while ( 1 ) {
    SLEEP( 100 ); // wake every 100 ms to check if we're done
    if ( ra.isStreamRunning() == false ) break;
  }

cleanup:
  ra.closeStream ();
}
