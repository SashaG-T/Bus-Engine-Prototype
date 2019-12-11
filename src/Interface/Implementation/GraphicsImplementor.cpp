#include "src/Interface/Implementation/GraphicsImplementor.h"

GraphicsImplementor::GraphicsImplementor()
:   window(0),
    renderer(0),
    highestID(-1),
    windowSize(0, 0),
    windowTitle("App"),
    windowWidth(640),
    windowHeight(480),
    windowFullscreen(false)
{
    SDLSharedInstance::init();
/*
    if(TTF_Init()) {

            fprintf(stderr, "Error: SDL_ttf could not be initialized: %s\n", TTF_GetError());
            std::stringstream ss;
            ss << "Error: SDL_ttf could not be initialized: \n" << TTF_GetError();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Generic Jumper | Sauteur G\u00e9n\u00e9rique",
                             ss.str().c_str(),
                             NULL);
            exit(1);
    }

    atexit(TTF_Quit);

    int flags = IMG_INIT_PNG;
    if((IMG_Init(flags)&flags) != flags) {

            fprintf(stderr, "Error: SDL_image could not be initialized: %s\n", IMG_GetError());
            std::stringstream ss;
            ss << "Error: SDL_image could not be initialized: \n" << IMG_GetError();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Generic Jumper | Sauteur G\u00e9n\u00e9rique",
                             ss.str().c_str(),
                             NULL);
            exit(1);
    }

    atexit(IMG_Quit);
*/

    SDL_Rect bounds;
    SDL_GetDisplayBounds(0, &bounds);
    displaySize.x = bounds.w;
    displaySize.y = bounds.h;
}

void GraphicsImplementor::createWindow() {

    int flag = 0;
    if(windowFullscreen) {
        flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    window = SDL_CreateWindow(windowTitle,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight,
                              SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | flag);

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC |
                                  SDL_RENDERER_TARGETTEXTURE);

    if(window == NULL) {
        //fprintf(stderr, "Error: Could not create a Window.\n");
        exit(1);
    }
    if(renderer == NULL) {
        //fprintf(stderr, "Error: Could not create a renderer.\n");
        exit(1);
    }

    //Set render draw colour
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(renderer, windowSize.x, windowSize.y);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    windowID = SDL_GetWindowID(window);

    SDL_DisplayMode displayMode;
    int32_t bestRate = 60; //Deafult refresh rate. (FPS)
    int32_t numOfDisplays = SDL_GetNumVideoDisplays();
    for(int i = 0; i < numOfDisplays; i++) {
        SDL_GetDesktopDisplayMode(i, &displayMode);
        if(displayMode.refresh_rate > bestRate) {
            bestRate = displayMode.refresh_rate;
        }
    }
    fastestRefreshRate = bestRate;
}

GraphicsImplementor::~GraphicsImplementor() {
    //destructor
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    //IMG_Quit(); //here for the same reason that SDLNet_Quit() is in SDLSharedInstance::~SDLSharedInstance().
    //TTF_Quit();
    SDLSharedInstance::quit();
}
/*
int32_t GraphicsImplementor::getNextID() {
    int32_t id;
    if(idPriorityQueue.size() == 0) {
        id = ++highestID;
    } else {
        id = idPriorityQueue.top();
        idPriorityQueue.pop();
    }
    return id;
}

void GraphicsImplementor::renderClear() {
    SDL_RenderClear(renderer);
}

void GraphicsImplementor::basicRenderCopy(int32_t textureID, int32_t destX, int32_t destY) {

    SDL_Texture* texture = 0;
    if(validateID(textureID)) {

        int32_t width, height;
        texture = textureDataMap.at(textureID).texture.get();
        SDL_QueryTexture(texture, 0, 0, &width, &height);

        SDL_Rect sourceRect, destRect;
        sourceRect.x = 0;
        sourceRect.y = 0;
        sourceRect.w = width;
        sourceRect.h = height;
        destRect.x = destX;
        destRect.y = destY;
        destRect.w = width;
        destRect.h = height;

        SDL_RenderCopy(renderer, texture, &sourceRect, &destRect);

    }

}

void GraphicsImplementor::renderCopy(int32_t textureID, Rectangle const*const sourceRect, Rectangle const*const destinationRect) {

    SDL_Texture* texture = 0;
    if(validateID(textureID)) {

        SDL_Rect srcRect, destRect;
        SDL_Rect* srcRectPtr = 0;
        SDL_Rect* destRectPtr = 0;

        if(sourceRect) {
            srcRect.x = sourceRect->x;
            srcRect.y = sourceRect->y;
            srcRect.h = sourceRect->h;
            srcRect.w = sourceRect->w;
            srcRectPtr = &srcRect;
        }

        if(destinationRect) {
            destRect.x = destinationRect->x;
            destRect.y = destinationRect->y;
            destRect.h = destinationRect->h;
            destRect.w = destinationRect->w;
            destRectPtr = & destRect;
        }

        texture = textureDataMap.at(textureID).texture.get();
        SDL_RenderCopy(renderer, texture, srcRectPtr, destRectPtr);

    }

}

void GraphicsImplementor::renderCopy(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight) {

    SDL_Texture* texture = 0;

    if(validateID(textureID)) {

        SDL_Rect sourceRect, destRect;
        sourceRect.x = sourceX;
        sourceRect.y = sourceY;
        sourceRect.w = sourceWidth;
        sourceRect.h = sourceHeight;
        destRect.x = destX;
        destRect.y = destY;
        destRect.w = (destWidth==0?sourceWidth:destWidth);
        destRect.h = (destHeight==0?sourceHeight:destHeight);

        texture = textureDataMap.at(textureID).texture.get();
        SDL_RenderCopy(renderer, texture, &sourceRect, &destRect);
    }

}

void GraphicsImplementor::renderCopyEx(int32_t textureID, Rectangle const*const sourceRect, Rectangle const*const destinationRect,  const double angle, const Point& center, const Flip flip) {

    SDL_Texture* texture = 0;
    if(validateID(textureID)) {

        SDL_Rect srcRect, destRect;
        SDL_Rect* srcRectPtr = 0;
        SDL_Rect* destRectPtr = 0;

        if(sourceRect) {
            srcRect.x = sourceRect->x;
            srcRect.y = sourceRect->y;
            srcRect.h = sourceRect->h;
            srcRect.w = sourceRect->w;
            srcRectPtr = &srcRect;
        }

        if(destinationRect) {
            destRect.x = destinationRect->x;
            destRect.y = destinationRect->y;
            destRect.h = destinationRect->h;
            destRect.w = destinationRect->w;
            destRectPtr = & destRect;
        }

        SDL_RendererFlip f = SDL_FLIP_NONE;
        if(flip == Flip::HORIZONTAL_FLIP) {
            f = SDL_FLIP_HORIZONTAL;
        } else if(flip == Flip::VERTICAL_FLIP) {
            f = SDL_FLIP_VERTICAL;
        } else if(flip == Flip::VERTICAL_HORIZONTAL_FLIP) {
            f = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
        }

        SDL_Point point;
        point.x = center.x;
        point.y = center.y;

        texture = textureDataMap.at(textureID).texture.get();
        SDL_RenderCopyEx(renderer, texture, srcRectPtr, destRectPtr, angle, &point, f);

    }

}

void GraphicsImplementor::renderCopyEx(int32_t textureID, int destX, int destY, int destWidth, int destHeight, int sourceX, int sourceY, int sourceWidth, int sourceHeight, const double angle, const Point& center, const Flip flip) {

    SDL_Texture* texture = 0;
    if(validateID(textureID)) {

        SDL_RendererFlip f = SDL_FLIP_NONE;
        if(flip == Flip::HORIZONTAL_FLIP) {
            f = SDL_FLIP_HORIZONTAL;
        } else if(flip == Flip::VERTICAL_FLIP) {
            f = SDL_FLIP_VERTICAL;
        } else if(flip == Flip::VERTICAL_HORIZONTAL_FLIP) {
            f = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
        }

        SDL_Rect sourceRect, destRect;
        sourceRect.x = sourceX;
        sourceRect.y = sourceY;
        sourceRect.w = sourceWidth;
        sourceRect.h = sourceHeight;
        destRect.x = destX;
        destRect.y = destY;
        destRect.w = destWidth;
        destRect.h = destHeight;

        SDL_Point point;
        point.x = center.x;
        point.y = center.y;

        texture = textureDataMap.at(textureID).texture.get();
        SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect, angle, &point, f);
    }

}

void GraphicsImplementor::renderPresent() {
    SDL_RenderPresent(renderer);
}

//*only works if it was already loaded with loadIMG / loadBMP / loadBMPandSetTransparent.
int32_t GraphicsImplementor::getTextureID(const char* filename) {

    int32_t id = -1;
    bool notFound = true;

    for(auto iter = textureDataMap.begin(); iter != textureDataMap.end() && notFound; iter++) {
        TextureData& textureData(iter->second);
        std::string& textureFilename(textureData.filename);
        if(textureFilename.compare(filename) == 0) {
            notFound = false;
            id = iter->first;
        }
    }
    return id;
}

int32_t GraphicsImplementor::loadBMP(const char* filename) {

    SDL_Surface* tmpSurface = 0;
    int32_t id = getTextureID(filename);

    if(!validateID(id)) {
        tmpSurface = SDL_LoadBMP(filename);
        SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
        //Add tmpTexture to the textureList, and add the filename to the filenameList.
        if(tmpTexture) {
            SDL_SetTextureBlendMode(tmpTexture, SDL_BLENDMODE_BLEND);
            id = getNextID();
            textureDataMap.insert(std::pair<int32_t, TextureData>(id, TextureData(tmpTexture, filename)));
        } else {
            std::stringstream ss;
            ss << "Graphics Interface: loadBMP: SDL_CreateTextureFromSurface failure: Check parameters.\n";
            ss << "\t-> Does the file \"" << filename << "\" exist. Is it corrupt?\n";
            throw std::runtime_error(ss.str());
        }
    } else {
        TextureData& textureData(textureDataMap.at(id));
        textureData.referenceCount++;
    }
    return id;
}

int32_t GraphicsImplementor::loadIMG(const char* filename) {

    int32_t id = getTextureID(filename);
    if(!validateID(id)) {
        SDL_Texture* tmpTexture = IMG_LoadTexture(renderer, filename);

        if (tmpTexture) {
            SDL_SetTextureBlendMode(tmpTexture, SDL_BLENDMODE_BLEND);
            id = getNextID();
            textureDataMap.insert(std::pair<int32_t, TextureData>(id, TextureData(tmpTexture, filename)));
        } else {
            std::stringstream ss;
            ss << "Graphics Interface: loadIMG: IMG_LoadTexture failure: Check parameters.\n";
            ss << "\t-> Does the file \"" << filename << "\" exist. Is it corrupt?\n";
            throw std::runtime_error(ss.str());
        }
    } else {
        TextureData& textureData(textureDataMap.at(id));
        textureData.referenceCount++;
    }
    return id;
}

int32_t GraphicsImplementor::incrementTextureReferenceCount(int32_t id) {

    int32_t retID = -1;
    if(validateID(id)) {
        TextureData& textureData(textureDataMap.at(id));
        textureData.referenceCount++;
        retID = id;
    }
    return retID;
}

//Doesn't actually create a texture, just reserves space for one and returns the textureID.
int32_t GraphicsImplementor::createTexture() {

    int32_t id = getNextID();
    textureDataMap.insert(std::pair<int32_t, TextureData>(id, TextureData(0, "")));
    return id;
}

///If using createTexture() of createTexture(int width, int height), you must redraw the textures on the event onWindowExposed(uint32_t windowID) or else the texture will vanish.
///Do this by using a controller for the object that is holding the sprite so you can destroy the texture and make a new one for the
///new renderer context.
int32_t GraphicsImplementor::createTexture(int width, int height) {

    SDL_Texture* tmpTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    int32_t id = getNextID();
    textureDataMap.insert(std::pair<int32_t, TextureData>(id, TextureData(tmpTexture, "")));
    return id;
}

void GraphicsImplementor::drawText(int32_t textureID, const char* text, const char* filename, int size, uint32_t colour, FontFormat format, uint32_t wrapLength) {

    if(validateID(textureID)) {

        SDL_Texture* tmpTexture = 0;
        SDL_Surface* tmpSurface = 0;
        TTF_Font* font = 0;
        SDL_Color tmpColour;

        font = TTF_OpenFont(filename, size);
        if(!font) {
            std::stringstream ss;
            ss << "Graphics Interface: drawText: TTF_OpenFont failure: Check parameters.\n";
            ss << "\t-> Does the file \"" << filename << "\" exist. Is it corrupt?\n";
            std::stringstream sss;
            sss << "Error: \n" << TTF_GetError() << "\nThe file is either missing or corrupt.\nRe-installing the program should fix this problem.";
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Generic Jumper | Sauteur G\u00e9n\u00e9rique",
                             sss.str().c_str(),
                             NULL);
            throw std::runtime_error(ss.str());
        }
        tmpColour.a = 0x000000FF & colour;
        colour >>= 8;
        tmpColour.b = 0x000000FF & colour;
        colour >>= 8;
        tmpColour.g = 0x000000FF & colour;
        colour >>= 8;
        tmpColour.r = colour;

        if(format == ASCII) {
            tmpSurface = TTF_RenderText_Blended_Wrapped(font, text, tmpColour, wrapLength);
        } else if(format == UTF8) {
            tmpSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, tmpColour, wrapLength);
        } else if(format == UNICODE) {
            tmpSurface = TTF_RenderUNICODE_Blended_Wrapped(font, (Uint16*)text, tmpColour, wrapLength);
        }

        tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
        TTF_CloseFont(font);

        SDL_SetTextureAlphaMod(tmpTexture, tmpColour.a); //set alpha.

        SDL_SetTextureBlendMode(tmpTexture, SDL_BLENDMODE_BLEND); //Make sure they blend properly.

        TextureData& textureData(textureDataMap.at(textureID));
        textureData.texture = std::shared_ptr<SDL_Texture>(tmpTexture, TextureDataDeleter());
        textureData.filename = std::string(text);

    }

}

//set's width an height to the textures dimensions. 0 if invalid texture.
void GraphicsImplementor::getTextureDimensions(int32_t textureID, int& width, int& height) {

    if(validateID(textureID)) {
        SDL_QueryTexture(textureDataMap.at(textureID).texture.get(), 0, 0, &width, &height);
    } else {
        width = 0;
        height = 0;
    }
}

//Get pixel data. pixels must be a pre allocated array of a proper size.
bool GraphicsImplementor::getTexturePixels(uint32_t textureID, Rectangle const sourceRect, uint32_t* pixels, uint32_t transparencyColour) {

    bool validID = validateID(textureID);
    if(validID) {
        SDL_Texture* tmpTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET | SDL_TEXTUREACCESS_STREAMING, sourceRect.w, sourceRect.h);

        SDL_Texture* texture = textureDataMap.at(textureID).texture.get();

        //save render colour.
        SDL_Rect srcRect = {sourceRect.x, sourceRect.y, sourceRect.w, sourceRect.h};
        SDL_Rect dstRect = {0, 0, sourceRect.w, sourceRect.h};

        uint8_t r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

        //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(tmpTexture, SDL_BLENDMODE_NONE); //so the texture is filled all with 0's.

        //SDL_SetTextureAlphaMod(tmpTexture, 1);
        SDL_SetRenderTarget(renderer, tmpTexture);

        SDL_Color transparency;
        transparency.a = transparencyColour & 0xFF;
        transparencyColour >>= 8;
        transparency.b = transparencyColour & 0xFF;
        transparencyColour >>= 8;
        transparency.g = transparencyColour & 0xFF;
        transparencyColour >>= 8;
        transparency.r = transparencyColour & 0xFF;

        SDL_SetRenderDrawColor(renderer, transparency.r, transparency.g, transparency.b, transparency.a);

        //fill the whole texture.
        SDL_RenderClear(renderer); //clear scree to transparency colour. alpha will always be 0xff since we're reading from the renderer.

        //Render to out texture for streamming
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        //SDL_RenderPresent(renderer);

        //int pitch = 0;

        //get pixels
        SDL_RenderReadPixels(renderer, &dstRect, SDL_PIXELFORMAT_RGBA8888, (void*)pixels, 4*sourceRect.w);

        //set render target back to normal.
        SDL_SetRenderTarget(renderer, 0);

        //test
        SDL_Rect rect = {0, 0, 16, 16};
        SDL_RenderCopy(renderer, tmpTexture, &rect, &rect);
        //set draw colour back to what it was.
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        //destroy our streaming texture.
        SDL_DestroyTexture(tmpTexture);
    }
    return validID;
}

//fill texture with given colour. Only works with textures created with [ int32_t createTexture(int, int); ].
void GraphicsImplementor::textureClear(int32_t textureID, uint32_t colour) {

    if(validateID(textureID)) {
        TextureData& textureData(textureDataMap.at(textureID));
        SDL_Texture* texture(textureData.texture.get());
        if(texture) {

            SDL_Color tmpColour;
            tmpColour.a = 0x000000FF & colour;
            colour >>= 8;
            tmpColour.b = 0x000000FF & colour;
            colour >>= 8;
            tmpColour.g = 0x000000FF & colour;
            colour >>= 8;
            tmpColour.r = colour;

            uint8_t r, g, b, a;
            SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

            SDL_SetRenderTarget(renderer, texture);

            SDL_SetRenderDrawColor(renderer, tmpColour.r, tmpColour.g, tmpColour.b, tmpColour.a);
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

            //fill the whoe texture.
            SDL_RenderClear(renderer);

            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);

            SDL_SetRenderTarget(renderer, 0);
        }

    }
}

void GraphicsImplementor::setRenderTarget(int32_t textureID) {

    SDL_Texture* texture = 0;
    if(validateID(textureID)) {
        TextureData& textureData(textureDataMap.at(textureID));
        texture = textureData.texture.get();
    }
    SDL_SetRenderTarget(renderer, texture);
}

//returns true if it was destroyed, false if not.
//if deleting the highest 'causes more work. if deleting more than 1 id, best to delete lower ones first.
bool GraphicsImplementor::destroyTexture(int32_t textureID) {

    bool found = validateID(textureID);
    if(found) {
        textureDataMap.erase(textureID);
        idPriorityQueue.push(textureID);
        if(textureID == highestID) {
            //find new highest used id.
            int highestID = -1;
            for(auto& iter : textureDataMap) {
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

//returns true if it was unloaded. false if not.
bool GraphicsImplementor::unloadTexture(int32_t textureID) {

    bool found = validateID(textureID);
    if(found) {
        TextureData& textureData(textureDataMap.at(textureID));
        if(textureData.referenceCount-- < 1) {
            destroyTexture(textureID);
        }
    }
    return found;
}

bool GraphicsImplementor::validateID(int32_t textureID) {
    return textureDataMap.find(textureID) != textureDataMap.end();
}

void GraphicsImplementor::renderRectangle(int32_t x, int32_t y, int32_t w, int32_t h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(renderer, &rect);
}

void GraphicsImplementor::renderCircle(int32_t x, int32_t y, int32_t r) {
    double xx(-r);
    double yy(0);
    double lx = xx;
    double ly = yy;
    double ti(1.0 / r);
    for(double t = 0; t < 6.28219; t += ti) {
        double xxx(xx * cos(t) - yy * sin(t));
        double yyy(yy * cos(t) + xx * sin(t));

        SDL_RenderDrawLine(renderer, (int32_t)(x + lx), (int32_t)(y + ly), (int32_t)(x + xxx), (int32_t)(y + yyy));
        lx = xxx;
        ly = yyy;
    }
    SDL_RenderDrawLine(renderer, lx, ly, xx, yy);
}

void GraphicsImplementor::renderLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

Point const & GraphicsImplementor::getWindowSize() {
    return windowSize;
}

Point const & GraphicsImplementor::getDisplaySize() {
    return displaySize;
}

bool GraphicsImplementor::isFullscreenOn() {
    return windowFullscreen;
}

void GraphicsImplementor::toggleFullscreen() {
    if(windowFullscreen) {
        SDL_SetWindowFullscreen(window, 0);
    } else {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    windowFullscreen = !windowFullscreen;
}

void GraphicsImplementor::setDrawColour(uint32_t colour) {

    uint8_t r, g, b, a;
    a = 0x000000FF & colour;
    colour >>= 8;
    b = 0x000000FF & colour;
    colour >>= 8;
    g = 0x000000FF & colour;
    colour >>= 8;
    r = 0x000000FF & colour;

    SDL_SetRenderDrawColor(renderer, r, g, b, a);

}

uint32_t GraphicsImplementor::getDrawColour() {

    uint8_t r, g, b, a;
    uint32_t colour;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    colour = r * 0x01000000 + g * 0x00010000 + b * 0x00000100 + a;
    return colour;

}
*/

int32_t GraphicsImplementor::getFastestRefreshRate() {
    return fastestRefreshRate;
}

void GraphicsImplementor::setWindowSize(uint32_t width, uint32_t height) {

    windowWidth = width;
    windowHeight = height;

    SDL_Rect bounds;
    SDL_GetDisplayBounds(0, &bounds);
    if(!width) {
        width = bounds.w; //default width.
    }
    if(!height) {
        height = bounds.h; //default height.
    }
    windowSize = {(int32_t)width, (int32_t)height};

    if(window) {
        SDL_SetWindowSize(window, width, height);
    }
}

void GraphicsImplementor::setWindowTitle(const char* title) {
    this->windowTitle = title;
    if(window) {
        SDL_SetWindowTitle(window, windowTitle);
    }
}

void GraphicsImplementor::setFullscreen(bool windowFullscreen) {
    this->windowFullscreen = windowFullscreen;
    if(window) {
        if(windowFullscreen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else {
            SDL_SetWindowFullscreen(window, 0);
        }
    }
}

void GraphicsImplementor::setWindowIcon(const char* filename) {

    if(window) {
        SDL_Surface* tmp(SDL_LoadBMP(filename));
        SDL_SetWindowIcon(window, tmp);
        SDL_FreeSurface(tmp);
    }

}

