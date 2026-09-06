//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// SDL_render / some SDL_video / some SDL_Rect ElfScript bindings
//-----------------------------------------------------------------------------
// ok before i get mad stupid bind line by line the important after we have a
// window and renderer :
//
//  [X] SDL_SetRenderDrawColor
//  [X] SDL_RenderClear
//  [X] SDL_RenderPresent
//  -----
//  [X] SDL_RenderLine
//  [X] SDL_RenderDebugText
//  [X] SDL_RenderRect
//  [X] SDL_RenderFillRect
//  -----
//  [X] SDL_RenderTexture
// ---------------------------------------------------------------
//
// NOTE: the batch stuff: SDL_RenderRects SDL_RenderLines ...
//       here i should use the PointStorageObject ..
//



#include <SDL3/SDL.h>

#include "SDL3_render.h"
#include "ConsoleTypes.h" //need them for color/rect ...

#include "console/scriptPreprocessor.h"
#include "console/engineAPI.h"
#include "console/consoleExtras.h" // for PoD types - dont forget to add the .cpp to your build
#include "objects/Array.h"

namespace ElfSDL3 {

void RegisterRenderConstants() {
    using namespace Con;

    // SDL_WindowFlags (SDL_video.h)
    REGISTER_CONST_U64(SDL_WINDOW_FULLSCREEN           ); // SDL_UINT64_C(0x0000000000000001)    /**< window is in fullscreen mode */
    REGISTER_CONST_U64(SDL_WINDOW_OPENGL               ); // SDL_UINT64_C(0x0000000000000002)    /**< window usable with OpenGL context */
    REGISTER_CONST_U64(SDL_WINDOW_OCCLUDED             ); // SDL_UINT64_C(0x0000000000000004)    /**< window is occluded */
    REGISTER_CONST_U64(SDL_WINDOW_HIDDEN               ); // SDL_UINT64_C(0x0000000000000008)    /**< window is neither mapped onto the desktop nor shown in the taskbar/dock/window list; SDL_ShowWindow() is required for it to become visible */
    REGISTER_CONST_U64(SDL_WINDOW_BORDERLESS           ); // SDL_UINT64_C(0x0000000000000010)    /**< no window decoration */
    REGISTER_CONST_U64(SDL_WINDOW_RESIZABLE            ); // SDL_UINT64_C(0x0000000000000020)    /**< window can be resized */
    REGISTER_CONST_U64(SDL_WINDOW_MINIMIZED            ); // SDL_UINT64_C(0x0000000000000040)    /**< window is minimized */
    REGISTER_CONST_U64(SDL_WINDOW_MAXIMIZED            ); // SDL_UINT64_C(0x0000000000000080)    /**< window is maximized */
    REGISTER_CONST_U64(SDL_WINDOW_MOUSE_GRABBED        ); // SDL_UINT64_C(0x0000000000000100)    /**< window has grabbed mouse input */
    REGISTER_CONST_U64(SDL_WINDOW_INPUT_FOCUS          ); // SDL_UINT64_C(0x0000000000000200)    /**< window has input focus */
    REGISTER_CONST_U64(SDL_WINDOW_MOUSE_FOCUS          ); // SDL_UINT64_C(0x0000000000000400)    /**< window has mouse focus */
    REGISTER_CONST_U64(SDL_WINDOW_EXTERNAL             ); // SDL_UINT64_C(0x0000000000000800)    /**< window not created by SDL */
    REGISTER_CONST_U64(SDL_WINDOW_MODAL                ); // SDL_UINT64_C(0x0000000000001000)    /**< window is modal */
    REGISTER_CONST_U64(SDL_WINDOW_HIGH_PIXEL_DENSITY   ); // SDL_UINT64_C(0x0000000000002000)    /**< window uses high pixel density back buffer if possible */
    REGISTER_CONST_U64(SDL_WINDOW_MOUSE_CAPTURE        ); // SDL_UINT64_C(0x0000000000004000)    /**< window has mouse captured (unrelated to MOUSE_GRABBED) */
    REGISTER_CONST_U64(SDL_WINDOW_MOUSE_RELATIVE_MODE  ); // SDL_UINT64_C(0x0000000000008000)    /**< window has relative mode enabled */
    REGISTER_CONST_U64(SDL_WINDOW_ALWAYS_ON_TOP        ); // SDL_UINT64_C(0x0000000000010000)    /**< window should always be above others */
    REGISTER_CONST_U64(SDL_WINDOW_UTILITY              ); // SDL_UINT64_C(0x0000000000020000)    /**< window should be treated as a utility window, not showing in the task bar and window list */
    REGISTER_CONST_U64(SDL_WINDOW_TOOLTIP              ); // SDL_UINT64_C(0x0000000000040000)    /**< window should be treated as a tooltip and does not get mouse or keyboard focus, requires a parent window */
    REGISTER_CONST_U64(SDL_WINDOW_POPUP_MENU           ); // SDL_UINT64_C(0x0000000000080000)    /**< window should be treated as a popup menu, requires a parent window */
    REGISTER_CONST_U64(SDL_WINDOW_KEYBOARD_GRABBED     ); // SDL_UINT64_C(0x0000000000100000)    /**< window has grabbed keyboard input */
    REGISTER_CONST_U64(SDL_WINDOW_FILL_DOCUMENT        ); // SDL_UINT64_C(0x0000000000200000)    /**< window is in fill-document mode (Emscripten only), since SDL 3.4.0 */
    REGISTER_CONST_U64(SDL_WINDOW_VULKAN               ); // SDL_UINT64_C(0x0000000010000000)    /**< window usable for Vulkan surface */
    REGISTER_CONST_U64(SDL_WINDOW_METAL                ); // SDL_UINT64_C(0x0000000020000000)    /**< window usable for Metal view */
    REGISTER_CONST_U64(SDL_WINDOW_TRANSPARENT          ); // SDL_UINT64_C(0x0000000040000000)    /**< window with transparent buffer */
    REGISTER_CONST_U64(SDL_WINDOW_NOT_FOCUSABLE        ); // SDL_UINT64_C(0x0000000080000000)    /**< window should not be focusable */

    // ------ SDL_PixelFormat (only the important )------
    REGISTER_CONST_S32(SDL_PIXELFORMAT_XRGB8888);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_ARGB8888);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_XBGR8888);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_ABGR8888);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_RGBX8888);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_RGBA8888);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_BGRX8888);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_BGRA8888);

    REGISTER_CONST_S32(SDL_PIXELFORMAT_RGB24);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_BGR24);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_RGBA32);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_ARGB32);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_BGRA32);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_ABGR32);

    REGISTER_CONST_S32(SDL_PIXELFORMAT_RGBA64_FLOAT);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_ARGB2101010);

    REGISTER_CONST_S32(SDL_PIXELFORMAT_UNKNOWN);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_INDEX8);
    REGISTER_CONST_S32(SDL_PIXELFORMAT_RGB565);


    // ----------- SDL_COLORSPACE_* ----------------
    REGISTER_CONST_S32(SDL_COLORSPACE_UNKNOWN);
    REGISTER_CONST_S32(SDL_COLORSPACE_SRGB);
    REGISTER_CONST_S32(SDL_COLORSPACE_SRGB_LINEAR);

    REGISTER_CONST_S32(SDL_COLORSPACE_HDR10);

    REGISTER_CONST_S32(SDL_COLORSPACE_JPEG);
    REGISTER_CONST_S32(SDL_COLORSPACE_BT601_LIMITED);
    REGISTER_CONST_S32(SDL_COLORSPACE_BT601_FULL);
    REGISTER_CONST_S32(SDL_COLORSPACE_BT709_LIMITED);
    REGISTER_CONST_S32(SDL_COLORSPACE_BT709_FULL);
    REGISTER_CONST_S32(SDL_COLORSPACE_BT2020_LIMITED);
    REGISTER_CONST_S32(SDL_COLORSPACE_BT2020_FULL);

    REGISTER_CONST_S32(SDL_COLORSPACE_RGB_DEFAULT);
    REGISTER_CONST_S32(SDL_COLORSPACE_YUV_DEFAULT);

    // -------- SDL_TextureAccess -----------------
    REGISTER_CONST_S32(SDL_TEXTUREACCESS_STATIC);
    REGISTER_CONST_S32(SDL_TEXTUREACCESS_STREAMING);
    REGISTER_CONST_S32(SDL_TEXTUREACCESS_TARGET);

    // ---------- SDL_PropertiesID ----------------
    REGISTER_CONST_S32(SDL_PROPERTY_TYPE_INVALID);
    REGISTER_CONST_S32(SDL_PROPERTY_TYPE_POINTER);
    REGISTER_CONST_S32(SDL_PROPERTY_TYPE_STRING);
    REGISTER_CONST_S32(SDL_PROPERTY_TYPE_NUMBER);
    REGISTER_CONST_S32(SDL_PROPERTY_TYPE_FLOAT);
    REGISTER_CONST_S32(SDL_PROPERTY_TYPE_BOOLEAN);


    Con::registerEnumS32<SDL_RendererLogicalPresentation>("", false);
    Con::registerEnumS32<SDL_ScaleMode>("", false);
    Con::registerEnumS32<SDL_FlipMode>("", false);

    // Blend >>
    // SDL_BlendMode
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_NONE);
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_BLEND);
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_BLEND_PREMULTIPLIED);
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_ADD);
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_ADD_PREMULTIPLIED);
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_MOD);
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_MUL);
    Con::REGISTER_CONST_U32(SDL_BLENDMODE_INVALID);

    Con::registerEnumS32<SDL_BlendOperation>("", false);
    Con::registerEnumS32<SDL_BlendFactor>("", false);
    // <<<< Blend
}
// -----------------------------------------------------------------------------

void InitRenderer() {
    if (IsRenderInitialized) return;
    RegisterRenderConstants();
    IsRenderInitialized = true;
}
DefineEngineFunction(SDL_RENDERER_INIT, void, (),,"initialize the ElfSDL3::SDL3_Render System") {
    InitRenderer();
}
void ShutDownRenderer() {
    RendererMap.clear();
    WindowMap.clear();
    SurfaceMap.clear();
    TextureMap.clear();
}
DefineEngineFunction(SDL_RENDERER_SHUTDOWN, void, (),,"shutdown and unload the ElfSDL3::SDL3_Render System") {
    ShutDownRenderer();
}
// -----------------------------------------------------------------------------
// Helper
// -----------------------------------------------------------------------------
//return the object or nullprt and fire a Errormessage!
SDL_Renderer* getRendererByID(S32 rendererID){
    SDL_Renderer* renderer  = RendererMap.getValue(rendererID);
    if (!renderer) {
        Con::errorf("Invalid RendererID: %d", rendererID);
        return nullptr;
    }
    return renderer;
}

SDL_Texture* getTextureByID(S32 textureID){
    SDL_Texture* texture = TextureMap.getValue(textureID);
    if (!texture) {
        Con::errorf("Invalid TextureID: %d", textureID);
        return nullptr;
    }
    return texture;
}

// -----------------------------------------------------------------------------
// Bindings
// -----------------------------------------------------------------------------
// extern SDL_DECLSPEC int SDLCALL SDL_GetNumRenderDrivers(void);
DefineEngineFunction(SDL_GetNumRenderDrivers, S32 , (),,"") {
    return SDL_GetNumRenderDrivers();
}
// extern SDL_DECLSPEC const char * SDLCALL SDL_GetRenderDriver(int index);
DefineEngineFunction(SDL_GetRenderDriver, const char *  , (S32 index),,"") {
    return SDL_GetRenderDriver(index);
}

// extern SDL_DECLSPEC bool SDLCALL
// SDL_CreateWindowAndRenderer(const char *title, int width, int height, SDL_WindowFlags window_flags, SDL_Window **window, SDL_Renderer **renderer);
//
// Test:
// print(SDL_CreateWindowAndRenderer("Hello SDL3", 800, 450, SDL_WINDOW_RESIZABLE));
DefineEngineFunction( SDL_CreateWindowAndRenderer, const char *,
    (const char* title, S32 width, S32 height, U64 window_flags),
    ,"This create a SDL_Window and SDL_Renderer.\n@return String: '1 [WindowID] [RenderID]' or '0' if failed") {
    SDL_Renderer* renderer;
    SDL_Window* window;
    if (SDL_CreateWindowAndRenderer(title, width, height, window_flags, &window, &renderer)) {
       S32 windowID =  WindowMap.add(window);
       S32 rendererID =  RendererMap.add(renderer);
       StringBuilder result;
       result.format("1 %d %d", windowID, rendererID);
       return result.end().c_str();
    } else {
        return "0";
    }
}

// NOTE:SDL_video.h!! extern SDL_DECLSPEC SDL_Window * SDLCALL SDL_CreateWindow(const char *title, int w, int h, SDL_WindowFlags flags);
DefineEngineFunction( SDL_CreateWindow, S32,
    (const char* title, S32 width, S32 height, U64 window_flags),
    ,"This create a SDL_Window and SDL_Renderer\n@return S32 WindowID") {
    SDL_Window* window  = SDL_CreateWindow(title, width, height, window_flags);
    if (window) {
       return  WindowMap.add(window);
    }

    Con::errorf("SDL_CreateWindow: Failed to create Window: %s", SDL_GetError());
    return 0;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowTitle(SDL_Window *window, const char *title);
DefineEngineFunction( SDL_SetWindowTitle, bool, (S32 WindowID, const char* title ), ,"Set a Window Title") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return false;

    return SDL_SetWindowTitle(window, title);
}

// extern SDL_DECLSPEC const char * SDLCALL SDL_GetWindowTitle(SDL_Window *window);
DefineEngineFunction( SDL_GetWindowTitle, String, (S32 WindowID), ,"Get a Window Title") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_GetWindowTitle(window);
}

// SDL_video: extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowIcon(SDL_Window *window, SDL_Surface *icon);
DefineEngineFunction( SDL_SetWindowIcon, bool, (S32 WindowID, S32 SurfaceID ), ,"Set a Window Icon from Surface") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return false;
    SDL_Surface* surface = SurfaceMap.getValue(SurfaceID);
    if (!surface) {
        Con::errorf("Failed to get Surface by SurfaceID: %d", SurfaceID);
        return false;
    }
    return SDL_SetWindowIcon(window, surface);
}



// SDL_Video!! FIXME add SDL3_video!!
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowPosition(SDL_Window *window, int x, int y);
DefineEngineFunction( SDL_SetWindowPosition, bool, (S32 WindowID, S32 x, S32 y), ,"Set the Window position") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowPosition(window, x, y);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowPosition(SDL_Window *window, int *x, int *y);
DefineEngineFunction( SDL_GetWindowPosition, ConsoleVector, (S32 WindowID), ,"Get the Window position") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    int x; int y;
    if (!SDL_GetWindowPosition(window, &x, &y)) return result;
    result.points[0] = (F32) x;
    result.points[1] = (F32) y;

    return result;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowSize(SDL_Window *window, int w, int h);
DefineEngineFunction( SDL_SetWindowSize, bool, (S32 WindowID, S32 width, S32 height), ,"Set the Window Size") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowSize(window, width, height);
}


DefineEngineFunction(SDL_StartTextInput, bool, (S32 WindowID),, "start text input mode for input onSDLTextInputEvent(%text)"){
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return false;
    return SDL_StartTextInput(window);
}
DefineEngineFunction(SDL_StopTextInput, bool, (S32 WindowID),, "STOP text input mode for input onSDLTextInputEvent(%text)"){
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return false;
    return SDL_StopTextInput(window);
}


// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowSize(SDL_Window *window, int *w, int *h);
DefineEngineFunction( SDL_GetWindowSize, ConsoleVector, (S32 WindowID), ,"Get the Window Size") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    int w; int h;
    if (!SDL_GetWindowSize(window, &w, &h)) return result;
    result.points[0] = (F32) w;
    result.points[1] = (F32) h;

    return result;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowSafeArea(SDL_Window *window, SDL_Rect *rect);
DefineEngineFunction( SDL_GetWindowSafeArea, ConsoleVector, (S32 WindowID), ,"") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    SDL_Rect r;
    if (!SDL_GetWindowSafeArea(window, &r)) return result;
    result = toConsoleVector(r);

    return result;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowAspectRatio(SDL_Window *window, float min_aspect, float max_aspect);
DefineEngineFunction( SDL_SetWindowAspectRatio, bool, (S32 WindowID, F32 min, F32 max), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowAspectRatio(window, min, max);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowAspectRatio(SDL_Window *window, float *min_aspect, float *max_aspect);
DefineEngineFunction( SDL_GetWindowAspectRatio, ConsoleVector, (S32 WindowID), ,"") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    float min,max;
    if (!SDL_GetWindowAspectRatio(window, &min, &max)) return result;
    result.points[0] = min;
    result.points[1] = max;

    return result;
}


// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowBordersSize(SDL_Window *window, int *top, int *left, int *bottom, int *right);
DefineEngineFunction( SDL_GetWindowBordersSize, ConsoleVector, (S32 WindowID), ,"Get the Window border size: top, left, bottom, right") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    int t,l,b,r;
    if (!SDL_GetWindowBordersSize(window, &t, &l, &b, &r)) return result;
    result.points[0] = (F32) t;
    result.points[1] = (F32) l;
    result.points[2] = (F32) b;
    result.points[3] = (F32) r;

    return result;
}


// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowSizeInPixels(SDL_Window *window, int *w, int *h);
DefineEngineFunction( SDL_GetWindowSizeInPixels, ConsoleVector, (S32 WindowID), ,"Get the Window Size in pixels") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    int w; int h;
    if (!SDL_GetWindowSizeInPixels(window, &w, &h)) return result;
    result.points[0] = (F32) w;
    result.points[1] = (F32) h;

    return result;
}
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowMinimumSize(SDL_Window *window, int min_w, int min_h);
DefineEngineFunction( SDL_SetWindowMinimumSize, bool, (S32 WindowID, S32 width, S32 height), ,"Set the Window minimum Size") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowMinimumSize(window, width, height);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowMinimumSize(SDL_Window *window, int *w, int *h);
DefineEngineFunction( SDL_GetWindowMinimumSize, ConsoleVector, (S32 WindowID), ,"Get the Window minimum Size") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    int w; int h;
    if (!SDL_GetWindowMinimumSize(window, &w, &h)) return result;
    result.points[0] = (F32) w;
    result.points[1] = (F32) h;

    return result;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowMaximumSize(SDL_Window *window, int max_w, int max_h);
DefineEngineFunction( SDL_SetWindowMaximumSize, bool, (S32 WindowID, S32 width, S32 height), ,"Set the Window maximum Size") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowMaximumSize(window, width, height);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowMaximumSize(SDL_Window *window, int *w, int *h);
DefineEngineFunction( SDL_GetWindowMaximumSize, ConsoleVector, (S32 WindowID), ,"Get the Window maximum Size") {
    ConsoleVector result = {};
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return result;
    int w; int h;
    if (!SDL_GetWindowMaximumSize(window, &w, &h)) return result;
    result.points[0] = (F32) w;
    result.points[1] = (F32) h;

    return result;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowBordered(SDL_Window *window, bool bordered);
DefineEngineFunction( SDL_SetWindowBordered, bool, (S32 WindowID, bool bordered), ,"Set bordered") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowBordered(window, bordered);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowResizable(SDL_Window *window, bool resizable);
DefineEngineFunction( SDL_SetWindowResizable, bool, (S32 WindowID, bool resizable), ,"Set resizable") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowResizable(window, resizable);
}


// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowAlwaysOnTop(SDL_Window *window, bool on_top);
DefineEngineFunction( SDL_SetWindowAlwaysOnTop, bool, (S32 WindowID, bool value), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowAlwaysOnTop(window, value);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowFillDocument(SDL_Window *window, bool fill);
DefineEngineFunction( SDL_SetWindowFillDocument, bool, (S32 WindowID, bool value), ,"Emscripten only") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowFillDocument(window, value);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_ShowWindow(SDL_Window *window);
DefineEngineFunction( SDL_ShowWindow, bool, (S32 WindowID), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_ShowWindow(window);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_HideWindow(SDL_Window *window);
DefineEngineFunction( SDL_HideWindow, bool, (S32 WindowID), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_HideWindow(window);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_RaiseWindow(SDL_Window *window);
DefineEngineFunction( SDL_RaiseWindow, bool, (S32 WindowID), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_RaiseWindow(window);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_MaximizeWindow(SDL_Window *window);
DefineEngineFunction( SDL_MaximizeWindow, bool, (S32 WindowID), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_MaximizeWindow(window);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_MinimizeWindow(SDL_Window *window);
DefineEngineFunction( SDL_MinimizeWindow, bool, (S32 WindowID), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_MinimizeWindow(window);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_RestoreWindow(SDL_Window *window);
DefineEngineFunction( SDL_RestoreWindow, bool, (S32 WindowID), ,"") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_RestoreWindow(window);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowFullscreen(SDL_Window *window, bool fullscreen);
DefineEngineFunction( SDL_SetWindowFullscreen, bool, (S32 WindowID, bool fullscreen), ,"Set fullscreen") {
    SDL_Window* window  = WindowMap.getValue(WindowID);
    if (!window) return "";

    return SDL_SetWindowFullscreen(window, fullscreen);
}

// TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_SyncWindow(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_WindowHasSurface(SDL_Window *window);
// extern SDL_DECLSPEC SDL_Surface * SDLCALL SDL_GetWindowSurface(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowSurfaceVSync(SDL_Window *window, int vsync);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowSurfaceVSync(SDL_Window *window, int *vsync);
// extern SDL_DECLSPEC bool SDLCALL SDL_UpdateWindowSurface(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_UpdateWindowSurfaceRects(SDL_Window *window, const SDL_Rect *rects, int numrects);
// extern SDL_DECLSPEC bool SDLCALL SDL_DestroyWindowSurface(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowKeyboardGrab(SDL_Window *window, bool grabbed);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowMouseGrab(SDL_Window *window, bool grabbed);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowKeyboardGrab(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetWindowMouseGrab(SDL_Window *window);
// extern SDL_DECLSPEC SDL_Window * SDLCALL SDL_GetGrabbedWindow(void);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowMouseRect(SDL_Window *window, const SDL_Rect *rect);
// extern SDL_DECLSPEC const SDL_Rect * SDLCALL SDL_GetWindowMouseRect(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowOpacity(SDL_Window *window, float opacity);
// extern SDL_DECLSPEC float SDLCALL SDL_GetWindowOpacity(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowParent(SDL_Window *window, SDL_Window *parent);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowModal(SDL_Window *window, bool modal);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowFocusable(SDL_Window *window, bool focusable);
// extern SDL_DECLSPEC bool SDLCALL SDL_ShowWindowSystemMenu(SDL_Window *window, int x, int y);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowHitTest(SDL_Window *window, SDL_HitTest callback, void *callback_data);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowShape(SDL_Window *window, SDL_Surface *shape);
// extern SDL_DECLSPEC bool SDLCALL SDL_FlashWindow(SDL_Window *window, SDL_FlashOperation operation);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowProgressState(SDL_Window *window, SDL_ProgressState state);
// extern SDL_DECLSPEC SDL_ProgressState SDLCALL SDL_GetWindowProgressState(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetWindowProgressValue(SDL_Window *window, float value);
// extern SDL_DECLSPEC float SDLCALL SDL_GetWindowProgressValue(SDL_Window *window);
// extern SDL_DECLSPEC bool SDLCALL SDL_ScreenSaverEnabled(void);
// extern SDL_DECLSPEC bool SDLCALL SDL_EnableScreenSaver(void);
// extern SDL_DECLSPEC bool SDLCALL SDL_DisableScreenSaver(void);

// NOTE:SDL_video.h!! extern SDL_DECLSPEC SDL_Window * SDLCALL SDL_CreatePopupWindow(SDL_Window *parent, int offset_x, int offset_y, int w, int h, SDL_WindowFlags flags);
DefineEngineFunction( SDL_CreatePopupWindow, S32,
    (S32 parentWindowID, S32 offset_x, S32 offset_y, S32 w, S32 h, U64 window_flags),
    (SDL_WINDOW_POPUP_MENU) ,"This create a SDL_Window and SDL_Renderer\n@return S32 WindowID") {

    SDL_Window* parentWindow = WindowMap.getValue(parentWindowID);
    if (!parentWindow) {
        Con::errorf("SDL_CreatePopupWindow invalid window id: %d", parentWindowID);
        WindowMap.dump();
        return 0;
    }
    SDL_Window* window  = SDL_CreatePopupWindow(parentWindow, offset_x, offset_y, w, h, window_flags);
    if (window) {
       return  WindowMap.add(window);
    }

    Con::errorf("SDL_CreatePopupWindow: Failed to create Window: %s", SDL_GetError());
    return 0;
}



// NOTE:SDL_video.h!! extern SDL_DECLSPEC void SDLCALL SDL_DestroyWindow(SDL_Window *window);
DefineEngineFunction( SDL_DestroyWindow, bool, (S32 windowID),,"destroy a window") {
    return WindowMap.remove(windowID);
}


// extern SDL_DECLSPEC SDL_Renderer * SDLCALL SDL_CreateRenderer(SDL_Window *window, const char *name);
DefineEngineFunction( SDL_CreateRenderer, S32, (S32 windowID, const char* name),("")
    ,"Create a renderer for a Window. If name is empty SDL decide the renderer (default)\n@return S32 RendererID") {
    SDL_Window* window = WindowMap.getValue(windowID);
    if (!window) {
        Con::errorf("SDL_CreateRenderer invalid window id: %d", windowID);
        return 0;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, dStrlen(name) == 0 ? nullptr : name);
    if (renderer) {
        return RendererMap.add(renderer);
    }
    Con::errorf("SDL_CreateRenderer Failed to create Renderer: %s", SDL_GetError());
    return 0;
}

// TODO >>>>>>
// extern SDL_DECLSPEC SDL_Renderer * SDLCALL SDL_CreateRendererWithProperties(SDL_PropertiesID props);
// extern SDL_DECLSPEC SDL_Renderer * SDLCALL SDL_CreateGPURenderer(SDL_GPUDevice *device, SDL_Window *window);
// extern SDL_DECLSPEC SDL_GPUDevice * SDLCALL SDL_GetGPURendererDevice(SDL_Renderer *renderer);
// extern SDL_DECLSPEC SDL_Renderer * SDLCALL SDL_CreateSoftwareRenderer(SDL_Surface *surface);
// extern SDL_DECLSPEC SDL_Renderer * SDLCALL SDL_GetRenderer(SDL_Window *window);
// extern SDL_DECLSPEC SDL_Window * SDLCALL SDL_GetRenderWindow(SDL_Renderer *renderer);
// extern SDL_DECLSPEC const char * SDLCALL SDL_GetRendererName(SDL_Renderer *renderer);
// extern SDL_DECLSPEC SDL_PropertiesID SDLCALL SDL_GetRendererProperties(SDL_Renderer *renderer);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderOutputSize(SDL_Renderer *renderer, int *w, int *h);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetCurrentRenderOutputSize(SDL_Renderer *renderer, int *w, int *h);
// <<<<<<

// extern SDL_DECLSPEC SDL_Texture * SDLCALL SDL_CreateTexture(SDL_Renderer *renderer, SDL_PixelFormat format, SDL_TextureAccess access, int w, int h);
DefineEngineFunction( SDL_CreateTexture, S32,
                      (S32 rendererID, S32 pixelFormat, S32 textureAccces, S32 w, S32 h ),
                      ,"This create a Texture.\n@return S32 TextureID") {
    SDL_Renderer* renderer  = getRendererByID(rendererID);
    if (!renderer) return 0;
    SDL_Texture *texture = SDL_CreateTexture(renderer,(SDL_PixelFormat)pixelFormat, (SDL_TextureAccess) textureAccces, w, h );
    if (!texture) {
        Con::errorf("SDL_CreateTexture: Failed to create Texture: %s", SDL_GetError());
        return 0;
    }
    return TextureMap.add(texture);
}

//NOTE SDL_surface.c :: SDL_Surface *SDL_LoadSurface(const char *file)
DefineEngineFunction(SDL_LoadSurface, S32, (const char* filename), , "load a surface from file. @return SurfaceID") {
    SDL_Surface* surface = SDL_LoadSurface(filename);
    if (!surface) {
        Con::errorf("SDL_LoadSurface failed for file: %s. Error:%s", filename, SDL_GetError());
        return 0;
    }
    return SurfaceMap.add(surface);
}
DefineEngineFunction(SDL_DestroySurface, bool, (S32 SurfaceID), , "unload a surface") {
    return SurfaceMap.remove(SurfaceID);
}

// extern SDL_DECLSPEC SDL_Texture * SDLCALL SDL_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface);
DefineEngineFunction( SDL_CreateTextureFromSurface, S32,
                      (S32 rendererID, S32 SurfaceID ),
                      ,"This create a Texture by SurfaceID.\n@return S32 TextureID") {
    SDL_Renderer* renderer  = getRendererByID(rendererID);
    if (!renderer) return 0;
    SDL_Surface* surface = SurfaceMap.getValue(SurfaceID);
    if (!surface) {
        Con::errorf("Failed to get Surface by SurfaceID: %d", SurfaceID);
        return 0;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surface);
    if (!texture) {
        Con::errorf("SDL_CreateTextureFromSurface: Failed to create Texture: %s", SDL_GetError());
        return 0;
    }
    return TextureMap.add(texture);
}


// TODO:
// extern SDL_DECLSPEC SDL_Texture * SDLCALL SDL_CreateTextureWithProperties(SDL_Renderer *renderer, SDL_PropertiesID props);
// extern SDL_DECLSPEC SDL_PropertiesID SDLCALL SDL_GetTextureProperties(SDL_Texture *texture);
// extern SDL_DECLSPEC SDL_Renderer * SDLCALL SDL_GetRendererFromTexture(SDL_Texture *texture);

// extern SDL_DECLSPEC bool SDLCALL SDL_GetTextureSize(SDL_Texture *texture, float *w, float *h);
DefineEngineFunction(SDL_GetTextureSize, /*Point2F*/ ConsoleVector, (S32 textureID),,"get the size of a texture as float Point" ) {
    ConsoleVector result = {0};
    SDL_Texture* texture = getTextureByID(textureID);
    if (!texture) return result;

    SDL_GetTextureSize(texture, &result.points[0], &result.points[1]);

    return result;
}


// not nativ SDL:
DefineEngineFunction(SDL_GetTextureRect, /*RectF*/ ConsoleVector, (S32 textureID),,"get the size of a texture as float Rectangle (ElfSDL3 extension)" ) {
    ConsoleVector result = {0.f,0.f, 0.f, 0.f};
    SDL_Texture* texture = getTextureByID(textureID);
    if (!texture) return result;
    result.points[2] = texture->w;
    result.points[3] = texture->h;
    return result;
}

// TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_SetTexturePalette(SDL_Texture *texture, SDL_Palette *palette);
// extern SDL_DECLSPEC SDL_Palette * SDLCALL SDL_GetTexturePalette(SDL_Texture *texture);

// extern SDL_DECLSPEC bool SDLCALL SDL_SetTextureColorMod(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b);
DefineEngineFunction(SDL_SetTextureColorMod, bool, (S32 textureID, U8 r, U8 g, U8 b),(255,255,255)
,"set the color for a texture. default white for reset" ) {
    SDL_Texture* texture = getTextureByID(textureID);
    if (!texture) return false;

    return SDL_SetTextureColorMod(texture, r,g,b);
}

//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_SetTextureColorModFloat(SDL_Texture *texture, float r, float g, float b);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetTextureColorMod(SDL_Texture *texture, Uint8 *r, Uint8 *g, Uint8 *b);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetTextureColorModFloat(SDL_Texture *texture, float *r, float *g, float *b);

// extern SDL_DECLSPEC bool SDLCALL SDL_SetTextureAlphaMod(SDL_Texture *texture, Uint8 alpha);
DefineEngineFunction(SDL_SetTextureAlphaMod, bool, (S32 textureID, U8 a),(255)
        ,"set the alpha for a texture, default 255 for reset" ) {
    SDL_Texture* texture = getTextureByID(textureID);
    if (!texture) return false;

    return SDL_SetTextureAlphaMod(texture, a);
}

//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_SetTextureAlphaModFloat(SDL_Texture *texture, float alpha);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetTextureAlphaMod(SDL_Texture *texture, Uint8 *alpha);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetTextureAlphaModFloat(SDL_Texture *texture, float *alpha);

// extern SDL_DECLSPEC bool SDLCALL SDL_SetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode blendMode);
DefineEngineFunction(SDL_SetTextureBlendMode, bool, (S32 textureID, U32 blendMode),(SDL_BLENDMODE_NONE)
    ,"set the BlendMode for a texture, default SDL_BLENDMODE_NONE" ) {
    SDL_Texture* texture = getTextureByID(textureID);
    if (!texture) return false;

    return SDL_SetTextureBlendMode(texture, blendMode);
}

//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_GetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode *blendMode);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetTextureScaleMode(SDL_Texture *texture, SDL_ScaleMode scaleMode);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetTextureScaleMode(SDL_Texture *texture, SDL_ScaleMode *scaleMode);
// extern SDL_DECLSPEC bool SDLCALL SDL_UpdateTexture(SDL_Texture *texture, const SDL_Rect *rect, const void *pixels, int pitch);
// extern SDL_DECLSPEC bool SDLCALL SDL_UpdateYUVTexture(SDL_Texture *texture,
// extern SDL_DECLSPEC bool SDLCALL SDL_UpdateNVTexture(SDL_Texture *texture,
// extern SDL_DECLSPEC bool SDLCALL SDL_LockTexture(SDL_Texture *texture,
// extern SDL_DECLSPEC bool SDLCALL SDL_LockTextureToSurface(SDL_Texture *texture, const SDL_Rect *rect, SDL_Surface **surface);
// extern SDL_DECLSPEC void SDLCALL SDL_UnlockTexture(SDL_Texture *texture);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture);
// extern SDL_DECLSPEC SDL_Texture * SDLCALL SDL_GetRenderTarget(SDL_Renderer *renderer);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderLogicalPresentation(SDL_Renderer *renderer, int w, int h, SDL_RendererLogicalPresentation mode);
DefineEngineFunction(SDL_SetRenderLogicalPresentation, bool, (S32 RendererID,  S32 logicalWidth, S32 logicalHeight, S32 mode),
                     ((S32)SDL_LOGICAL_PRESENTATION_STRETCH) ,
                     "Set SDL_SetRenderLogicalPresentation which does scale the screen.\n"
                     "Warning: ImGui windows (like console) looks a bit strange than!") {
    SDL_Renderer* renderer = getRendererByID(RendererID);
    if (!renderer) return false;
   return SDL_SetRenderLogicalPresentation(renderer, logicalWidth, logicalHeight
            ,(SDL_RendererLogicalPresentation) mode);
}

//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderLogicalPresentation(SDL_Renderer *renderer, int *w, int *h, SDL_RendererLogicalPresentation *mode);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderLogicalPresentationRect(SDL_Renderer *renderer, SDL_FRect *rect);
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderCoordinatesFromWindow(SDL_Renderer *renderer, float window_x, float window_y, float *x, float *y);
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderCoordinatesToWindow(SDL_Renderer *renderer, float x, float y, float *window_x, float *window_y);
// extern SDL_DECLSPEC bool SDLCALL SDL_ConvertEventToRenderCoordinates(SDL_Renderer *renderer, SDL_Event *event);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderViewport(SDL_Renderer *renderer, const SDL_Rect *rect);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderViewport(SDL_Renderer *renderer, SDL_Rect *rect);
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderViewportSet(SDL_Renderer *renderer);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderSafeArea(SDL_Renderer *renderer, SDL_Rect *rect);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderClipRect(SDL_Renderer *renderer, const SDL_Rect *rect);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderClipRect(SDL_Renderer *renderer, SDL_Rect *rect);
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderClipEnabled(SDL_Renderer *renderer);


// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderScale(SDL_Renderer *renderer, float scaleX, float scaleY);
// NOTE Mouse:   SDL_ConvertEventToRenderCoordinates(getRenderer(), &event);
DefineEngineFunction(SDL_SetRenderScale, bool , (S32 rendererID, F32 scaleX, F32 scaleY), (1.f, 1.f)
,"set the render scale - default 1.f") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_SetRenderScale(renderer, scaleX, scaleY);
}

//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderScale(SDL_Renderer *renderer, float *scaleX, float *scaleY);

// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
DefineEngineFunction(SDL_SetRenderDrawColor, bool , (S32 rendererID, U8 r, U8 g, U8 b, U8 a), (255)
                     ,"set the render color") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_SetRenderDrawColor(renderer, r,g,b,a);
}

// ElfScript
DefineEngineFunction(SDL_SetRenderDrawColorVec, bool , (S32 rendererID, ConsoleVector colorVec),
,"set the render color") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;
    if (colorVec.points[3] == 0.f) colorVec.points[3] = 255.f;

    return SDL_SetRenderDrawColor(renderer,
            (U8) colorVec.points[0]
            ,(U8) colorVec.points[1]
            ,(U8) colorVec.points[2]
            ,(U8) colorVec.points[3]
    );
}


// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderDrawColorFloat(SDL_Renderer *renderer, float r, float g, float b, float a);
DefineEngineFunction(SDL_SetRenderDrawColorFloat, bool , (S32 rendererID, F32 r, F32 g, F32 b, F32 a), (1.f)
,"set the render color using float") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_SetRenderDrawColorFloat(renderer, r,g,b,a);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderDrawColor(SDL_Renderer *renderer, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
DefineEngineFunction(SDL_GetRenderDrawColor, ConsoleVector , (S32 rendererID),
,"get the render color") {
    ConsoleVector colorVec = {0};
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return colorVec;
    U8 r,g,b,a;

    if (!SDL_GetRenderDrawColor(renderer,&r, &g, &b, &a)) return colorVec;
    colorVec.points[0] = (U8)r;
    colorVec.points[1] = (U8)g;
    colorVec.points[2] = (U8)b;
    colorVec.points[3] = (U8)a;
    return colorVec;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderDrawColorFloat(SDL_Renderer *renderer, float *r, float *g, float *b, float *a);
DefineEngineFunction(SDL_GetRenderDrawColorFloat, ConsoleVector , (S32 rendererID),
,"get the render color") {
    ConsoleVector colorVec = {0};
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return colorVec;
    float r,g,b,a;

    if (!SDL_GetRenderDrawColorFloat(renderer,&r, &g, &b, &a)) return colorVec;
    colorVec.points[0] = r;
    colorVec.points[1] = g;
    colorVec.points[2] = b;
    colorVec.points[3] = a;
    return colorVec;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderColorScale(SDL_Renderer *renderer, float scale);
DefineEngineFunction(SDL_SetRenderColorScale,  bool , (S32 rendererID,F32 scale),,"") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;
    return SDL_SetRenderColorScale(renderer, scale);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderColorScale(SDL_Renderer *renderer, float *scale);
DefineEngineFunction(SDL_GetRenderColorScale,  F32 , (S32 rendererID),,"") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;
    F32 scale = 0.f;
    if (!SDL_GetRenderColorScale(renderer, &scale)) return 0.f;
    return scale;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode blendMode);
DefineEngineFunction(SDL_SetRenderDrawBlendMode, bool , (S32 rendererID,U32 blendMode), (SDL_BLENDMODE_NONE)
,"set the blendmode, without parameter NONE = disabled is set.") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_SetRenderDrawBlendMode(renderer,blendMode);
}


// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode *blendMode);
DefineEngineFunction(SDL_GetRenderDrawBlendMode, U32 , (S32 rendererID),
,"get the blendmode return U32_MAX if failed") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;
    U32 blendMode = 0;
    if (!SDL_GetRenderDrawBlendMode(renderer, &blendMode)) return U32_MAX;
    return blendMode;
}

// extern SDL_DECLSPEC bool SDLCALL SDL_RenderClear(SDL_Renderer *renderer);
DefineEngineFunction(SDL_RenderClear, bool , (S32 rendererID),
,"clear the current render device - called before the drawing begin - see also SDL_SetRenderDrawColor") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderClear(renderer);
}



// extern SDL_DECLSPEC bool SDLCALL SDL_RenderPoint(SDL_Renderer *renderer, float x, float y);
DefineEngineFunction(SDL_RenderPoint, bool , (S32 rendererID, F32 x, F32 y),
                     ,"render a Point") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderPoint(renderer, x, y);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_RenderPoints(SDL_Renderer *renderer, const SDL_FPoint *points, int count);
#ifdef ENABLE_CONSOLE_VECTOR
DefineEngineFunction(SDL_RenderPoints, bool , (S32 rendererID, Array* pointsArray),
        ,"render points from an Array of TypeVector (non TypeVector will be skipped!)\n"
         "no that fast, since we need to fill the points every call."
) {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer || !pointsArray || pointsArray->mValues.size() == 0) return false;
    Vector<SDL_FPoint> points;
    S32 count = pointsArray->mValues.size();
    S32 realCount = 0;
    points.reserve(count);
    for (U32 i = 0; i < count; i++) {
        ConsoleValue* value = &pointsArray->mValues[i];
        if (value->type != ConsoleValueType::cvVector) continue;
        points[realCount] = {value->v.points[0], value->v.points[1]};
        realCount ++;
    }

    return SDL_RenderPoints(renderer,points.address(), realCount);
}
#endif



// extern SDL_DECLSPEC bool SDLCALL SDL_RenderLine(SDL_Renderer *renderer, float x1, float y1, float x2, float y2);
DefineEngineFunction(SDL_RenderLine, bool , (S32 rendererID, F32 x1, F32 y1, F32 x2, F32 y2),
        ,"render a Line") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderLine(renderer, x1, y1, x2, y2);
}

// ElfScript
DefineEngineFunction(SDL_RenderLineRect, bool , (S32 rendererID, RectF pointsRect),
        ,"render a Line where (w)idth == x2 and (h)eight == y2") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderLine(renderer, pointsRect.x, pointsRect.y, pointsRect.w, pointsRect.h);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderLines(SDL_Renderer *renderer, const SDL_FPoint *points, int count);
#ifdef ENABLE_CONSOLE_VECTOR
DefineEngineFunction(SDL_RenderLines, bool , (S32 rendererID, Array* pointsArray),
                     ,"render points from an Array of TypeVector (non TypeVector will be skipped!)\n"
                     "no that fast, since we need to fill the points every call."
) {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer || !pointsArray || pointsArray->mValues.size() == 0) return false;
    Vector<SDL_FPoint> points;
    S32 count = pointsArray->mValues.size();
    S32 realCount = 0;
    points.reserve(count);
    for (U32 i = 0; i < count; i++) {
        ConsoleValue* value = &pointsArray->mValues[i];
        if (value->type != ConsoleValueType::cvVector) continue;
        points[realCount] = {value->v.points[0], value->v.points[1]};
        realCount ++;
    }

    return SDL_RenderLines(renderer,points.address(), realCount);
}
#endif



// extern SDL_DECLSPEC bool SDLCALL SDL_RenderRect(SDL_Renderer *renderer, const SDL_FRect *rect);
DefineEngineFunction(SDL_RenderRect, bool , (S32 rendererID, RectF rect),
        ,"render a Rect (unfilled)") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderRect(renderer, &rect);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_RenderRects(SDL_Renderer *renderer, const SDL_FRect *rects, int count);
#ifdef ENABLE_CONSOLE_VECTOR
DefineEngineFunction(SDL_RenderRects, bool , (S32 rendererID, Array* pointsArray),
                     ,"render points from an Array of TypeVector (non TypeVector will be skipped!)\n"
                     "no that fast, since we need to fill the points every call."
) {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer || !pointsArray || pointsArray->mValues.size() == 0) return false;
    Vector<SDL_FRect> points;
    S32 count = pointsArray->mValues.size();
    S32 realCount = 0;
    points.reserve(count);
    for (U32 i = 0; i < count; i++) {
        ConsoleValue* value = &pointsArray->mValues[i];
        if (value->type != ConsoleValueType::cvVector) continue;
        points[realCount] = toRectF(value->v);
        realCount ++;
    }

    return SDL_RenderRects(renderer,points.address(), realCount);
}
#endif

// extern SDL_DECLSPEC bool SDLCALL SDL_RenderFillRect(SDL_Renderer *renderer, const SDL_FRect *rect);
DefineEngineFunction(SDL_RenderFillRect, bool , (S32 rendererID, RectF rect),
        ,"render a Rect (filled)") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderFillRect(renderer, &rect);
}

// RenderRect with float params instead of rect and bool filled
DefineEngineFunction(SDL_RenderRectF, bool , (S32 rendererID, F32 x, F32 y, F32 w, F32 h, bool filled),(true)
        ,"render a Rect  with float parameters and filled or not (not nativ SDL3)") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;
    static RectF rect = {};
    rect = {x,y,w,h};
    if (filled ) return SDL_RenderFillRect(renderer, &rect);
     return SDL_RenderRect(renderer, &rect);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderFillRects(SDL_Renderer *renderer, const SDL_FRect *rects, int count);
#ifdef ENABLE_CONSOLE_VECTOR
DefineEngineFunction(SDL_RenderFillRects, bool , (S32 rendererID, Array* pointsArray),
                     ,"render points from an Array of TypeVector (non TypeVector will be skipped!)\n"
                     "no that fast, since we need to fill the points every call."
) {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer || !pointsArray || pointsArray->mValues.size() == 0) return false;
    Vector<SDL_FRect> points;
    S32 count = pointsArray->mValues.size();
    S32 realCount = 0;
    points.reserve(count);
    for (U32 i = 0; i < count; i++) {
        ConsoleValue* value = &pointsArray->mValues[i];
        if (value->type != ConsoleValueType::cvVector) continue;
        points[realCount] = toRectF(value->v);
        realCount ++;
    }

    return SDL_RenderFillRects(renderer,points.address(), realCount);
}
#endif


// extern SDL_DECLSPEC bool SDLCALL SDL_RenderTexture(SDL_Renderer *renderer, SDL_Texture *texture,
//                      const SDL_FRect *srcrect, const SDL_FRect *dstrect);
DefineEngineFunction(SDL_RenderTexture, bool , (S32 rendererID, S32 textureID,  RectF srcRect,  RectF dstRect),
        ,"render texture from src Rect to dstRect") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;
    SDL_Texture* texture = getTextureByID(textureID);
    if (!texture) return false;

    return SDL_RenderTexture(renderer, texture, isValid(srcRect) ? &srcRect : nullptr, isValid(dstRect) ? &dstRect : nullptr);
}

// extern SDL_DECLSPEC bool SDLCALL SDL_RenderTextureRotated(SDL_Renderer *renderer, SDL_Texture *texture,
//                                                           const SDL_FRect *srcrect, const SDL_FRect *dstrect,
//                                                           double angle, const SDL_FPoint *center,
//                                                           SDL_FlipMode flip);
DefineEngineFunction(SDL_RenderTextureRotated, bool ,
        (S32 rendererID, S32 textureID,
         RectF srcRect,  RectF dstRect,
         F64 angle, Point2F centerPoint,
         S32 sdl_flipmode),
        ,"render texture rotated and flipped from srcRect to dstRect") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;
    SDL_Texture* texture = getTextureByID(textureID);
    if (!texture) return false;

    return SDL_RenderTextureRotated(renderer, texture
            , isValid(srcRect) ? &srcRect : nullptr, isValid(dstRect) ? &dstRect : nullptr
            , angle, &centerPoint, (SDL_FlipMode)sdl_flipmode);
}


//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderTextureAffine(SDL_Renderer *renderer, SDL_Texture *texture,
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderTextureTiled(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, float scale, const SDL_FRect *dstrect);
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderTexture9Grid(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, float left_width, float right_width, float top_height, float bottom_height, float scale, const SDL_FRect *dstrect);
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderTexture9GridTiled(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, float left_width, float right_width, float top_height, float bottom_height, float scale, const SDL_FRect *dstrect, float tileScale);
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderGeometry(SDL_Renderer *renderer,
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderGeometryRaw(SDL_Renderer *renderer,
// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderTextureAddressMode(SDL_Renderer *renderer, SDL_TextureAddressMode u_mode, SDL_TextureAddressMode v_mode);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderTextureAddressMode(SDL_Renderer *renderer, SDL_TextureAddressMode *u_mode, SDL_TextureAddressMode *v_mode);
// extern SDL_DECLSPEC SDL_Surface * SDLCALL SDL_RenderReadPixels(SDL_Renderer *renderer, const SDL_Rect *rect);

// extern SDL_DECLSPEC bool SDLCALL SDL_RenderPresent(SDL_Renderer *renderer);
DefineEngineFunction(SDL_RenderPresent, bool , (S32 rendererID),
        ,"called all the rendering(drawing) is done") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderPresent(renderer);
}

// extern SDL_DECLSPEC void SDLCALL SDL_DestroyTexture(SDL_Texture *texture);
DefineEngineFunction(SDL_DestroyTexture, bool , (S32 TextureID),
        ,"remove a renderer") {
    return TextureMap.remove(TextureID);
}


// extern SDL_DECLSPEC void SDLCALL SDL_DestroyRenderer(SDL_Renderer *renderer);
DefineEngineFunction(SDL_DestroyRenderer, bool , (S32 rendererID),
        ,"remove a renderer") {
    return RendererMap.remove(rendererID);
}

//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_FlushRenderer(SDL_Renderer *renderer);
// extern SDL_DECLSPEC void * SDLCALL SDL_GetRenderMetalLayer(SDL_Renderer *renderer);
// extern SDL_DECLSPEC void * SDLCALL SDL_GetRenderMetalCommandEncoder(SDL_Renderer *renderer);
// extern SDL_DECLSPEC bool SDLCALL SDL_AddVulkanRenderSemaphores(SDL_Renderer *renderer, Uint32 wait_stage_mask, Sint64 wait_semaphore, Sint64 signal_semaphore);

// extern SDL_DECLSPEC bool SDLCALL SDL_SetRenderVSync(SDL_Renderer *renderer, int vsync);
DefineEngineFunction(SDL_SetRenderVSync, bool , (S32 rendererID, S32 value),
        ,"set vertical sync on/off") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_SetRenderVSync(renderer, value);
}
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRenderVSync(SDL_Renderer *renderer, int *vsync);
DefineEngineFunction(SDL_GetRenderVSync, S32 , (S32 rendererID),
        ,"get vertical sync is on/off") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return 0;

    S32 result = 0;
    if (!SDL_GetRenderVSync(renderer, &result)) return 0;

    return result;
}
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderDebugText(SDL_Renderer *renderer, float x, float y, const char *str);
DefineEngineFunction(SDL_RenderDebugText, bool , (S32 rendererID, F32 x, F32 y, const char* str),
        ,"called all the rendering(drawing) is done") {
    SDL_Renderer* renderer = getRendererByID(rendererID);
    if (!renderer) return false;

    return SDL_RenderDebugText(renderer, x, y, str);
}

//TODO:
// extern SDL_DECLSPEC bool SDLCALL SDL_RenderDebugTextFormat(SDL_Renderer *renderer, float x, float y, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) SDL_PRINTF_VARARG_FUNC(4);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetDefaultTextureScaleMode(SDL_Renderer *renderer, SDL_ScaleMode scale_mode);
// extern SDL_DECLSPEC bool SDLCALL SDL_GetDefaultTextureScaleMode(SDL_Renderer *renderer, SDL_ScaleMode *scale_mode);
// extern SDL_DECLSPEC SDL_GPURenderState * SDLCALL SDL_CreateGPURenderState(SDL_Renderer *renderer, const SDL_GPURenderStateCreateInfo *createinfo);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetGPURenderStateFragmentUniforms(SDL_GPURenderState *state, Uint32 slot_index, const void *data, Uint32 length);
// extern SDL_DECLSPEC bool SDLCALL SDL_SetGPURenderState(SDL_Renderer *renderer, SDL_GPURenderState *state);
// extern SDL_DECLSPEC void SDLCALL SDL_DestroyGPURenderState(SDL_GPURenderState *state);




// -----------------------------------------------------------------------------
// Rect stuff - copied from BaseElf and renamed to SDL_ names so it's redundant
//              in BaseElf
// -----------------------------------------------------------------------------
// // DefineEngineFunction(SDL_PointInRect, bool , (Point2I p, RectI rect),
// //                      ,"Check a point is in rect --- Integer Rect !! --") {
// //     return SDL_PointInRect(&p, &rect);
// // }
// ElfScript 0.7 also changed to float!
DefineEngineFunction(SDL_PointInRect, bool , (Point2F p, RectF rect),
                     ,"Check a point is in rect") {
    return SDL_PointInRectFloat(&p, &rect);
}
DefineEngineFunction(SDL_PointInRectFloat, bool , (Point2F p, RectF rect),
                     ,"Check a point is in rect") {
    return SDL_PointInRectFloat(&p, &rect);
}



DefineEngineFunction(SDL_HasRectIntersection, bool , (RectI rectA, RectI rectB),
                     ,"Check rect intersection") {
    return SDL_HasRectIntersection(&rectA, &rectB);
}

DefineEngineFunction(SDL_HasRectIntersectionFloat, bool , (RectF rectA, RectF rectB),
                     ,"Check rect intersection") {
    return SDL_HasRectIntersectionFloat(&rectA, &rectB);
}
DefineEngineFunction(SDL_GetRectIntersectionFloat, /*RectF*/ ConsoleVector , (RectF rectA, RectF rectB),
                     ,"get rect intersection (overlap)") {
    RectF result = {0};
    SDL_GetRectIntersectionFloat(&rectA, &rectB, &result);
    return { result.x, result.y, result.w, result.h};
}
DefineEngineFunction(SDL_GetRectUnionFloat, /*RectF*/ ConsoleVector, (RectF rectA, RectF rectB),
                     ,"get rect unio both rects combined to one big.") {
    RectF result = {0};
    SDL_GetRectUnionFloat(&rectA, &rectB, &result);
    return { result.x, result.y, result.w, result.h};
}
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRectEnclosingPointsFloat(const SDL_FPoint *points, int count, const SDL_FRect *clip, SDL_FRect *result);

DefineEngineFunction(SDL_GetRectAndLineIntersectionFloat, bool , (RectF rect, F32 x1,F32 y1, F32 x2, F32 y2),
                     ,"check if a rect and a line intersects") {

    return SDL_GetRectAndLineIntersectionFloat(&rect, &x1, &y1, &x2, &y2);
}


// ---------------- special ----------------------
DefineEngineFunction(SDL_GetStringProperty, const char*, (S32 propertiesID, const char* key),,
        "get string for a property.") {
    return SDL_GetStringProperty((SDL_PropertiesID)propertiesID, key, "");
}
} //namespace
