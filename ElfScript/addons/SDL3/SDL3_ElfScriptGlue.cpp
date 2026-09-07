//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// Glue them together .. you can also use them separatly
// Added Emscripten logic also here !
//-----------------------------------------------------------------------------
// NOTE EMSCRIPTEN: -sASYNCIFY
// 1. SDL compile flag:
//
//  set(SDL_EMSCRIPTEN_PERSISTENT_PATH "/storage" CACHE STRING "" FORCE)
//

//
// 2. Linker settings:
//
/*
    target_link_options(${PROJECT_NAME} PUBLIC --sUSE_SDL=3 PUBLIC --preload-file ${ASSET_PATH})
    target_link_options(${PROJECT_NAME} PRIVATE
    "-sALLOW_MEMORY_GROWTH=1"
    "-sASYNCIFY"
    # filesystem
    "-sEXPORTED_RUNTIME_METHODS=FS"
    "-sFORCE_FILESYSTEM=1"
    "-lidbfs.js"
    )
*/

//
// 3. Save / Load a file - this was hard to finally get it work:
//
// Load:
//
// We need to call  >> EmscriptenMount(); << to mount /storage but then it's not
// ready to use wait for >> onEmscriptenMountReady() << to load your game.
// In pixels with the module loader i did :
/*
    $Globals::mountReady = false;
    function onEmscriptenMountReady() {
        echo("MOUNT READY ....");
        $Globals::mountReady = true;
    }

    function createPixels(%renderer, %window) {

        EmscriptenMount();
        if (getOS() $= "Emscripten") {
            SDL_SetWindowFullscreen(%window, 1);
            if (!$Globals::mountReady ) {
                Main.schedule(100, loadModule); //without index parameter should work
                return 0;
            }
        }
        ......


*/
// SAVE:
// Save is a bit easier simply call >> EmscriptenSync(); << after you saved it.
//-----------------------------------------------------------------------------

#include <SDL3/SDL.h>
#include "console/engineAPI.h"
#include "main/engineGlue.h"
#include "SDL3_ElfScriptGlue.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <iostream>
#endif

namespace ElfSDL3 {
    // ----------------------------------------------------------------------------


    // ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------
    // void InitSDLBindings() {
    void Init() {
        ElfSDL3::RegisterCoreConstants();
        ElfSDL3::InitKeyCodes();
        ElfSDL3::RegisterFileSystemConstants();
        ElfSDL3::InitRenderer();
        // ElfSDL3::Audio::Init(); << must be done in script! via  Audio_Init();
    }

    // DefineEngineFunction(InitSDLBindings, void, (), ,"Init the ElfScript SDL3-Bindings subsystem"){ InitSDLBindings();}

    // void ShutdownSDLBindings(){
    void Shutdown(){
        ElfSDL3::ShutDownRenderer();
        ElfSDL3::Audio::ShutDown();
    }
    // DefineEngineFunction(ShutdownSDLBindings, void, (), ,"ShutDown the ElfScript SDL3-Bindings subsystem"){ InitSDLBindings();}
    // ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------
    DefineEngineFunction(GetFrameTime, F64, (), , "Get the current frame time") {
        return FrameTime;
    }

    // ----------------------------------------------------------------------------
    DefineEngineFunction(SDL_MainLoop, bool, (S32 RendererID),(0),"Main Loop for events and more...\n"
    "optional RendererID parameter is required when you set SDL_SetRenderLogicalPresentation\n"
    "to match the mouse position to the scaled screen."
    ) {

        if (ElfSDL3::shutDownRequested) return false;

        static U32 lastTicks = SDL_GetTicks();

        U32 msEllapsed = SDL_GetTicks() - lastTicks;
        FrameTime = msEllapsed / 1000.f;
        engineGlue::process(msEllapsed );
        lastTicks = SDL_GetTicks();

        ElfSDL3::ClearInputFrameTicks();

        SDL_Renderer* renderer = nullptr;
        if ( RendererID > 0 ) {
            renderer = ElfSDL3::RendererMap.getValue(RendererID);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // when using SDL_SetRenderLogicalPresentation (scaling)
            if (renderer) {
                SDL_ConvertEventToRenderCoordinates(renderer, &event);
            }

            switch (event.type) {
                case SDL_EVENT_QUIT:
                    Con::debugf("******* Quit Event ********");
                    if (Con::isFunction("onSDL_EVENT_QUIT"))
                        Con::executef("onSDL_EVENT_QUIT");
                    return false;
                    break;
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
                    SDL_Window* window =  SDL_GetWindowFromID(event.window.windowID);
                    U32 id = WindowMap.getId(window);
                    Con::debugf("******* Window Close Event (ID:%d)********", id);
                     if (Con::isFunction("onSDL_EVENT_WINDOW_CLOSE_REQUESTED"))
                         Con::executef("onSDL_EVENT_WINDOW_CLOSE_REQUESTED", id);
                    break;
                }


                case SDL_EVENT_WILL_ENTER_BACKGROUND:
                    Con::debugf("******* SDL_EVENT_WILL_ENTER_BACKGROUND ********");
                    if (Con::isFunction("onSDL_ENTER_BACKGROUND"))
                        Con::executef("onSDL_ENTER_BACKGROUND");
                    break;

                case SDL_EVENT_TERMINATING:
                    Con::debugf("******* SDL_EVENT_TERMINATING ********");
                    if (Con::isFunction("onSDL_EVENT_TERMINATING"))
                        Con::executef("onSDL_EVENT_TERMINATING");
                    return false;
                    break;
            };
            ElfSDL3::onEvent(event);

        }

        // Event is called with current event...
        if (OnLoop) OnLoop(event);

        #ifdef __EMSCRIPTEN__
        emscripten_sleep(1);
        #endif


        return true;
    }

    // -------------------------------------------------------------------------
    // Emscripten:
    // -------------------------------------------------------------------------
    #ifdef __EMSCRIPTEN__

        extern "C" {
            EMSCRIPTEN_KEEPALIVE void SetIDBFSReady(bool success) {
                Con::executef("onEmscriptenMountReady", success);
            }
        }
    #endif

    // -------------------------------------------------------------------------
    DefineEngineFunction(EmscriptenMount, void, (),,"Init the save Game for Emscripten") {
        #ifdef __EMSCRIPTEN__
        EM_ASM(
            try {
                try { FS.mkdir('/storage'); } catch(e) {}

                FS.mount(IDBFS, {}, '/storage');
                console.log("IDBFS successful mouted to /storage .");

                FS.syncfs(true, function (err) {
                    if (err) console.error("IDBFS Error:", err);
                    else console.log("Loaded from IDBSF.");
                    _SetIDBFSReady(true);
                });
            } catch (e) {
                console.error("Critical mount error:", e);
                _SetIDBFSReady(false);
            }
        );
        Con::printf("EmscriptenMount.....  onEmscriptenMountReady !!");

        #endif
    }
    // -------------------------------------------------------------------------
    DefineEngineFunction(EmscriptenSync, void, (),,"Sync the save Game for Emscripten") {
        #ifdef __EMSCRIPTEN__
        EM_ASM(
            FS.syncfs(false, function (err) {
                if (err) console.error("Error writing to IndexedDB:", err);
                else console.log("IndexDB should now be populateed");
            });
        );
        Con::printf("EmscriptenSync.....");
        #endif
    }

}


