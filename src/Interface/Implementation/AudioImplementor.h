#ifndef AUDIOIMPLEMENTOR_H_
    #define AUDIOIMPLEMENTOR_H_

#include <stdexcept>
#include <sstream>
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/engine/AudioInterface.h"
#include "src/SDLBackend/SDLSharedInstance.h"

class AudioImplementor : public AudioInterface {

    private:

        struct ChunkDeleter {
            void operator()(void* ptr) {
                Mix_FreeChunk((Mix_Chunk*)ptr);
            }
        };

        struct MusicDeleter {
            void operator()(void* ptr) {
                Mix_FreeMusic((Mix_Music*)ptr);
            }
        };

        struct AudioData {
            bool isWav;
            std::shared_ptr<void> audioPtr;
            std::string filename;
            int referenceCount;
            AudioData(bool isWav, void* ptr, const char* filename)
            :   isWav(isWav),
                filename(filename),
                referenceCount(1)
            {
                if(isWav) {
                    audioPtr = std::shared_ptr<void>(ptr, ChunkDeleter());
                } else {
                    audioPtr = std::shared_ptr<void>(ptr, MusicDeleter());
                }
            };
        };

        std::map<int32_t, AudioData> audioDataMap;
        std::priority_queue<int32_t> idPriorityQueue;
        int32_t highestID;

        int32_t getNextID();

    public:
        AudioImplementor(); //setup.
        ~AudioImplementor(); //free up all Mix_Chunk pointers in wavSounds.

        int32_t loadWav(const char* filename);
        int32_t loadMus(const char* filename);

        void play(int32_t audioID, int32_t loops);
        void halt(int32_t audioID);

        void pauseMusic();
        void resumeMusic();
        int32_t playingMusic();
        void haltMusic();

        void unloadAudio(int32_t audioID);
        void unloadAudio(const char* filename);
        bool destroyAudio(int32_t audioID);
        void destroyAudio(const char* filename);

        bool validateID(int32_t audioID);

        int32_t getAudioID(const char* filename);

};

#endif // AUDIOIMPLEMENTOR_H_
