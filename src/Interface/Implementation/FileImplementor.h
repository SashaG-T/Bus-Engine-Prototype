#ifndef FILEIMPLEMENTOR_H_
    #define FILEIMPLEMENTOR_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>

#include "src/engine/FileInterface.h"
#include "src/engine/Engine.h"
#include "src/SDLBackend/SDLSharedInstance.h"
#include "src/generic/GenericStack.h"

#define FILE_SEEK_SET RW_SEEK_SET
#define FILE_SEEK_CUR RW_SEEK_CUR
#define FILE_SEEK_END RW_SEEK_END

///modes should be changed to a enum defined by the FileInterface.

class FileImplementor : public FileInterface {

    private:
        std::vector<SDL_RWops*> fileList;
        Stack<int32_t> freeIDStack;

        int32_t getNextID();

        const char* organizationName;
        const char* applicationName;
        const char* preferenceFilename;

        char* preferencePath;

        void setOrganizationName(const char* organizationName);
        void setApplicationName(const char* applicationName);

    public:
        FileImplementor();
        ~FileImplementor();

        char* getPreferencePath(); //get us our "safe" file path were we can save stuff.
        std::map<std::string, std::string> loadStringMap(std::iostream& stream);

        int32_t openMem(void* mem, int size);
        int32_t openFile(const char* filename, const char* mode); ///I feel like the mode should be changed to a value.
        int32_t closeFile(int32_t fileID);
        size_t read(int32_t fileID, void* buffer, size_t size, size_t maxRead);
        size_t write(int32_t fileID, void* buffer, size_t size, size_t maxWrite);
        int64_t seek(int32_t fileID, int64_t offset, int whence);

        size_t size(int32_t fileID);

        int32_t validateID(int32_t fileID); //check if it's within bounds of fileList.

};

#endif // FILEIMPLEMENTOR_H_
