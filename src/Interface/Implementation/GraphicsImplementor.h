#ifndef GRAPHICSIMPLEMENTOR_H_
    #define GRAPHICSIMPLEMENTOR_H_

#include <stdexcept>
#include <memory>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "src/Interface/GraphicsInterface.h"
#include "src/Interface/Implementation/SDLSharedInstance.h"

struct Point {
    int x, y;
    Point() : x(0), y(0) {};
    Point(int x, int y) : x(x), y(y) {};
};

struct GraphicsImplementor : public GraphicsInterface {

        struct TextureDataDeleter {
            void operator()(SDL_Texture* texture) {
                SDL_DestroyTexture(texture);
            }
        };

        struct TextureData {
            std::shared_ptr<SDL_Texture> texture;
            std::string filename;
            int referenceCount;
            TextureData(SDL_Texture* texture, const char* filename)
            :   texture(texture, TextureDataDeleter()),
                filename(filename),
                referenceCount(1)
            {};
        };

        SDL_Window* window;
        int32_t windowID;
        SDL_Renderer* renderer;
        const char* windowTitle;
        uint32_t windowWidth;
        uint32_t windowHeight;
        bool windowFullscreen;

        std::map<int32_t, TextureData> textureDataMap;
        std::priority_queue<int32_t, std::vector<int32_t>, std::greater<int32_t>> idPriorityQueue;   //priority for the next smallest available id. if empty them next is highestID + 1.
        int32_t highestID;                              //highest currently used ID. -1 means no id's used.

        int32_t getNextID();

        Point windowSize;
        Point displaySize;

        int32_t fastestRefreshRate;

        void createWindow();

    public:
        GraphicsImplementor();
        ~GraphicsImplementor();
/*
        void renderClear();
        void basicRenderCopy(int32_t textureID, int32_t destX, int32_t destY);
        void renderCopy(int32_t textureID, Rectangle const * const sourceRect, Rectangle const * const destinationRect);
        void renderCopy(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight);
        void renderCopyEx(int32_t textureID, Rectangle const * const sourceRect, Rectangle const * const destinationRect, const double angle, const Point& center, const Flip flip);
        void renderCopyEx(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight, const double angle, const Point& center, const Flip flip);
        void renderPresent();
        int32_t getTextureID(const char* filename);
        int32_t loadBMP(const char* filename);
        int32_t loadIMG(const char* filename);
        int32_t incrementTextureReferenceCount(int32_t id);

        int32_t createTexture(); //only reserves and returns a textureID.
        int32_t createTexture(int width, int height); //creates a texture and returns it's textureID.
        void drawText(int32_t textureID, const char* text, const char* filename, int size, uint32_t colour, FontFormat format, uint32_t wrapLength);
        void getTextureDimensions(int32_t textureID, int& width, int& height);
        bool getTexturePixels(uint32_t textureID, Rectangle const sourceRect, uint32_t* pixels, uint32_t transparencyColour);
        void textureClear(int32_t textureID, uint32_t colour);
        void setRenderTarget(int32_t textureID); //SDL_SetRenderTarget(SDL_Renderer*, SDL_Texture*);

        bool destroyTexture(int32_t textureID);
        bool unloadTexture(int32_t textureID);
        bool validateID(int32_t textureID);

        void renderRectangle(int32_t x, int32_t y, int32_t w, int32_t h);
        void renderCircle(int32_t x, int32_t y, int32_t r);
        void renderLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

        Point const & getWindowSize();
        Point const & getDisplaySize();

        bool isFullscreenOn();
        void toggleFullscreen();

        void setDrawColour(uint32_t colour);
        uint32_t getDrawColour();
*/
        int32_t getFastestRefreshRate();

        void setWindowSize(uint32_t width, uint32_t height);
        void setWindowTitle(const char* title);
        void setFullscreen(bool windowFullscreen);

        void setWindowIcon(const char* filename);

};

#endif // GRAPHICSIMPLEMENTOR_H_
