#ifndef FILEINTERFACE_H_
    #define FILEINTERFACE_H_

#include <stdint.h>
#include <memory>
#include <string>
#include <map>

struct FileInterface {

        virtual ~FileInterface() {};

        virtual char* getPreferencePath() =0; //get us our "safe" file path were we can save stuff.
        ///MAYBE THE STD::MAP IS BREAKING IT....
        virtual std::map<std::string, std::string> loadStringMap(std::iostream& stream) =0; //return std::map<string, string> of a match pairs file.

        virtual int32_t openMem(void* mem, int size) =0;
        virtual int32_t openFile(const char* filename, const char* mode) =0; ///I feel like the mode should be changed to a value.
        virtual int32_t closeFile(int32_t fileID) =0;
        virtual size_t read(int32_t fileID, void* buffer, size_t size, size_t maxRead) =0;
        virtual size_t write(int32_t fileID, void* buffer, size_t size, size_t maxWrite) =0;
        virtual int64_t seek(int32_t fileID, int64_t offset, int whence) =0; //-1: internal implementor failure, -2: invalid fileID

        virtual size_t size(int32_t fileID) =0;

        virtual int32_t validateID(int32_t fileID) =0; //check if it's within bounds of fileList.

        virtual void setOrganizationName(const char* organizationName) =0;
        virtual void setApplicationName(const char* applicationName) =0;

};

struct DummyFileInterface : public FileInterface {

        char dummyPath[1];

        DummyFileInterface()
        :   dummyPath{0}
        {}
        char* getPreferencePath() {return dummyPath;};
        virtual std::map<std::string, std::string> loadStringMap(std::iostream& stream) {return std::map<std::string, std::string>();};

        virtual int32_t openMem(void* mem, int size) {return 0;};
        virtual int32_t openFile(const char* filename, const char* mode) {return 0;};
        virtual int32_t closeFile(int32_t fileID) {return 0;};
        virtual size_t read(int32_t fileID, void* buffer, size_t size, size_t maxRead) {return 0;};
        virtual size_t write(int32_t fileID, void* buffer, size_t size, size_t maxWrite) {return 0;};
        virtual int64_t seek(int32_t fileID, int64_t offset, int whence) {return 0;};

        virtual size_t size(int32_t fileID) {return 0;};

        virtual int32_t validateID(int32_t fileID) {return 1;};

        virtual void setOrganizationName(const char* organizationName) {};
        virtual void setApplicationName(const char* applicationName) {};

};

#endif // FILEINTERFACE_H_
