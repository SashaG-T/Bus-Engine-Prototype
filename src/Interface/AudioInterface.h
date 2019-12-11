#ifndef AUDIOINTERFACE_H_
    #define AUDIOINTERFACE_H_

struct AudioInterface {

    virtual ~AudioInterface() {}; //free up all Mix_Chunk pointers in wavSounds.

    virtual int32_t loadWav(const char* filename) =0;
    virtual int32_t loadMus(const char* filename) =0;

    virtual void play(int32_t audioID, int32_t loops) =0;
    virtual void halt(int32_t audioID) =0;

    virtual void pauseMusic() =0;
    virtual void resumeMusic() =0;
    virtual int32_t playingMusic() =0;
    virtual void haltMusic() =0;

    virtual void unloadAudio(int32_t audioID) =0;
    virtual void unloadAudio(const char* filename) =0;
    virtual bool destroyAudio(int32_t audioID) =0;
    virtual void destroyAudio(const char* filename) =0;

    virtual bool validateID(int32_t audioID) =0;

    virtual int32_t getAudioID(const char* filename) =0;

};

struct DummyAudioInterface : public AudioInterface {

    int32_t loadWav(const char* filename) {return 0;};
    int32_t loadMus(const char* filename) {return 0;};
    void play(int32_t audioID, int32_t loops) {};
    void halt(int32_t audioID) {};
    void pauseMusic() {};
    void resumeMusic() {};
    int32_t playingMusic() {return 0;};
    void haltMusic() {};
    void unloadAudio(int32_t audioID) {};
    void unloadAudio(const char* filename) {};
    bool destroyAudio(int32_t audioID) {return true;};
    void destroyAudio(const char* filename) {};
    bool validateID(int32_t audioID) {return true;};
    int32_t getAudioID(const char* filename) {return 0;};

};

#endif // AUDIOINTERFACE_H_
