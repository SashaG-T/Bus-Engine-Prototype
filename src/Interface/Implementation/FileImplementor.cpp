#include "src/SDLBackend/FileImplementor.h"

FileImplementor::FileImplementor()
:   organizationName("org"),
    applicationName("app")
{
    SDLSharedInstance::init();
}

FileImplementor::~FileImplementor() {

    for(auto iter = fileList.begin(); iter != fileList.end(); iter++) {

        SDL_RWops* rw = *iter;
        if(rw) {

            SDL_RWclose(rw);
        }
    }

    SDLSharedInstance::quit();

}

void FileImplementor::setOrganizationName(const char* organizationName) {
    this->organizationName = organizationName;
    preferencePath = SDL_GetPrefPath(organizationName, applicationName);
}

void FileImplementor::setApplicationName(const char* applicationName) {
    this->applicationName = applicationName;
    preferencePath = SDL_GetPrefPath(organizationName, applicationName);
}

char* FileImplementor::getPreferencePath() {
    return preferencePath;
}

std::map<std::string, std::string> FileImplementor::loadStringMap(std::iostream& stream) {
    std::map<std::string, std::string> map;
    std::string line;
    while(std::getline(stream, line)) {
        std::stringstream pairStream(line);
        std::string keyString;
        std::string valueString;
        pairStream >> keyString;
        std::getline(pairStream, valueString);
        //ltrim value
        valueString.erase(valueString.begin(), std::find_if(valueString.begin(), valueString.end(), [](int ch){return !std::isspace(ch);}));
        map.insert(std::pair<std::string, std::string>(keyString, valueString));
    }
    return map;
}

int32_t FileImplementor::getNextID() {

    int32_t nextID = fileList.size();
    if(!freeIDStack.isEmpty()) {
        nextID = freeIDStack.pop();
    } else {
        fileList.push_back(0);
    }
    return nextID;

}

int32_t FileImplementor::openMem(void* mem, int size) {

    int32_t fileID = getNextID();
    fileList.at(fileID) = SDL_RWFromMem(mem, size);
    return fileID;

}

int32_t FileImplementor::openFile(const char* filename, const char* mode) {

    int32_t fileID = getNextID();
    fileList.at(fileID) = SDL_RWFromFile(filename, mode);
    return fileID;

}

int32_t FileImplementor::closeFile(int32_t fileID) {

    int32_t err = 0;
    if(validateID(fileID)) {

        err = SDL_RWclose(fileList.at(fileID)); ///NOT SURE IF SDL_RWclose SET PTR TO NULL.
        fileList.at(fileID) = 0;
        freeIDStack.push(fileID);
    }
    return err;

}

size_t FileImplementor::read(int32_t fileID, void* buffer, size_t size, size_t maxRead) {

    size_t numObjectsRead = 0;
    if(validateID(fileID)) {

        numObjectsRead = SDL_RWread(fileList.at(fileID), buffer, size, maxRead);
    }
    return numObjectsRead;

}

size_t FileImplementor::write(int32_t fileID, void* buffer, size_t size, size_t maxWrite) {

    size_t numObjectsWrote = 0;
    if(validateID(fileID)) {

        numObjectsWrote = SDL_RWwrite(fileList.at(fileID), buffer, size, maxWrite);
    }
    return numObjectsWrote;

}

int64_t FileImplementor::seek(int32_t fileID, int64_t offset, int whence) {

    int64_t offsetToEOF(-2);
    if(validateID(fileID)) {

        offsetToEOF = SDL_RWseek(fileList.at(fileID), offset, whence);
    }
    return offsetToEOF;

}

size_t FileImplementor::size(int32_t fileID) {

    size_t size = 0;
    if(validateID(fileID)) {

        size = SDL_RWsize(fileList.at(fileID));
    }
    return size;

}

int32_t FileImplementor::validateID(int32_t fileID) {
    return fileID >= 0 && fileID < (int32_t)fileList.size(); ///Maybe casting to fileList.size() isn't the way to go?
}
