//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
#include <SDL3/SDL.h>

#include "SDL3_Filesystem.h"
#include "console/scriptPreprocessor.h"
#include "console/engineAPI.h"
#include "objects/Array.h"


namespace ElfSDL3 {


// -----------------------------------------------------------------------------
void RegisterFileSystemConstants() {


    Con::REGISTER_CONST_S32(SDL_PATHTYPE_NONE);      /**< path does not exist */
    Con::REGISTER_CONST_S32(SDL_PATHTYPE_FILE);      /**< a normal file */
    Con::REGISTER_CONST_S32(SDL_PATHTYPE_DIRECTORY); /**< a directory */
    Con::REGISTER_CONST_S32(SDL_PATHTYPE_OTHER);      /**< something completely different like a device node (not a symlink, those are always followed) */
    // ----------------
    Con::REGISTER_CONST_S32(SDL_FOLDER_HOME);        /**< The folder which contains all of the current user's data); preferences); and documents. It usually contains most of the other folders. If a requested folder does not exist); the home folder can be considered a safe fallback to store a user's documents. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_DESKTOP);     /**< The folder of files that are displayed on the desktop. Note that the existence of a desktop folder does not guarantee that the system does show icons on its desktop; certain GNU/Linux distros with a graphical environment may not have desktop icons. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_DOCUMENTS);   /**< User document files); possibly application-specific. This is a good place to save a user's projects. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_DOWNLOADS);   /**< Standard folder for user files downloaded from the internet. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_MUSIC);       /**< Music files that can be played using a standard music player (mp3); ogg...). */
    Con::REGISTER_CONST_S32(SDL_FOLDER_PICTURES);    /**< Image files that can be displayed using a standard viewer (png); jpg...). */
    Con::REGISTER_CONST_S32(SDL_FOLDER_PUBLICSHARE); /**< Files that are meant to be shared with other users on the same computer. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_SAVEDGAMES);  /**< Save files for games. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_SCREENSHOTS); /**< Application screenshots. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_TEMPLATES);   /**< Template files to be used when the user requests the desktop environment to create a new file in a certain folder); such as "New Text File.txt".  Any file in the Templates folder can be used as a starting point for a new file. */
    Con::REGISTER_CONST_S32(SDL_FOLDER_VIDEOS);      /**< Video files that can be played using a standard video player (mp4); webm...). */
    Con::REGISTER_CONST_S32(SDL_FOLDER_COUNT);        /**< Total number of types in this enum); not a folder type by itself. */


}
// -----------------------------------------------------------------------------
// defined here but can be a problem when it's defined somewhere else
#ifndef SDL_FILESYSTEM_NO_GET_ERROR
DefineEngineFunction(SDL_GetError,  const char* , () ,,
                     "Get the last SDL Error."){
    return SDL_GetError();
}
#endif
// -----------------------------------------------------------------------------
DefineEngineFunction(SDL_FileExists, bool, (const char* path),, "return true if the file exits"){
    return SDL_GetPathInfo(path, nullptr);
}

DefineEngineFunction(SDL_GetPathInfo, SimObject*, (const char* path),,
                     "return an object  if the file exits with dynamic fields"){
    if (!path) return nullptr;
    SimObject* obj = new SimObject();
    obj->registerObject();

    SDL_PathInfo info;
    if (!SDL_GetPathInfo(path, &info)) return nullptr;


    obj->setDataField(StringTable->insert( "file_type" ), nullptr,  avar("%d" ,(S32)info.type) );
    obj->setDataField(StringTable->insert( "file_size" ), nullptr,  avar("%llu" ,info.size) );
    obj->setDataField(StringTable->insert( "file_create_time" ), nullptr,  avar("%lld" ,info.create_time) );
    obj->setDataField(StringTable->insert( "file_modify_time" ), nullptr,  avar("%lld" ,info.modify_time) );
    obj->setDataField(StringTable->insert( "file_access_time" ), nullptr,  avar("%lld" ,info.access_time) );

    return obj;
}
// -----------------------------------------------------------------------------
// %resultArray = SDL_GlobDirectory(SDL_GetBasePath(),"*", true);
DefineEngineFunction(SDL_GlobDirectory, Array*,
                     (const char* path,const char* pattern, bool patternIgnoreCase),
                     ("*", true),
                     "scan a directory and return an Array Object with: "
                     "dynamic parameters are: query_path, query_pattern, query_ignorecase\n"
                     "return true if result was not empty.")
{
    if (!path ) return nullptr;
    // extern SDL_DECLSPEC char ** SDLCALL SDL_GlobDirectory(const char *path, const char *pattern, SDL_GlobFlags flags, int *count);
    int count = 0;
    SDL_GlobFlags flags = patternIgnoreCase ?  SDL_GLOB_CASEINSENSITIVE : 0;

    char **matches = SDL_GlobDirectory(path, pattern, flags, &count);

    if (matches == nullptr) return nullptr;

    Array* obj = new Array();
    obj->registerObject();

    obj->setDataField(StringTable->insert( "query_path" ), nullptr,  path );
    obj->setDataField(StringTable->insert( "query_pattern" ), nullptr,  pattern );
    obj->setDataField(StringTable->insert( "query_ignorecase" ), nullptr,  avar("%d", patternIgnoreCase) );


    ConsoleValue value;
    for (int i = 0; i < count; i++) {
        value.setString( matches[i] );
        obj->mValues.push_back(value);
    }
    SDL_free(matches);

    return obj;

}
// DefineEngineFunction(SDL_GlobDirectory, bool,
//                      (const char* path,S32 resultObjectId, const char* pattern, bool patternIgnoreCase),
//                      ("*", true),
//                      "resultObjectId is the object which gets it dynamic fields filled with information\n"
//                      "dynamic parameters are: query_path, query_pattern, query_ignorecase\n"
//                      "for the result: count, result_[i]\n"
//                      "return true if result was not empty."){
//     if (!path ) return false;
//     SimObject* obj = Sim::findObject(resultObjectId);
//     if (!obj) {
//         Con::errorf("SDL_GlobDirectory :: resultObjectId is invalid!");
//         return false;
//     }
//     // extern SDL_DECLSPEC char ** SDLCALL SDL_GlobDirectory(const char *path, const char *pattern, SDL_GlobFlags flags, int *count);
//     int count = 0;
//     SDL_GlobFlags flags = patternIgnoreCase ?  SDL_GLOB_CASEINSENSITIVE : 0;
//
//     char **matches = SDL_GlobDirectory(path, pattern, flags, &count);
//
//     if (matches == nullptr) return false;
//
//     obj->setDataField(StringTable->insert( "query_path" ), nullptr,  path );
//     obj->setDataField(StringTable->insert( "query_pattern" ), nullptr,  pattern );
//     obj->setDataField(StringTable->insert( "query_ignorecase" ), nullptr,  avar("%d", patternIgnoreCase) );
//
//     obj->setDataField(StringTable->insert( "count" ), nullptr,  avar("%d", count) );
//     for (int i = 0; i < count; i++) {
//         obj->setDataField(StringTable->insert( avar("result_%d", i) ), nullptr,  matches[i] );
//     }
//     SDL_free(matches);
//
//     return true;
//
// }
// -----------------------------------------------------------------------------
DefineEngineFunction(SDL_CopyFile, bool, (const char *oldpath, const char *newpath),,"Copy a file.") {
    return SDL_CopyFile(oldpath, newpath);
}
// -----------------------------------------------------------------------------
DefineEngineFunction(SDL_CreateDirectory, bool, (const char *path),,"Create a Directory.") {
    return SDL_CreateDirectory(path);
}
// -----------------------------------------------------------------------------
DefineEngineFunction(SDL_RenamePath, bool, (const char *oldpath, const char *newpath),,"rename a path/file.") {
    return SDL_RenamePath(oldpath, newpath);
}
// -----------------------------------------------------------------------------
DefineEngineFunction(SDL_RemovePath, bool, (const char *path),,"rename a path/file.") {
    return SDL_RemovePath(path);
}

// -----------------------------------------------------------------------------
DefineEngineFunction(SDL_GetUserFolder, const char* , (S32 sdl_folder),,"get the user folder. see also SDL_FOLDER_") {
    return SDL_GetUserFolder((SDL_Folder)sdl_folder);
}
DefineEngineFunction(SDL_GetBasePath, const char* , () ,,
                     "Get the directory where the application was run from.") {
    return SDL_GetBasePath();
}
DefineEngineFunction(SDL_GetCurrentDirectory, const char* , () ,,
                     "Get what the system believes is the current working directory.") {
    return SDL_GetCurrentDirectory();
}
DefineEngineFunction(SDL_GetPrefPath,  String , (const char* org, const char* app) ,,
                     "Get the user-and-app-specific path where files can be written."){
    static String resultPath = "";
    char* rawPath = SDL_GetPrefPath(org, app);
    if (!rawPath) return "";
    resultPath = rawPath;
    SDL_free(rawPath);

    return resultPath;
}
// -----------------------------------------------------------------------------
DefineEngineFunction(SDL_SaveFile, bool, (const char *path, const char* data, U32 size),(0),"save data to a file.") {
    if (size == 0) size = dStrlen(data);
    return SDL_SaveFile(path, data, (size_t)size);
}

DefineEngineFunction(SDL_LoadFile, const char*, (const char *path),,"load data from a file.") {
    size_t size = 0;
    void* data = SDL_LoadFile(path, &size);
    if (size == 0 || !data) return "";
    return (const char*)data;
}

} //namespace
