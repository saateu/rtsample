#include "RtAudio.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>

int main (int argc, char *argv[]) {
  RtAudio ra;
  RtAudio::DeviceInfo info;
  unsigned int devnum;
  const char *api = NULL;

  if (!argv[1]) {
    std::cout << "No API, exit\n";
    exit (0);
  }

  std::vector< RtAudio::Api > apis;
  RtAudio :: getCompiledApi( apis );

  std::map<int, std::string> apiMap;
  apiMap[RtAudio::MACOSX_CORE] = "OS-X Core Audio";
  apiMap[RtAudio::WINDOWS_ASIO] = "Windows ASIO";
  apiMap[RtAudio::WINDOWS_DS] = "Windows DirectSound";
  apiMap[RtAudio::WINDOWS_WASAPI] = "Windows WASAPI";
  apiMap[RtAudio::UNIX_JACK] = "Jack Client";
  apiMap[RtAudio::LINUX_ALSA] = "Linux ALSA";
  apiMap[RtAudio::LINUX_PULSE] = "Linux PulseAudio";
  apiMap[RtAudio::LINUX_OSS] = "Linux OSS";
  apiMap[RtAudio::RTAUDIO_DUMMY] = "RtAudio Dummy";

  std::cout << "\nCurrent API: " << apiMap[ra.getCurrentApi() ] << std::endl;

  for ( unsigned int i = 0; i<apis.size(); i++ )
    std::cout << "  " << apiMap[ apis[i] ] << std::endl;

  devnum = ra.getDeviceCount();
  if (!devnum) {
    std::cout << "No audio devices found\n" << std::endl;
    exit (0);
  }

  for (unsigned int i = 0; i < devnum; i++) {
    info = ra.getDeviceInfo(i);
    if (strcmp (info.name.c_str(), argv[1]) == 0)
      api = info.name.c_str();
      std::cout << "Using" << api << std::endl;
  }

  if (!api) {
    std::cout << "Can't use " << argv[1] <<std::endl;
  }
}
