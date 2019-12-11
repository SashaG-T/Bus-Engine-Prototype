#include "src/SDLBackend/AudioImplementor.h"

AudioImplementor::AudioImplementor()
:   highestID(-1)
{

    SDLSharedInstance::init();

    //initialize audio.
    int audioRate = 44100;
    Uint16 audioFormat = AUDIO_S16SYS;
    int audioChannels = 2;
    int audioBuffers = 512;

    if(Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers) != 0) {

        fprintf(stderr, "Error: Unable to initialize audio: %s\n", Mix_GetError());
        std::stringstream ss;
        ss << "Error: Unable to initialize audio: \n" << Mix_GetError();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                         "Generic Jumper | Sauteur G\u00e9n\u00e9rique",
                         ss.str().c_str(),
                         NULL);
        exit(1); //for now...
    }

    atexit(Mix_Quit);

}

AudioImplementor::~AudioImplementor() {
    audioDataMap = std::map<int32_t, AudioData>(); //erase everything...
    Mix_Quit();
    SDLSharedInstance::quit();
}

int32_t AudioImplementor::getNextID() {

    int32_t id;
    if(idPriorityQueue.size() == 0) {
        id = ++highestID;
    } else {
        id = idPriorityQueue.top();
        idPriorityQueue.pop();
    }
    return id;
}

int32_t AudioImplementor::getAudioID(const char* filename) {

    int32_t audioID = -1;
    bool notFound = true;
    for(auto iter = audioDataMap.begin(); iter != audioDataMap.end() && notFound; iter++) {
        AudioData& audioData(iter->second);
        std::string audioFilename(iter->second.filename);
        if(audioFilename.compare(filename) == 0) {
            notFound = false;
            audioID = iter->first;
        }
    }
    return audioID;
}

bool AudioImplementor::validateID(int32_t audioID) {
    return audioDataMap.find(audioID) != audioDataMap.end();
}

int32_t AudioImplementor::loadWav(const char* filename) {

    Mix_Chunk* chunk = 0;
    int32_t audioID = getAudioID(filename);

    if(!validateID(audioID)) {
        chunk = Mix_LoadWAV(filename);
        if(chunk) {
            audioID = getNextID();
            audioDataMap.insert(std::pair<int32_t, AudioData>(audioID, AudioData(true, (void*)chunk, filename)));
        } else {
            std::stringstream ss;
            ss << "Audio Interface: loadWav: Mix_LoadWAV failure: Check parameters.\n";
            ss << "\t-> Does the file \"" << filename << "\" exist. Is it corrupt?\n";
            throw std::runtime_error(ss.str());
        }
    } else {
        AudioData& audioData(audioDataMap.at(audioID));
        audioData.referenceCount++;
    }
    return audioID;
}

int32_t AudioImplementor::loadMus(const char* filename) {

    Mix_Music* musx = 0;
    int32_t audioID = getAudioID(filename);

    if(!validateID(audioID)) {
        musx = Mix_LoadMUS(filename);
        if(musx) {
            audioID = getNextID();
            audioDataMap.insert(std::pair<int32_t, AudioData>(audioID, AudioData(false, (void*)musx, filename)));
        } else {
            std::stringstream ss;
            ss << "Audio Interface: loadMus: Mix_LoadMUS failure: Check parameters.\n";
            ss << "\t-> Does the file \"" << filename << "\" exist. Is it corrupt?\n";
            throw std::runtime_error(ss.str());
        }
    } else {
        AudioData& audioData(audioDataMap.at(audioID));
        audioData.referenceCount++;
    }
    return audioID;
}

void AudioImplementor::play(int32_t audioID, int32_t loops) {

    if(validateID(audioID)) {

        AudioData& audioData(audioDataMap.at(audioID));

        if(audioData.isWav) {

            Mix_Chunk* chunk((Mix_Chunk*)audioData.audioPtr.get());
            int32_t channel = Mix_PlayChannel(-1, chunk, loops);
            if(channel == -1) {
                std::stringstream ss;
                ss << "Audio Interface: play: Mix_PlayChannel failure: Play back failure.";
                throw std::runtime_error(ss.str());
            }
        } else {

            Mix_Music* music((Mix_Music*)audioData.audioPtr.get());
            int32_t result = Mix_PlayMusic(music, loops);
            if(result == -1) {
                std::stringstream ss;
                ss << "Audio Interface: play: Mix_PlayMusic failure: Play back failure.";
                throw std::runtime_error(ss.str());
            }
        }

    }

}

void AudioImplementor::halt(int32_t audioID) {
    if(validateID(audioID)) {
        AudioData& audioData(audioDataMap.at(audioID));
        if(audioData.isWav) {
            Mix_HaltChannel(-1);
        } else {
            std::stringstream ss;
            ss << "Audio Interface: halt exception:\n";
            ss << "\t->The audio ID " << audioID << " does not reference a wav file.";
            ss << "\t  Use function pauseMusic() if you want to stop a non wav file's playback.";
            throw std::logic_error(ss.str());
        }
    }
}

void AudioImplementor::pauseMusic() {
    Mix_PauseMusic();
}

void AudioImplementor::resumeMusic() {
    Mix_ResumeMusic();
}

int32_t AudioImplementor::playingMusic() {
    return Mix_PlayingMusic();
}

void AudioImplementor::haltMusic() {
    Mix_HaltMusic();
}

void AudioImplementor::unloadAudio(int32_t audioID) {

    if(validateID(audioID)) {

        AudioData& audioData(audioDataMap.at(audioID));
        if(audioData.referenceCount-- < 1) {
            destroyAudio(audioID);
        }
    }
}

void AudioImplementor::unloadAudio(const char* filename) {

    unloadAudio(getAudioID(filename));
}

bool AudioImplementor::destroyAudio(int32_t audioID) {
    bool found = validateID(audioID);
    if(found) {
        audioDataMap.erase(audioID);
        idPriorityQueue.push(audioID);
        if(audioID == highestID) {
            //find new highest used id.
            int highestID = -1;
            for(auto& iter : audioDataMap) {
                if(iter.first > highestID) {
                    highestID = iter.first;
                }
            }
            //remove all unused id's that are higher than the highest used it from the priority queue.
            std::vector<int32_t> lowerIDs;
            while(idPriorityQueue.size() > 0) {
                int32_t id = idPriorityQueue.top();
                idPriorityQueue.pop();
                if(id <= highestID) {
                    lowerIDs.push_back(id);
                }
            }
            for(auto& iter : lowerIDs) {
                idPriorityQueue.push(iter);
            }
        }
    }
    return found;
}

void AudioImplementor::destroyAudio(const char* filename) {

    destroyAudio(getAudioID(filename));
}


