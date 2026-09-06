//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// Storage Template for Resources and Resource Tools
// usage: ElfStorage<Image, UnloadImage> ImageMap;
// NOTE when the map stores pointer use getValue instead of get!
//-----------------------------------------------------------------------------
#pragma once
#include <platform/types.h>
#include "core/util/tVector.h"
#include <console/console.h>

// using std:: map/vector (fixme ?)
#include <unordered_map>
#include <vector>

namespace ElfResource {

// -------------------------------------------------------------------------
// Template for Resource Handling
// - add() add a item (copy)
// - get() get the pointer of the item
// - getList() get a list of the objects as copy
//          use result.data() to get the pointer.
// - remove() remove a item from the Storage
// - clear() unload all resouces and reset storage
// -------------------------------------------------------------------------
template <typename T, void (*UnloadFunc)(T)>
struct ElfStorage {
    S32 mId = 0;
    std::unordered_map<S32, T> mMap;

    // -------------------------------------------------------------------------
    S32 add(T item) {
        mMap[++mId] = item;
        return mId;
    }
    // -------------------------------------------------------------------------
    // NOTE when the map stores pointer use getValue instead of get!
    T getValue(S32 id, T defaultValue = T()) {
        if (id <= 0) return defaultValue;

        auto it = mMap.find(id);
        if (it != mMap.end()) {
            return it->second;
        }
        return defaultValue;
    }
    // -------------------------------------------------------------------------
    T* get(S32 id) {
        if (id <= 0) return nullptr;
        auto it = mMap.find(id);
        if (it != mMap.end()) {
            return &it->second;
        }
        return nullptr;
    }
    // -------------------------------------------------------------------------
    void dump() {
        StringBuilder dumpStr;
        dumpStr.format("We have %llu Objects. Ids: ",mMap.size());
        for (auto& [key, val] : mMap) {
             dumpStr.append(avar("%d ", key));
        }
        Con::printf("%s", dumpStr.end().c_str());

    }
    // -------------------------------------------------------------------------
    std::vector<T> getList(Vector<S32> ids) {
        std::vector<T> objects;
        for (S32 i =0; i < ids.size(); i++) {
            if (ids[i] <= 0) continue;
            auto obj  = get(ids[i]);
            if (obj) objects.push_back(*obj);
        }
        return objects;
    }
    // -------------------------------------------------------------------------
    bool remove(S32 id) {
        if (id <= 0) return false;
        auto it = mMap.find(id);
        if (it == mMap.end()) return false;

        UnloadFunc(it->second);

        mMap.erase(it);
        return true;
    }
    // -------------------------------------------------------------------------
    bool removeId(S32 id) {
        if (id <= 0) return false;
        auto it = mMap.find(id);
        if (it == mMap.end()) return false;
        mMap.erase(it);
        return true;
    }
    // -------------------------------------------------------------------------
    void clear() {
        for (auto& [key, val] : mMap) {
            UnloadFunc(val);
        }
        mMap.clear();
        mId = 0;
    }

    // -------------------------------------------------------------------------
    S32 getId(const T& item) const {
        if (!item) return 0;
        for (const auto& [key, val] : mMap) {
            if (val == item) {
                return key;
            }
        }
        return 0;
    }

    S32 getIdByAddress(const typename std::conditional_t<std::is_pointer_v<T>, T, const T*> itemPtr) const {
        if (!itemPtr) return 0;

        for (const auto& [key, val] : mMap) {
            if constexpr (std::is_pointer_v<T>) {
                if (val == itemPtr) return key;
            } else {
                if (&val == itemPtr) return key;
            }
        }
        return 0;
    }

};
// -------------------------------------------------------------------------
// ************** USAGE EXAMPLES FROM raylib-elfscript *********************
// -------------------------------------------------------------------------
/*
template<>
inline S32 ElfStorage<Texture, UnloadTexture>::add(Texture item) {

    if (item.id == 0) {
        return 0;
    }

    for (const auto& pair : mMap) {
        if (pair.second.id == item.id) {
            return pair.first;
        }
    }

    mMap[++mId] = item;
    return mId;
}
// -------------------------------------------------------------------------
// Special for shader check already added
// -------------------------------------------------------------------------
template<>
inline S32 ElfStorage<Shader, UnloadShader>::add(Shader item) {
    if (item.id == 0) {
        return 0;
    }

    for (const auto& pair : mMap) {
        if (pair.second.id == item.id) {
            return pair.first;
        }
    }

    mMap[++mId] = item;
    return mId;
}

// -------------------------------------------------------------------------
// Special for default font when id <=0 !
// -------------------------------------------------------------------------
template<>
inline Font* ElfStorage<Font, UnloadFont>::get(S32 id) {
    if (id <= 0) {
        static Font defaultFont = GetFontDefault();
        return &defaultFont;
    }

    auto it = mMap.find(id);
    if (it != mMap.end()) {
        return &it->second;
    }
    return nullptr;
}
// -------------------------------------------------------------------------
// Special for Typ Material
// -------------------------------------------------------------------------
template<>
inline Material* ElfStorage<Material, UnloadMaterial>::get(S32 id) {
    if (id <= 0) {
        static Material defaultMaterial = LoadMaterialDefault();
        return &defaultMaterial;
    }

    auto it = mMap.find(id);
    if (it != mMap.end()) {
        return &it->second;
    }
    return nullptr;
}


// -----------------------------------------------------------------------------------
// Special for animations
struct ElfAnimationBlock {
    ModelAnimation* anims = nullptr;
    S32 count = 0;
};


inline void UnloadElfAnimationBlock(ElfAnimationBlock block) {
    if (block.anims != nullptr && block.count > 0) {
        UnloadModelAnimations(block.anims, block.count);
    }
}
// -----------------------------------------------------------------------------------

inline ElfStorage<Image, UnloadImage> ImageMap;
inline ElfStorage<Texture, UnloadTexture> TextureMap;
inline ElfStorage<Font, UnloadFont> FontMap;
inline ElfStorage<Model, UnloadModel> ModelMap;
inline ElfStorage<Mesh, UnloadMesh> MeshMap;
inline ElfStorage<Material, UnloadMaterial> MaterialsMap;
inline ElfStorage<ElfAnimationBlock, UnloadElfAnimationBlock> ModelAnimationMap; //special
inline ElfStorage<Shader, UnloadShader> ShadersMap;
inline ElfStorage<Wave, UnloadWave> WaveMap;
inline ElfStorage<Sound, UnloadSound> SoundMap;
inline ElfStorage<Music, UnloadMusicStream> MusicMap;
inline ElfStorage<ElfImageColors, UnloadElfColorsData> ColorsMap;
//NOTE if you add a new one dont forget to add it to shutDown!!

*/


// -----------------------------------------------------------------------------------
} //ElfResource
