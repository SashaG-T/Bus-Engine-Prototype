#ifndef GRAPHICSINTERFACE_H_
    #define GRAPHICSINTERFACE_H_

#include <stdint.h>

/*
#include "src/generic/Point.h"
#include "src/generic/Rectangle.h"
#include "src/engine/FontFormat.h"
*/

enum Flip {
        NO_FLIP,
        VERTICAL_FLIP,
        HORIZONTAL_FLIP,
        VERTICAL_HORIZONTAL_FLIP,
        NUM_OF_FLIP_TYPES
};

struct GraphicsInterface {

        virtual ~GraphicsInterface() {};
/*
        virtual void renderClear() =0;
        virtual void basicRenderCopy(int32_t textureID, int32_t destX, int32_t destY) =0;
        virtual void renderCopy(int32_t textureID, Rectangle const * const sourceRect, Rectangle const * const destinationRect) =0;
        virtual void renderCopy(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight) =0;
        virtual void renderCopyEx(int32_t textureID, Rectangle const * const sourceRect, Rectangle const * const destinationRect, const double angle, const Point& center, const Flip flip) =0;
        virtual void renderCopyEx(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight, const double angle, const Point& center, const Flip flip) =0;
        virtual void renderPresent() =0;
        virtual int32_t getTextureID(const char* filename) =0;
        virtual int32_t loadBMP(const char* filename) =0;
        virtual int32_t loadIMG(const char* filename) =0;
        virtual int32_t incrementTextureReferenceCount(int32_t id) =0;

        virtual int32_t createTexture() =0; //only reserves and returns a textureID.
        virtual int32_t createTexture(int width, int height) =0; //creates a texture and returns it's textureID.
        virtual void drawText(int32_t textureID, const char* text, const char* filename, int size, uint32_t colour, FontFormat format, uint32_t wrapLength) =0;
        virtual void getTextureDimensions(int32_t textureID, int& width, int& height) =0;
        virtual bool getTexturePixels(uint32_t textureID, Rectangle const sourceRect, uint32_t* pixels, uint32_t transparencyColour) =0;
        virtual void textureClear(int32_t textureID, uint32_t colour) =0; //must be done before setRenderTarget .. bug?
        virtual void setRenderTarget(int32_t textureID) =0; //SDL_SetRenderTarget(SDL_Renderer*, SDL_Texture*);

        virtual bool destroyTexture(int32_t textureID) =0;
        virtual bool unloadTexture(int32_t textureID) =0;
        virtual bool validateID(int32_t textureID) =0;

        virtual void renderRectangle(int32_t x, int32_t y, int32_t w, int32_t h) =0;
        virtual void renderCircle(int32_t x, int32_t y, int32_t r) =0;
        virtual void renderLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2) =0;

        virtual Point const & getWindowSize() =0;
        virtual Point const & getDisplaySize() =0;

        virtual bool isFullscreenOn() =0;
        virtual void toggleFullscreen() = 0;

        virtual void setDrawColour(uint32_t colour) =0;
        virtual uint32_t getDrawColour() =0;
*/
        virtual int32_t getFastestRefreshRate() =0; ///Undefined behaviour when called before createWindow()

        virtual void setWindowSize(uint32_t width, uint32_t height) =0;
        virtual void setWindowTitle(const char* title) =0;
        virtual void setFullscreen(bool windowFullscreen) =0;
        virtual void setWindowIcon(const char* filename) =0;

    private:
        virtual void createWindow() =0;

};
/*
struct DummyGraphicsInterface : public GraphicsInterface {

        Point dummySize;

        void renderClear() {};
        void basicRenderCopy(int32_t textureID, int32_t destX, int32_t destY) {};
        void renderCopy(int32_t textureID, Rectangle const * const sourceRect, Rectangle const * const destinationRect) {};
        void renderCopy(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight) {};
        void renderCopyEx(int32_t textureID, Rectangle const * const sourceRect, Rectangle const * const destinationRect, const double angle, const Point& center, const Flip flip) {};
        void renderCopyEx(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight, const double angle, const Point& center, const Flip flip) {};
        void renderPresent() {};
        int32_t getTextureID(const char* filename) {return 0;};
        int32_t loadBMP(const char* filename) {return 0;};
        int32_t loadIMG(const char* filename) {return 0;};
        int32_t incrementTextureReferenceCount(int32_t id) {return 0;};

        int32_t createTexture() {return 0;};
        int32_t createTexture(int width, int height) {return 0;};
        void drawText(int32_t textureID, const char* text, const char* filename, int size, uint32_t colour, FontFormat format, uint32_t wrapLength) {};
        void getTextureDimensions(int32_t textureID, int& width, int& height) {};
        bool getTexturePixels(uint32_t textureID, Rectangle const sourceRect, uint32_t* pixels, uint32_t transparencyColour) {return true;};
        void textureClear(int32_t textureID, uint32_t colour) {};
        void setRenderTarget(int32_t textureID) {};

        bool destroyTexture(int32_t textureID) {return true;};
        bool unloadTexture(int32_t textureID) {return true;};
        bool validateID(int32_t textureID) {return true;};

        void renderRectangle(int32_t x, int32_t y, int32_t w, int32_t h) {};
        void renderCircle(int32_t x, int32_t y, int32_t r) {};
        void renderLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {};

        Point const & getWindowSize() {return dummySize;};
        Point const & getDisplaySize() {return dummySize;};

        bool isFullscreenOn() {return true;};
        void toggleFullscreen() {};

        void setDrawColour(uint32_t colour) {};
        uint32_t getDrawColour() {return 0;};

        int32_t getFastestRefreshRate() {return 60;};

        void setWindowSize(uint32_t width, uint32_t height) {};
        void setWindowTitle(const char* title) {};
        void setFullscreen(bool windowFullscreen) {};
        void setWindowIcon(const char* filename) {};

    private:
        void createWindow() {};

};
*/
#endif // GRAPHICSINTERFACE_H_
