//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// ElfScript BaseFlux/SDL3 binding
//-----------------------------------------------------------------------------
#include <SDL3/SDL.h>
#include "console/engineAPI.h"
#include "console/consoleExtras.h"
#include "console/simSet.h"
#include "core/volume.h"
#include "math/mMathFn.h"
#include "math/mMathConsoleVector.h"
#include "core/util/tDictionary.h"

#include <string>
#include <format>

#include "BaseFlux/Main.h"
#include "BaseFlux/Draw.h"
#include "BaseFlux/Collision.h"
#include <console/script.h>

#include "ConsoleTypes.h" //NOTE Moved to ElfScript Addons
#include "ColorConstants.h"
#include <SDL3_render.h>


extern BaseFlux::Main app;
extern SDL_Point gMousePos;


/* constant example:
 Con::registerEnumS32<MyEnum>("$MyEnum::", true);
 Con::setScriptConstant("_LEFT_", 1); //real constant using preprocessor*
 Con::setScriptConstant("_RIGHT_", 2); //real constant using preprocessor
*/



// ---------------------------- Helper -------------------------------------
String getFullPath(String pathIdent = "base:/") {
    if (pathIdent.isEmpty()) pathIdent = "base:/";
    if (pathIdent.equal("script:/")) return Torque::FS::GetCwd().getFullPath();
    std::string tmp = pathIdent.c_str();
    app.setFullPath(tmp);
    return tmp.c_str();
}
namespace ElfSDL3 {

    SDL_Point GetScreenSize() {
        SDL_Point screenSize = {0,0};
        SDL_RendererLogicalPresentation mode = SDL_LOGICAL_PRESENTATION_DISABLED;

        if (SDL_GetRenderLogicalPresentation(app.getRenderer(), &screenSize.x, &screenSize.y, &mode)) {
        } else {
            Con::errorf("SDL_GetRenderLogicalPresentation failed: %s\n", SDL_GetError());
        }
        // fallback
        if (mode == 0) SDL_GetWindowSize(app.getWindow(), &screenSize.x, &screenSize.y);

        return screenSize;
    }



} // namespace

// -----------------------------------------------------------------------------
void registerColors() {
    const String prefix = "";

    const std::vector<std::pair<String, Color>> colors = {
        {"LIGHTGRAY", LIGHTGRAY},
        {"GRAY", GRAY},
        {"DARKGRAY", DARKGRAY},
        {"YELLOW", YELLOW},
        {"GOLD", GOLD},
        {"ORANGE", ORANGE},
        {"PINK", PINK},
        {"RED", RED},
        {"MAROON", MAROON},
        {"GREEN", GREEN},
        {"LIME", LIME},
        {"DARKGREEN", DARKGREEN},
        {"SKYBLUE", SKYBLUE},
        {"BLUE", BLUE},
        {"DARKBLUE", DARKBLUE},
        {"PURPLE", PURPLE},
        {"VIOLET", VIOLET},
        {"DARKPURPLE", DARKPURPLE},
        {"BEIGE", BEIGE},
        {"BROWN", BROWN},
        {"DARKBROWN", DARKBROWN},
        {"WHITE", WHITE},
        {"BLACK", BLACK},
        {"BLANK", BLANK},
        {"MAGENTA", MAGENTA},
        {"RAYWHITE", RAYWHITE},

        {"WHITEBLEND", WHITEBLEND},
        {"SLATEGRAY", SLATEGRAY},
        {"SEABLUE", SEABLUE},
        {"NEONPINK", NEONPINK},
        {"ACIDGREEN", ACIDGREEN}
    };


    for (std::size_t i = 0; i < colors.size(); ++i) {
        String fullName = prefix + colors[i].first;
        Color value = static_cast<Color>(colors[i].second);

        std::string colorString = std::format("{{ {},{},{},{} }}", value.r, value.g, value.b, value.a);
        Con::setScriptConstant(fullName.c_str(), colorString );
        // Con::printf("DEBUG key value: %s => %s", fullName.c_str(), colorString.c_str());


        // ConsoleBaseType* type = ConsoleBaseType::getType(TypeColor);
        //
        // if (type) {
        //     const char* colorStrP = type->getData(&value, nullptr, 0);
        //     std::string colorString = std::format("\"{}\"", colorStrP);
        //     Con::setScriptConstant(fullName.c_str(), colorString );
        //     // Con::printf("DEBUG key value: %s => %s", fullName.c_str(), colorString.c_str());
        // }
    }

}


// -----------------------------------------------------------------------------
// =============================================================================
//  SettingsObject (singelton)
// =============================================================================
class SettingsObject: public SimObject
{
    typedef SimObject Parent;
    BaseFlux::Settings mSettings;

    StringTableEntry mCompany;
    StringTableEntry mCaption;
    StringTableEntry mVersion;
    StringTableEntry mIconFileName;
    StringTableEntry mAssetPath;
    StringTableEntry mSoundPathAppend;
    StringTableEntry mTexturePathAppend;
    StringTableEntry mIniFileName;
public:
    DECLARE_CONOBJECT(SettingsObject);
    SettingsObject() {
        mSettings = app.getSettings(); //create a copy
        mCompany = StringTable->insert(mSettings.Company.c_str());
        mCaption = StringTable->insert(mSettings.Caption.c_str());
        mVersion  = StringTable->insert(mSettings.Version.c_str());
        mIconFileName  = StringTable->insert(mSettings.IconFilename.c_str());
        mAssetPath  = StringTable->insert(mSettings.AssetPath.c_str());
        mSoundPathAppend  = StringTable->insert(mSettings.SoundPathAppend.c_str());
        mTexturePathAppend  = StringTable->insert(mSettings.TexturePathAppend.c_str());
        mIniFileName  = StringTable->insert(mSettings.IniFileName.c_str());
    }

    void Sync() {
        mSettings.Company = mCompany;
        mSettings.Caption = mCaption;
        mSettings.Version = mVersion;
        mSettings.IconFilename = mIconFileName;
        mSettings.AssetPath = mAssetPath;
        mSettings.SoundPathAppend = mSoundPathAppend;
        mSettings.TexturePathAppend = mTexturePathAppend;
        mSettings.IniFileName = mIniFileName;
        app.getSettings() = mSettings;
    }


    static void initPersistFields() {
          Parent::initPersistFields();
        addField("ScreenSize", TypePoint2I, Offset(mSettings.ScreenSize,SettingsObject));
        addField("FpsLimit", TypeS32, Offset(mSettings.FpsLimit,SettingsObject));
        addField("WindowMaximized", TypeBool, Offset(mSettings.WindowMaximized,SettingsObject));
        addField("FullScreen", TypeBool, Offset(mSettings.FullScreen,SettingsObject));
        addField("EnableVSync", TypeBool, Offset(mSettings.EnableVSync,SettingsObject));
        //FIXME ?!
        addField("Company", TypeString, Offset(mCompany,SettingsObject));
        addField("Caption", TypeString, Offset(mCaption,SettingsObject));
        addField("Version", TypeString, Offset(mVersion,SettingsObject));

        addField("IconFilename", TypeString, Offset( mIconFileName ,SettingsObject));

        addField("AssetPath", TypeString, Offset(mAssetPath,SettingsObject));
        addField("SoundPathAppend", TypeString, Offset(mSoundPathAppend,SettingsObject));
        addField("TexturePathAppend", TypeString, Offset(mTexturePathAppend,SettingsObject));

        addField("EnableDockSpace", TypeBool, Offset(mSettings.EnableDockSpace,SettingsObject));
        addField("IniFileName", TypeString, Offset(mIniFileName,SettingsObject));

        addField("sdlWindowFlagsOverwrite", TypeS32, Offset(mSettings.sdlWindowFlagsOverwrite,SettingsObject));

        addField("clearColor", TypeColor, Offset(mSettings.clearColor,SettingsObject));
    }
    // std::string getPrefsPath();
    // std::string getSafeCompany();
    // std::string getSafeCaption();
};
IMPLEMENT_CONOBJECT(SettingsObject);
DefineEngineMethod(SettingsObject, sync, void, (),,"Settings must be synced to get active!") {
    object->Sync();
}

SettingsObject* gSettingsObject;



// =============================================================================
// SpriteBaseObject
// a different approch as GameObject. Store:
//  - RecF src rectangle
//  - RecF dest rectangle
//  - F32 angle
//  - Point2F centerPoint
//  - S32 flip (mode)
//  - Color color
// =============================================================================
class SpriteBaseObject :public SimObject
{
    typedef SimObject Parent;
public:
    DECLARE_CONOBJECT(SpriteBaseObject);
    ConsoleVector   mSrcRect = { 0.f, 0.f, 0.f, 0.f};
    ConsoleVector   mDstRect = { 0.f, 0.f, 0.f, 0.f};
    F32     mAngle   = 0.f;
    ConsoleVector mCenterPoint = {0.f, 0.f};
    S32     mFlip = 0; // (SDL_FlipMode)
    Color   mColor = WHITEBLEND;
    bool    mVisible = true;

    // this is used for camera !!!!
    ConsoleVector   mScreenRect = { 0.f, 0.f, 0.f, 0.f};
    ConsoleVector mScreenCenterPoint = { 0.f, 0.f};

    static void initPersistFields() {
        Parent::initPersistFields();
        addField("srcRect", TypeVector, Offset(mSrcRect,SpriteBaseObject), "Rect on Texture.");
        addField("srcX", TypeF32, Offset(mSrcRect.points[0],SpriteBaseObject));
        addField("srcY", TypeF32, Offset(mSrcRect.points[1],SpriteBaseObject));
        addField("srcWidth", TypeF32, Offset(mSrcRect.points[2],SpriteBaseObject));
        addField("srcHeight", TypeF32, Offset(mSrcRect.points[3],SpriteBaseObject));

        addField("dstRect", TypeVector, Offset(mDstRect,SpriteBaseObject), "Rect on Render Target.");
        addField("x", TypeF32, Offset(mDstRect.points[0],SpriteBaseObject));
        addField("y", TypeF32, Offset(mDstRect.points[1],SpriteBaseObject));
        addField("width", TypeF32, Offset(mDstRect.points[2],SpriteBaseObject));
        addField("height", TypeF32, Offset(mDstRect.points[3],SpriteBaseObject));

        addField("angle", TypeF32, Offset(mAngle,SpriteBaseObject), "Rotation Angle on Render Target (degree)");
        addField("centerPoint", TypeVector, Offset(mCenterPoint,SpriteBaseObject), "The Centerpoit of the Rotation - when angle is set.");
        addField("flip", TypeS32, Offset(mFlip,SpriteBaseObject), "Flip mode on Render Target 0..3, see also SDL_FlipMode");

        addField("color", TypeColor, Offset(mColor,SpriteBaseObject), "Color in Byte representation default WHITE = 255 255 255 255");
        addField("r", TypeU8, Offset(mColor.r,SpriteBaseObject));
        addField("g", TypeU8, Offset(mColor.g,SpriteBaseObject));
        addField("b", TypeU8, Offset(mColor.b,SpriteBaseObject));
        addField("a", TypeU8, Offset(mColor.a,SpriteBaseObject));

        addField("visible", TypeBool, Offset(mVisible,SpriteBaseObject), "Flag for Rendering with SpriteGroup or manually checked.");

        addField("ScreenRect", TypeVector, Offset(mScreenRect,SpriteBaseObject), "Translated rect when using camera translate");
        addField("ScreenCenterPoint", TypeVector, Offset(mScreenCenterPoint,SpriteBaseObject), "Translated point when using camera translate");
    }

    // center the centerpoint by dstRect
    void CenterCenterPoint() {
        mCenterPoint.points[0] = mDstRect.points[2] / 2.0;
        mCenterPoint.points[1] = mDstRect.points[3] / 2.0;
    }


}; //CLASS
IMPLEMENT_CONOBJECT(SpriteBaseObject);

DefineEngineMethod(SpriteBaseObject, CenterCenterPoint,void, (),,"center the centerPoint inside the dstRect") {
    object->CenterCenterPoint();
}

DefineEngineMethod(SpriteBaseObject, colorFromHSV, void, (S32 hue, F32 saturation, F32 value, U8 alpha)
    , (255), "Set the color from hue(0..359), sat (0..1), value(0..1). The Rainbow function ;)") {
        object->mColor = fromHSV(hue, saturation, value, alpha);
}



// ---------- color Manupulation ----------
DefineEngineFunction(getColorFromHSV, Color , (S32 hue, F32 saturation, F32 value, U8 alpha)
, (255), "get the color from hue(0..359), sat (0..1), value(0..1). The Rainbow function ;)") {
    return fromHSV(hue, saturation, value, alpha);
}

DefineEngineFunction( getScaledColorSaturation, Color, (Color color, F32 scale), , "scale the color saturation ") {
    setSaturationScale(color, scale);
    return color;
}
DefineEngineFunction( getScaledColorValue, Color, (Color color, F32 scale), , "scale the color value ") {
    setValueScale(color, scale);
    return color;
}
DefineEngineFunction( getScaledColor, Color, (Color color, F32 scale), , "scale the color") {
    setScale(color, scale);
    return color;
}
// =============================================================================
//  Camera2DObject
// This does not work automaticly it's used to translate a destination Rectangle
// =============================================================================
class Camera2DObject: public SimObject
{
    typedef SimObject Parent;

    Point2F mCenter = {0.f, 0.f};

public:
    DECLARE_CONOBJECT(Camera2DObject);

    F32 mX = 0.f;
    F32 mY = 0.f;
    F32 mZoom = 1.f;

    bool onAdd() override {
        Reset(); //<< begin + center
        return Parent::onAdd();
    }

    static void initPersistFields() {
        Parent::initPersistFields();
        addField("x", TypeF32, Offset(mX,Camera2DObject));
        addField("y", TypeF32, Offset(mY,Camera2DObject));
        addField("zoom", TypeF32, Offset(mZoom,Camera2DObject));
    }

    void Reset() {
        Begin();  //make sure we are up to date!
        mX = mCenter.x;
        mY = mCenter.y;
        mZoom = 1.f;
    }

    // must be called before !!!
    void Begin() {
        Point2I sizeI = ElfSDL3::GetScreenSize();
        mCenter.x = (F32)sizeI.x / 2.0f;
        mCenter.y = (F32)sizeI.y / 2.0f;
    }

    // -------------------------------------------------------------------------
    // Screen Coordinates -> World Coordinates
    // (example usage: mouse point)
    Point2F ScreenToWorld(const Point2F& screenPos) {
        Point2F worldPos;
        F32 safeZoom = (mZoom == 0.f) ? 0.0001f : mZoom;
        worldPos.x = ((screenPos.x - mCenter.x) / safeZoom) + mX;
        worldPos.y = ((screenPos.y - mCenter.y) / safeZoom) + mY;
        return worldPos;
    }
    // -------------------------------------------------------------------------
    // Fast Frustum Culling Check (Uses AABB intersection in World Space)
    bool IsInView(const RectF& worldRect) {
        F32 safeZoom = (mZoom == 0.f) ? 0.0001f : mZoom;
        F32 halfViewW = mCenter.x / safeZoom;
        F32 halfViewH = mCenter.y / safeZoom;

        F32 camMinX = mX - halfViewW;
        F32 camMaxX = mX + halfViewW;
        F32 camMinY = mY - halfViewH;
        F32 camMaxY = mY + halfViewH;

        F32 objMinX = worldRect.x;
        F32 objMaxX = worldRect.x + worldRect.w;
        F32 objMinY = worldRect.y;
        F32 objMaxY = worldRect.y + worldRect.h;

        return (objMaxX >= camMinX && objMinX <= camMaxX &&
        objMaxY >= camMinY && objMinY <= camMaxY);
    }
    // -------------------------------------------------------------------------
    // Translate the WorldRect to Render Rect
    void TranslateWorldRect(RectF& rect) {
        rect.x = (rect.x - mX) * mZoom + mCenter.x;
        rect.y = (rect.y - mY) * mZoom + mCenter.y;
        rect.w *= mZoom;
        rect.h *= mZoom;
    }

    // Translate the rotation center point when zoomed.
    void TranslateCenterPoint(Point2F& centerPoint) {
        centerPoint.x *= mZoom;
        centerPoint.y *= mZoom;
    }

    // All in One call
    bool Translate(RectF& rect, Point2F& centerPoint) {
        if (!IsInView(rect)) return false;
        TranslateWorldRect(rect);
        TranslateCenterPoint(centerPoint);
        return true;
    }
    // -------------------------------------------------------------------------
    // Translate a SpriteBaseObject
    bool Translate(SpriteBaseObject* sprite) {
        // skip invisible
        if (!sprite || !sprite->mVisible) return false;
        sprite->mScreenCenterPoint = sprite->mCenterPoint;
        sprite->mScreenRect = sprite->mDstRect;
        RectF r = toRectF(sprite->mScreenRect);
        Point2F p = toPoint2F(sprite->mScreenCenterPoint);
        bool result = Translate(r,p);
        sprite->mScreenRect = toConsoleVector(r);
        sprite->mScreenCenterPoint = toConsoleVector(p);
        return result;
    }

    // get the Translated mouse position !
    Point2F GetMousePosition() {
        return ScreenToWorld(Point2F((F32)gMousePos.x, (F32)gMousePos.y));
    }

};
IMPLEMENT_CONOBJECT(Camera2DObject);

// -----------------------------------------------------------------------------
DefineEngineMethod(Camera2DObject, Begin, void, (),,
        "Refresh the Center position. Must be called on init and when screensize is changed\n"
        "This is called on add. If you do not changes the screen size you do not need to call it at all."
) {
    object->Begin();
}

DefineEngineMethod(Camera2DObject, Reset, void, (),,
        "Refresh the position and Reset the cam to center postion and zoom 1") {
    object->Reset();
}
// -----------------------------------------------------------------------------
DefineEngineMethod(Camera2DObject, GetMousePosition, Point2F, (),,
        "Get the Mouse position in world Coordinates") {
    return object->GetMousePosition();
}
DefineEngineMethod(Camera2DObject, ScreenToWorld, Point2F, (Point2F position),,
                   "get the world position by a screen position") {
    return object->ScreenToWorld(position);
}

// -----------------------------------------------------------------------------
DefineEngineMethod(Camera2DObject, GetInsideSceen, bool, (RectF rect),,
                   "Check is a rect is inside the screen (frustum culling)") {
    return object->IsInView(rect);
}
// -----------------------------------------------------------------------------
// Camera Translate world 2 screen:
// -----------------------------------------------------------------------------
DefineEngineMethod(Camera2DObject, TranslateWorldRect, /*RectF*/ ConsoleVector, (RectF rect),,
        "Translate a world rect to screen / render rect ") {
    object->TranslateWorldRect(rect);
    return toConsoleVector(rect);
}
DefineEngineMethod(Camera2DObject, TranslateCenterPoint, /*Point2F*/ ConsoleVector, (Point2F point),,
        "Translate a world rect to screen / render rect ") {
    object->TranslateCenterPoint(point);
    return toConsoleVector(point);
}

DefineEngineMethod(Camera2DObject, TranslateSpriteBaseObject, bool, (SpriteBaseObject* spriteObj),,
        "Translate to screenRect and screenCenterPoint of a SpriteBaseObject\n"
        "return false if out of view.") {
    if (!spriteObj) {
        Con::errorf("TranslateSpriteBaseObject: SpriteBaseObject invalid");
        return false;
    }
    return object->Translate(spriteObj);
}
// =============================================================================
//  TextureObject
//  As group to AtlasEntryObject's
// =============================================================================
class AtlasEntryObject : public SimObject {
  typedef SimObject Parent;
public:
    DECLARE_CONOBJECT(AtlasEntryObject);
    RectF mRect;

    static void initPersistFields() {
        Parent::initPersistFields();
        addField("rect", TypeRectF, Offset(mRect,AtlasEntryObject));
        addField("x", TypeF32, Offset(mRect.x,AtlasEntryObject));
        addField("y", TypeF32, Offset(mRect.y,AtlasEntryObject));
        addField("width", TypeF32, Offset(mRect.w,AtlasEntryObject));
        addField("height", TypeF32, Offset(mRect.h,AtlasEntryObject));
    }
};
IMPLEMENT_CONOBJECT(AtlasEntryObject);

class TextureObject : public SimGroup
{
    typedef SimGroup Parent;
    StringTableEntry mFileName=nullptr;
    SDL_Texture* mTexture = nullptr;
    bool mIsTargetTexture = false;
    bool mOwnsTexture = false; // we need to take care or removing!

    Color mCurrentColor = BLACK;
    SDL_BlendMode mCurrentBlendMode = SDL_BLENDMODE_INVALID; //trigger inital setup

    void addObject( SimObject* obj ) override {
        // NOTE: make sure we only add AtlasEntryObject's
        AtlasEntryObject* atlasEntry = dynamic_cast<AtlasEntryObject*>(obj);
        if (!atlasEntry) return;
        Parent::addObject(obj);
    }

    void unload() {
        if (mOwnsTexture && mTexture) {
            SDL_DestroyTexture(mTexture);
            mTexture = nullptr;
        }
    }


public:
    DECLARE_CONOBJECT(TextureObject);

    SDL_BlendMode mDefaultBlendMode = SDL_BLENDMODE_BLEND;

    TextureObject() {
         mFileName = StringTable->EmptyString();
    }
    bool onAdd() override {
        mTexture = nullptr;
        if (mFileName && dStrlen(mFileName) > 0) {
            mTexture = app.getTexture(mFileName);
        }
        if (!mTexture) {
            //1 pixel white SDL_Texture as fallback
            if (!mFileName && dStrlen(mFileName) > 0) {
                Con::warnf("TextureObject. Failed to load texture: %s", mFileName);
            }
            mTexture = SDL_CreateTexture(
                app.getRenderer(),
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STATIC,
                1, 1
            );
            if (mTexture) {
                uint32_t whitePixel = 0xFFFFFFFF;
                SDL_UpdateTexture(mTexture, NULL, &whitePixel, sizeof(uint32_t));
            } else {
                return false;
            }
        }
        setBlendModeAndColor(WHITE, mDefaultBlendMode); //initial color and blendmode
        return Parent::onAdd();
    }
    void onRemove() override {
        unload();
        Parent::onRemove();
    }

    bool load(String fileName) {
        if (fileName.isEmpty()) return false;
        mFileName = StringTable->insert(fileName);
        SDL_Texture* tex =  app.getTexture(mFileName);
        if (!tex) return false;
        unload();
        mTexture = tex;
        return true;
    }
    SDL_Texture* get() { return mTexture; };
    bool set(SDL_Texture* texture, bool isTargetTexture) {
        if (!texture) return false;
        unload();
        mTexture = texture;
        mIsTargetTexture = isTargetTexture;
        mOwnsTexture = true;
        return true;
    }
    static void initPersistFields() {
        Parent::initPersistFields();
        addField("fileName", TypeString, Offset(mFileName,TextureObject));
        addField("blendMode", TypeU32, Offset(mDefaultBlendMode,TextureObject));
    }
    // ----------------------------------------------------
    bool SaveImage( const char* fileName, bool asPNG = true) {
        if ( !mTexture || !fileName) return false;

        SDL_PropertiesID props = SDL_GetTextureProperties(mTexture);
        S32 width = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
        S32 height = (int)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);
        SDL_PixelFormat format = (SDL_PixelFormat)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, SDL_PIXELFORMAT_UNKNOWN);

        if (width == 0 || height == 0 || format == SDL_PIXELFORMAT_UNKNOWN) {
            return false;
        }

        SDL_Texture* oldTarget = SDL_GetRenderTarget(app.getRenderer());

        if (!SDL_SetRenderTarget(app.getRenderer(), mTexture)) {
            return false;
        }
        SDL_Surface* surface = SDL_RenderReadPixels(app.getRenderer(), NULL);
        SDL_SetRenderTarget(app.getRenderer(), oldTarget);
        if (!surface) {
            SDL_Log("Failed to read pixels from texture: %s", SDL_GetError());
            return false;
        }

        bool success = false;

        if (asPNG) success = SDL_SavePNG(surface, fileName);
        else success = SDL_SaveBMP(surface, fileName);

        SDL_DestroySurface(surface);

        return success;
    }



    RectF getAtlasRect(S32 rowCount, S32 colCount, S32 index) {
        RectF result = {0.f,0.f,0.f,0.f};
        if (!mTexture || colCount < 1 || rowCount < 1 ) return result;
        F32 texW = (F32)mTexture->w;
        F32 texH = (F32)mTexture->h;

        result.w = texW / colCount;
        result.h = texH / rowCount;

        S32 cellX = index % colCount;
        S32 cellY = index / colCount;

        result.x = cellX * result.w;
        result.y = cellY * result.h;

        return result;
    }


    void setBlendModeAndColor(Color color, SDL_BlendMode mode) {
        if (color != mCurrentColor) {
            mCurrentColor = color;
            SDL_SetTextureColorMod(mTexture, color.r, color.g, color.b);
            SDL_SetTextureAlphaMod(mTexture, color.a);
        }

        // reset to default
        if (mode == SDL_BLENDMODE_INVALID) {
            // check it is changed-
            if (mCurrentBlendMode != mDefaultBlendMode) {
                mCurrentBlendMode = mDefaultBlendMode;
                SDL_SetTextureBlendMode(mTexture, mCurrentBlendMode);
            }
            // nothing else to do here - bail out
            return;
        }

        // check new mode is current mode
        if ( mode == mCurrentBlendMode ) {
            return;
        }
        mCurrentBlendMode = mode;
        SDL_SetTextureBlendMode(mTexture, mCurrentBlendMode);
    }

//     bool SDL_RenderTextureRotated(SDL_Renderer *renderer, SDL_Texture *texture,
//                                   const SDL_FRect *srcrect, const SDL_FRect *dstrect,
//                                   double angle, const SDL_FPoint *center,
//                                   SDL_FlipMode flip);

    bool DrawRotatedSrcDstRect(RectF srcRect, RectF dstRect,
            F32 angle, Point2F centerPoint,
            SDL_FlipMode flip = SDL_FLIP_NONE,
            Color color=WHITE, SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID) {
        SDL_Texture* tex = mTexture;
        if (color.a < 1 || !tex) return false;

        setBlendModeAndColor(color, blendMode);

        return SDL_RenderTextureRotated(app.getRenderer(), tex, &srcRect, &dstRect,
                    angle, &centerPoint, flip);
    }
    // ----------
    bool DrawRotatedCentered(F32 x, F32 y, F32 angle,
            SDL_FlipMode flip = SDL_FLIP_NONE, Color color=WHITE,
            SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID) {
        SDL_Texture* tex = mTexture;
        if (color.a < 1 || !tex) return false;

        F32 w = (F32)tex->w;
        F32 h = (F32)tex->h;
        RectF dstRect = {x - w * 0.5f,y - h * 0.5f, w, h};
        Point2F centerPoint = { w/2, h/2 };

        setBlendModeAndColor(color, blendMode);

        return SDL_RenderTextureRotated(app.getRenderer(), tex, nullptr, &dstRect,
                    angle, &centerPoint, flip);
    }
    // ----------
    bool DrawSrcDstRect(RectF srcRect, RectF dstRect, Color color=WHITE, SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID) {
        SDL_Texture* tex = mTexture;
        if (color.a < 1 || !tex) return false;
        setBlendModeAndColor(color, blendMode);

        return SDL_RenderTexture(app.getRenderer(), tex, &srcRect, &dstRect);
    }
    // ----------
    bool DrawRect( RectF dstRect, Color color=WHITE, SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID) {
        SDL_Texture* tex = mTexture;
        if (color.a < 1 || !tex) return false;
        setBlendModeAndColor(color, blendMode);

        return SDL_RenderTexture(app.getRenderer(), tex, nullptr, &dstRect);
    }

    // ----------
    bool DrawCentered( F32 x, F32 y, Color color=WHITE, SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID) {
        SDL_Texture* tex = mTexture;
        if (color.a < 1 || !tex) return false;

        F32 w = (F32)tex->w;
        F32 h = (F32)tex->h;
        RectF dstRect = {x - w * 0.5f,y - h * 0.5f, w, h};

        setBlendModeAndColor(color, blendMode);

        return SDL_RenderTexture(app.getRenderer(), tex, nullptr, &dstRect);
    }

};
IMPLEMENT_CONOBJECT(TextureObject);

DefineEngineMethod(TextureObject, load, bool, (const char* fileName),,"Load a Texture") {
    return object->load(fileName);
}

DefineEngineMethod(TextureObject, getSize,/*Point2I*/ ConsoleVector, (),
                   ,"get the width and height of the loaded texture" ) {
    SDL_Texture* tex = object->get();
    if (!tex) return {0,0};

    return { (F32)tex->w,(F32)tex->h,0.f,0.f };
    // return {tex->w,tex->h};
}

DefineEngineMethod(TextureObject, getAtlasRect,/*RectF*/ ConsoleVector, (S32 colCount, S32 rowCount, S32 index),
                   ,"get the rectangle on a atlas texture for a given index" ) {
    return toConsoleVector(object->getAtlasRect(colCount, rowCount, index));
}


DefineEngineMethod(TextureObject, DrawRotatedSrcDstRect,bool,
        (RectF srcRect, RectF dstRect,F32 angle, Point2F centerPoint, S32 sdl_flip,  Color color, U32 blendMode),
        (0,WHITE, (U32)SDL_BLENDMODE_INVALID)
        ,"Draw a rotated and optional flipped texture with source and destination rect."
        "@flip: see also SDL_FLIP_ constants" ) {
    return object->DrawRotatedSrcDstRect(srcRect, dstRect,angle, centerPoint, (SDL_FlipMode)sdl_flip, color, blendMode);
}

DefineEngineMethod(TextureObject, DrawRect,bool, (RectF dstRect, Color color, U32 blendMode), (WHITE, (U32)SDL_BLENDMODE_INVALID)
                   ,"Draw a texture with source and destination rect" ) {
     return object->DrawRect( dstRect, color, blendMode);
}

DefineEngineMethod(TextureObject, DrawSrcDstRect,bool,
                   (RectF srcRect, RectF dstRect,  Color color, U32 blendMode),
                   (WHITE,  (U32)SDL_BLENDMODE_INVALID)
                   ,"Draw a the texture from srcRect to dstRect") {
    return object->DrawSrcDstRect(srcRect, dstRect, color, blendMode);

}



DefineEngineMethod(TextureObject, DrawCentered,bool, (F32 x, F32 y, Color color, U32 blendMode), (WHITE,(U32)SDL_BLENDMODE_INVALID)
                   ,"Draw the texture centered at the position" ) {
    return object->DrawCentered(x,y,color, blendMode);
}


DefineEngineMethod(TextureObject, DrawRotatedCentered,bool,
    (F32 x, F32 y, F32 angle , S32 flip, Color color, U32 blendMode)
    , (0,WHITE, (U32)SDL_BLENDMODE_INVALID)
    ,"Draw a centered rotated (optional flipped) texture at the position"
    "@flip: see also SDL_FLIP_ constants" ) {
    return object->DrawRotatedCentered(x,y,angle, (SDL_FlipMode)flip, color, blendMode);
}


DefineEngineMethod(TextureObject, SaveImage,bool, (const char* fileName, bool asPNG),(true)
,"Save the current texture image as png or bmp." ) {
    return object->SaveImage(fileName, asPNG);
}



// =============================================================================
//  SpriteObject
// =============================================================================
class SpriteObject :public SpriteBaseObject
{
    typedef SpriteBaseObject Parent;
public:
    DECLARE_CONOBJECT(SpriteObject);

    ConsoleVector mVelo = {0.f, 0.f , 0.f, 0.f};
    U32     mLayer = 0;
    SDL_BlendMode mBlendMode = SDL_BLENDMODE_INVALID;
    SimObjectId mTextureObjectId = 0;
    StringTableEntry mTextureName = StringTable->EmptyString();


    static void initPersistFields() {
        Parent::initPersistFields();
        addField("velocity", TypePoint2F, Offset(mVelo,SpriteObject), "velocity as point see also veloX veloY");
        addField("veloX", TypeF32, Offset(mVelo.points[0],SpriteObject), "X velocity");
        addField("veloY", TypeF32, Offset(mVelo.points[1],SpriteObject), "Y velocity");
        addField("layer", TypeU32, Offset(mLayer,SpriteObject), "Layer of the Sprite. ");
        addField("blendMode", TypeU32, Offset(mBlendMode,SpriteObject), "SDL_BLENDMODE_ overwrite the Texture blendmode\ndefault SDL_BLENDMODE_INVALID => not overwritten");
        addField("textureObjectId", TypeU32, Offset(mTextureObjectId,SpriteObject), "Id of the Texture Object. Must be set after loading from file. The id is not persistent!");
        addField("textureName", TypeString, Offset(mTextureName,SpriteObject), "optional Texturename");
    }
    // -------------------------------------------------------------------------
    // RectF getRectF() {
    //     return mDstRect;
    // }
    ConsoleVector getCenter2F() {
        return { mDstRect.points[0] + mDstRect.points[2] / 2.0f , mDstRect.points[1] + mDstRect.points[3] / 2.0f, 0.f, 0.f };
    }
    // -------------------------------------------------------------------------
    // Movement
    // -------------------------------------------------------------------------
    void moveLinear(F32 dt = -1.f) {
        if (dt <= 0.f ) dt =(F32)BaseFlux::getFrameTime();
        mDstRect.points[0] += mVelo.points[0] * dt;
        mDstRect.points[1] += mVelo.points[1] * dt;
    }
    // -------------------------------------------------------------------------
    void moveGravity(F32 gravityX, F32 gravityY, F32 dt = -1.f) {
        if (dt <= 0.f ) dt =(F32)BaseFlux::getFrameTime();
        mVelo.points[0] += gravityX * dt;
        mVelo.points[1] += gravityY * dt;
        moveLinear(dt);
    }

    // -------------------------------------------------------------------------
    void moveOrbital(Point2F ankerPoint,F32 gravity, F32 softening, F32 maxSpeed, F32 dt = -1.f) {
        if (dt <= 0.f ) dt =(F32)BaseFlux::getFrameTime();

        Point2F direction = ankerPoint - Point2F(mDstRect.points[0], mDstRect.points[1]);

        F32 distance = length(direction);

        // F32 G = 9.81f;

        if (distance > 0.0001f) {
            normalize(direction);
            F32 gravityPull = (gravity * 1000.f) / (distance * distance + softening);
            mVelo.points[0] += direction.x * gravityPull * dt;
            mVelo.points[1] += direction.y * gravityPull * dt;
        }

        // F32 drag = 0.995f;
        // mVelo.x *= drag;
        // mVelo.y *= drag;

        // F32 currentSpeed = ElfMath::mSqrt(mVelo.x * mVelo.x + mVelo.y * mVelo.y);
        F32 currentSpeed = ElfMath::Vec2Length(mVelo);
        if (currentSpeed > maxSpeed && currentSpeed > 0.0f) {
            mVelo.points[0] = (mVelo.points[0] / currentSpeed) * maxSpeed;
            mVelo.points[1] = (mVelo.points[1] / currentSpeed) * maxSpeed;
        }

        moveLinear(dt);

    }
    // -------------------------------------------------------------------------
    // Collistion solver
    // -------------------------------------------------------------------------
    bool solveCollideLine(F32 x1, F32 y1, F32 x2, F32 y2, F32 bounceStrength /*= 0.2f*/) {

        BaseFlux::Collision::Info info;
        if ( BaseFlux::Collision::getInfoRectLine(toRectF(mDstRect), x1, y1, x2, y2 , info))
        {
            RectF r = toRectF(mDstRect);
            BaseFlux::Collision::solveOberlap(r, info);
            mDstRect = toConsoleVector(r);

            if (bounceStrength > 0.f) {
                // float dotProduct = (mVelo.x * info.mNormal.x) + (mVelo.y * info.mNormal.y);
                float dotProduct = (mVelo.points[0] * info.mNormal.x) + (mVelo.points[1] * info.mNormal.y);
                if (dotProduct < 0.0f) {
                    float impulse = -(1.0f + bounceStrength) * dotProduct;
                    mVelo.points[0] += info.mNormal.x * impulse;
                    mVelo.points[1] += info.mNormal.y * impulse;
                }
            }
            return true;
        }
        return false;
    }
    // -------------------------------------------------------------------------
    bool solveCollideRect(RectF otherRect, bool stopMovement) {
        BaseFlux::Collision::Info info;
        if (BaseFlux::Collision::getInfoRectF(toRectF(mDstRect), otherRect, info)) {

            RectF r = toRectF(mDstRect);
            BaseFlux::Collision::solveOberlap(r, info);
            mDstRect = toConsoleVector(r);

            if (stopMovement) {
                mVelo.points[0] = 0.f;
                mVelo.points[1] = 0.f;
            }
            return true;
        }
        return false;
    }
    // -------------------------------------------------------------------------
    bool Draw(Camera2DObject* camera = nullptr , bool ignoreVisible = false) {
        if (!this->mVisible && !ignoreVisible) return false;
         // we have to fetch the texture
        TextureObject* texObject = dynamic_cast<TextureObject*>(Sim::findObject(this->mTextureObjectId));
        if (!texObject) return false;

        Draw(texObject, camera, ignoreVisible);
        return true;
    }
    // -------------------------------------------------------------------------
    // called from DrawBatch/drawAtlasBatch
    bool Draw(TextureObject* texture,  Camera2DObject* camera = nullptr , bool ignoreVisible = false)
    {

        if (!this->mVisible && !ignoreVisible) return false;

        if (!texture) return false;

        // autosize src if not set or invalid
        if (this->mSrcRect.points[2] <= 0.f || this->mSrcRect.points[3] <= 0.f) {
            this->mSrcRect = {
                0.f,
                0.f,
                (F32)texture->get()->w,
                (F32)texture->get()->h
            };
        }
        if (camera) {
            // Translate also make a frustum check :
            if (camera->Translate(this)) {
               return texture->DrawRotatedSrcDstRect(
                    toRectF(this->mSrcRect),
                    toRectF(this->mScreenRect),
                    this->mAngle,
                    toPoint2F( this->mScreenCenterPoint),
                    (SDL_FlipMode)this->mFlip,
                    this->mColor,
                    this->mBlendMode
                );
            }
        } else {
            return texture->DrawRotatedSrcDstRect(
                toRectF(this->mSrcRect),
                toRectF(this->mDstRect),
                this->mAngle,
                toPoint2F(this->mCenterPoint),
                (SDL_FlipMode)this->mFlip,
                this->mColor,
                this->mBlendMode
            );
        }
        return false;
    }
    // -------------------------------------------------------------------------
};
IMPLEMENT_CONOBJECT(SpriteObject);

DefineEngineMethod(SpriteObject, getCenter2F, ConsoleVector, (), ,"get the center point") {
    return object->getCenter2F();
}
// -------------------------------------------------------------------------
// Draw
// -------------------------------------------------------------------------
DefineEngineMethod(SpriteObject, Draw, bool, (U32 camera2DObjectId),(0),
                   "Draw the Texture") {
    Camera2DObject* camera = nullptr;
    if (camera2DObjectId > 0) {
        camera = dynamic_cast<Camera2DObject*>(Sim::findObject(camera2DObjectId));
    }
    return object->Draw(camera);
}

// OBSOLETE!
// DefineEngineMethod(SpriteObject, DrawTexture, bool, (SimObjectId texObjectID, bool centerDraw, bool useScreenRect),(false, false),
//                    "Simple Draw2D, useScreenRect use the screenRect instead of the destionationrect (Camera2DObject)\n"
//                    "When useScreenRect is enabled centerDraw is not allowed!!!") {
//     TextureObject* texObject = dynamic_cast<TextureObject*>(Sim::findObject(texObjectID));
//     if (!texObject) return false;
//     RectF srcRect  = object->mSrcRect;
//     if (srcRect.w <= 0 || srcRect.h <= 0) {
//         srcRect.x = 0;
//         srcRect.y = 0;
//         srcRect.w = (F32)texObject->get()->w;
//         srcRect.h = (F32)texObject->get()->h;
//     }
//
//     if (useScreenRect && centerDraw) {
//         centerDraw = false;
//         //show a error ?!
//     }
//
//     RectF dstRect; // had pointer but need a copy for centerDraw
//     Point2F* centerPoint;
//     if (useScreenRect) { //See also Camera2DObject
//         dstRect = object->mScreenRect;
//         centerPoint = &object->mScreenCenterPoint;
//     } else {
//         dstRect = object->mDstRect;
//         centerPoint = &object->mCenterPoint;
//     }
//     if (centerDraw) {
//         dstRect.x -= dstRect.w / 2.f;
//         dstRect.y -= dstRect.h / 2.f;
//     }
//
//     return texObject->DrawRotatedSrcDstRect(
//         srcRect, dstRect,
//         object->mAngle, *centerPoint,
//         (SDL_FlipMode)object->mFlip, object->mColor
//         ,object->mBlendMode
//     );
// }

// -------------------------------------------------------------------------
// Movement
// -------------------------------------------------------------------------
DefineEngineMethod(SpriteObject, moveLinear, void, (F32 dt ),(-1.f) ,"Move Linear by velocity") {
    object->moveLinear(dt);
}
DefineEngineMethod(SpriteObject, moveGravity,void, (F32 gravityX, F32 gravityY, F32 dt),
                   (0.f, 9.81f, -1.f) ,"Move with gravity acceleration default: 0,9.81") {
    object->moveGravity(gravityX, gravityY, dt);
}
DefineEngineMethod(SpriteObject, moveOrbital, void,
                   (Point2F ankerPoint, F32 gravity, F32 softening, F32 maxSpeed, F32 dt),
                   (10.f, 150.f, 350.f, -1.f) ,"Orbital Movement around ankerPoint") {
    object->moveOrbital(ankerPoint, gravity, softening, maxSpeed, dt);
}
// -------------------------------------------------------------------------
// Collistion solver
// -------------------------------------------------------------------------
DefineEngineMethod(SpriteObject, solveCollideLine,bool, (RectF linePoints, F32 bounceStrength)
,(0.2f),"check collision agains a line .. using Points as parameter x1,x2,y1,y2 (packed in a RectF)")
{
    return object->solveCollideLine(linePoints.x, linePoints.y, linePoints.w, linePoints.h, bounceStrength);
}
DefineEngineMethod(SpriteObject, solveCollideRect,bool, (RectF otherRect, bool stopMovement)
,(false),"Check collide and move out")
{
    return object->solveCollideRect(otherRect, stopMovement);
}


// =============================================================================
//  SpriteGroup
//  - A Group which only accepts SpriteObjects
//  - NOTE: drawAtlasBatch accept only one Texture - it's desinged for AtlasTextures!
// =============================================================================
class SpriteGroup : public SimGroup
{
    typedef SimGroup Parent;

    Map<U32, TextureObject*> mTextureMap;

public:
    DECLARE_CONOBJECT(SpriteGroup);

    SpriteGroup() {
        mTextureMap.clear();
    }

    void addObject( SimObject* obj ) override {
        // NOTE: make sure we only add SpriteObject's
        SpriteObject* sprite = dynamic_cast<SpriteObject*>(obj);
        if (!sprite) return;
        Parent::addObject(obj);
    }
    void sortLayers();


    void drawBatch(Camera2DObject* camera = nullptr) {
        TextureObject* texture = nullptr;
        mTextureMap.clear(); //this is slower but keep dangling pointers away
        lock();
        for( SimSet::iterator iter = begin(); iter != end(); ++ iter ) {
            SpriteObject* sprite = static_cast<SpriteObject*>(*iter);

            if (!sprite->mVisible) continue;

            if (sprite->mTextureObjectId == 0) continue;
            texture  = mTextureMap[sprite->mTextureObjectId];

            if (!texture) {
                texture = dynamic_cast<TextureObject*>(Sim::findObject(sprite->mTextureObjectId));
                if (!texture) {
                    Con::errorf("Sprite: %d Invalid Texture ID: %d ==> i set the sprite to invisible!"
                        ,sprite->getId(),sprite->mTextureObjectId );
                    sprite->mVisible = false;
                    continue;
                }
                mTextureMap[sprite->mTextureObjectId] = texture;
            }


            sprite->Draw(texture, camera);

        }
        unlock();
    }


    void drawAtlasBatch(TextureObject* texture, Camera2DObject* camera = nullptr) {
        if (!texture) return;
        lock();
        for( SimSet::iterator iter = begin(); iter != end(); ++ iter ) {
            SpriteObject* sprite = static_cast<SpriteObject*>(*iter);

            if (!sprite->mVisible) continue;

             sprite->Draw(texture, camera);

        }
        unlock();
    }

private:
    static S32 QSORT_CALLBACK compare_Layer( const void* a, const void* b );

};
IMPLEMENT_CONOBJECT(SpriteGroup);
// -----------------------------------------------------------------------------
// Layer
S32 QSORT_CALLBACK SpriteGroup::compare_Layer( const void* a, const void* b )
{
    const SpriteObject * cp_a = *(const SpriteObject**)a;
    const SpriteObject * cp_b = *(const SpriteObject**)b;

    if (cp_a->mLayer == cp_b->mLayer) return 0;

    if (cp_a->mLayer > cp_b->mLayer)
        return -1;
    else
        return 1;

}
void SpriteGroup::sortLayers()
{
    lock();
    dQsort( mObjectList.address(), size(), sizeof(SimObject *), SpriteGroup::compare_Layer );
    unlock();
}

DefineEngineMethod(SpriteGroup, sortLayers, void, (),,"Sort SpriteObject's by layer") {
    object->sortLayers();
}
// -----------------------------------------------------------------------------
// DrawBatch and Camera
// -----------------------------------------------------------------------------
DefineEngineMethod(SpriteGroup, drawBatch, bool, (S32 camera2DObjectId), (0),
                     "Draw all sprites in the group with option camera object") {
    Camera2DObject* camObject = nullptr;
    if (camera2DObjectId > 0) {
        camObject = dynamic_cast<Camera2DObject*>(Sim::findObject(camera2DObjectId));
    }
    object->drawBatch(camObject);
    return true;
}

DefineEngineMethod(SpriteGroup, drawAtlasBatch, bool, (S32 texObjectID, S32 camera2DObjectId), (0),
                     "Draw all sprites in the group with option camera object") {
    TextureObject* texObject = dynamic_cast<TextureObject*>(Sim::findObject(texObjectID));
    if (!texObject) return false;

    Camera2DObject* camObject = nullptr;
    if (camera2DObjectId > 0) {
        camObject = dynamic_cast<Camera2DObject*>(Sim::findObject(camera2DObjectId));
    }
    object->drawAtlasBatch(texObject,camObject);
    return true;
}


// =============================================================================
// SoundObject
// Using BaseFlux sound - only WAV supported - nativ by SDL3
// =============================================================================
class SoundObject : public SimObject
{
    typedef SimObject Parent;
    StringTableEntry mFileName=nullptr;
    bool mLoop = false;
    F32 mGain = 1.f;
    BaseFlux::WavData* mWaveData;
public:
    DECLARE_CONOBJECT(SoundObject);
    SoundObject() {
        mFileName = StringTable->EmptyString();
    }
    bool onAdd() override {
        mWaveData = app.getSound(mFileName);
        if (!mWaveData) return false;
        return Parent::onAdd();
    }
    // void onRemove() override;

    BaseFlux::WavData* get() { return mWaveData; };
    static void initPersistFields() {
          Parent::initPersistFields();
        addField("fileName", TypeString, Offset(mFileName,SoundObject));
        addField("gain", TypeF32, Offset(mGain,SoundObject));
        addField("loop", TypeBool, Offset(mLoop,SoundObject));
    }

    bool play() {
        if (!mWaveData) return false;
        return app.getAudioManager().play(mWaveData, mGain, mLoop);
    }
    bool stop() {
        if (!mWaveData) return false;
        return app.getAudioManager().stop(mWaveData);
    }
};
IMPLEMENT_CONOBJECT(SoundObject);

DefineEngineMethod(SoundObject, play,bool, (),,"play the sound" ) { return object->play(); }
DefineEngineMethod(SoundObject, stop,bool, (),,"stop the sound" ) { return object->stop(); }

// =============================================================================
ConsoleFunctionGroupBegin( SDL, "SDL/BaseFlux functions");
// =============================================================================
// Lazy sound playing - slower but fast coded
// bool playSound(std::string fileName, float gain = 1.0f, bool loop = false);
DefineEngineFunction(playSound, bool , (const char* fileName, F32 gain, bool loop),(0.9f, false)
                     ,"lazy way to play a sound by filename ") {
    return app.playSound(fileName, gain, loop);
}
DefineEngineFunction(stopSound, bool , (const char* fileName),
,"lazy way to stop a sound by filename ") {
    return app.stopSound(fileName);
}

DefineEngineFunction(SetClearBackground, void , (Color color),
                     ,"set Settings.clearColor if alpha  == 0 it's disabled") {
    app.getSettings().clearColor = color;
}

DefineEngineFunction(ClearBackground, void , (Color color),
                     ,"clear the background the color - see also SetClearBackground") {
   // app.getSettings().clearColor = color;

    SDL_SetRenderDrawColor(app.getRenderer()
    , color.r
    , color.g
    , color.b
    , color.a
    );
    SDL_RenderClear(app.getRenderer());

}


// -----------------------------------------------------------------------------
// SDL
// -----------------------------------------------------------------------------
DefineEngineFunction(SetColor, void , (Color color),
                     ,"set the render color") {
    SDL_SetRenderDrawColor(app.getRenderer(), color.r, color.g, color.b, color.a);
}
// ----------------------------------------------------------------------------
DefineEngineFunction(BeginBlendMode, void, (U32 blendmode),(SDL_BLENDMODE_BLEND)," see also SDL_BLENDMODE_*") {
    SDL_SetRenderDrawBlendMode(app.getRenderer(), blendmode );

}
DefineEngineFunction(EndBlendMode, void,(),,"") {
    SDL_SetRenderDrawBlendMode(app.getRenderer(), SDL_BLENDMODE_NONE );
}
// ----------------------------------------------------------------------------
DefineEngineFunction(BeginScale, void , (F32 x, F32 y),,"") {
    SDL_SetRenderScale(app.getRenderer(), x, y);
}
DefineEngineFunction(EndScale, void , (),,"") {
    SDL_SetRenderScale(app.getRenderer(), 1.f, 1.f);
}
// -----------------------------------------------------------------------------
DefineEngineFunction(PointInRectI, bool , (Point2I p, RectI rect),
                     ,"Check a point is in rect") {
    return SDL_PointInRect(&p, &rect);
}
DefineEngineFunction(PointInRect, bool , (Point2F p, RectF rect),
                     ,"Check a point is in rect") {
    return SDL_PointInRectFloat(&p, &rect);
}
DefineEngineFunction(HasRectIntersectionI, bool , (RectI rectA, RectI rectB),
                     ,"Check rect intersection") {
    return SDL_HasRectIntersection(&rectA, &rectB);
}

DefineEngineFunction(HasRectIntersection, bool , (RectF rectA, RectF rectB),
                     ,"Check rect intersection") {
    return SDL_HasRectIntersectionFloat(&rectA, &rectB);
}
DefineEngineFunction(GetRectIntersection, ConsoleVector , (RectF rectA, RectF rectB),
                     ,"get rect intersection (overlap)") {
    RectF result = {0};
    SDL_GetRectIntersectionFloat(&rectA, &rectB, &result);
    return { result.x, result.y, result.w,result.h};
}
DefineEngineFunction(GetRectUnion, ConsoleVector , (RectF rectA, RectF rectB),
                     ,"get rect unio both rects combined to one big.") {
    RectF result = {0};
    SDL_GetRectUnionFloat(&rectA, &rectB, &result);
    return { result.x, result.y, result.w,result.h};
}
// extern SDL_DECLSPEC bool SDLCALL SDL_GetRectEnclosingPointsFloat(const SDL_FPoint *points, int count, const SDL_FRect *clip, SDL_FRect *result);

DefineEngineFunction(HasRectLineIntersection, bool , (RectF rect, F32 x1,F32 y1, F32 x2, F32 y2),
                     ,"check if a rect and a line intersects") {

    return SDL_GetRectAndLineIntersectionFloat(&rect, &x1, &y1, &x2, &y2);
}



// ------

DefineEngineFunction(DrawFPS, void, (F32 x, F32 y)
,, "Draw FPS as position")
{
    Color color = GREEN;
    U32 fps =  BaseFlux::getFPS();
    String text = String::ToString("%d fps",fps);

    if (fps < 15) color = RED;
    else if (fps < 30) color = ORANGE;

    BaseFlux::DrawDebugText(app.getRenderer(),x,y,text.c_str(), 1.0f, color, true/*, shadowColor*/);
}

DefineEngineFunction(DrawText, void, ( F32 x, F32 y,String text,
                                      F32 scale, Color color,
                                      bool doShadow, Color shadowColor)
,(1.0, BLACK, false, DARKGRAY),"Draw a Text with optional shadow")
{
    BaseFlux::DrawDebugText(app.getRenderer(),x,y,text.c_str(), scale, color, doShadow, shadowColor);
}

DefineEngineFunction(MeasureText, ConsoleVector /*Point2F*/, ( String text, F32 scale)
,(1.0),"Calculate Text Width and Heigth and return Point2F")
{
    F32 size = (F32)SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * scale;
    return { (F32) text.length() * size, size, 0.f, 0.f};
}

DefineEngineFunction(DrawLine, void, (F32 x1, F32 y1,F32 x2, F32 y2, Color color, F32 thickness)
        ,(WHITE, 1.f),"Draw a Line") {
    if (thickness != 1.f)
        BaseFlux::DrawLineThick(app.getRenderer(), x1,y1,x2,y2, thickness, color);
    else
        BaseFlux::DrawLine(app.getRenderer(), Point2F(x1,y1),Point2F(x2,y2), color);
}

DefineEngineFunction(DrawLineRec, void, (RectF points, Color color, F32 thickness)
    ,(WHITE, 1.f),"Draw a Line using rect w=x2 h=y2 as parameter") {
    if (thickness != 1.f)
        BaseFlux::DrawLineThick(app.getRenderer(), Point2F(points.x,points.y),Point2F(points.w,points.h), thickness, color);
    else
        BaseFlux::DrawLine(app.getRenderer(), Point2F(points.x,points.y),Point2F(points.w,points.h), color);
}

DefineEngineFunction(DrawRect, void, (F32 x, F32 y,F32 w, F32 h, Color color, bool fill)
        ,(WHITE, true),"Draw a Rect") {
    BaseFlux::DrawRect(app.getRenderer(), RectF(x,y,w,h), color, fill);
}
DefineEngineFunction(DrawRectRec, void, (RectF rect, Color color, bool fill)
,(WHITE, true),"Draw a Rect") {
    BaseFlux::DrawRect(app.getRenderer(), rect, color, fill);
}

DefineEngineFunction(DrawCircle, void, (F32 x, F32 y,F32 radius, Color color, bool fill)
    ,(WHITE, true),"Draw a Circle") {
    BaseFlux::DrawCircle(app.getRenderer(), radius, Point2F(x,y), color, fill);
}
DefineEngineFunction(DrawArc, void, (F32 x, F32 y,F32 radius,F32 startRad, F32 endRad, Color color, bool fill)
,(WHITE, true),"Draw a Arc: startRad / endRad in radians ") {
    BaseFlux::DrawArc(app.getRenderer(), radius,startRad, endRad, Point2F(x,y), color, fill);
}
DefineEngineFunction(DrawDonut, void, (F32 x, F32 y,F32 innerRadius,F32 outerRadius, Color color, bool fill)
,(WHITE, true),"Draw a Arc") {
    BaseFlux::DrawDonut(app.getRenderer(), innerRadius,outerRadius, Point2F(x,y), color, fill);
}


// =============================================================================
// RenderTarget and Save functions
// =============================================================================
DefineEngineFunction(CreateRenderTarget, S32  /*textureObjectId*/, (Point2I size),,
                     "Create a Render Target Texture and return the TextureObject") {
    SDL_Texture *target_texture = SDL_CreateTexture(
        app.getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        size.x, size.y
    );
    if (!target_texture) {
        Con::errorf("Failed to create Target Texture!");
        return 0;
    }
    TextureObject* result = new TextureObject();
    result->registerObject(); // before set because this calles add
    if (!result->set(target_texture, true)) {
        result->deleteObject();
        return 0;
    }
    return result->getId();
}
// -----------------------------------------------------------------------------
DefineEngineFunction(SetRenderTarget, bool  , (SimObjectId texObjectID),,
                     "Set a Render Target if textureObjectId is 0 it is reseted") {

    if (texObjectID == 0) {
        return SDL_SetRenderTarget(app.getRenderer(), nullptr);
    }
    TextureObject* texObject = dynamic_cast<TextureObject*>(Sim::findObject(texObjectID));
    if (!texObject) return false;
    return SDL_SetRenderTarget(app.getRenderer(),texObject->get());
}
// -----------------------------------------------------------------------------
DefineEngineFunction(GetRendererName, String, (), ,"Get the current renderer name") {
    return SDL_GetRendererName(app.getRenderer());
}
// -----------------------------------------------------------------------------
// Lights /*
// SDL_Texture* CreatePointLightTexture(SDL_Renderer* renderer, int radius, SDL_Color color);
DefineEngineFunction(CreatePointLightTexture, S32, (S32 radius, Color color, bool diffuse), (WHITE, true),
        "Create Point Light Texture object" ) {
    SDL_Texture* target_texture = BaseFlux::CreatePointLightTexture(app.getRenderer(), radius, color, diffuse);
    if (!target_texture) {
        Con::errorf("Failed to create Point Light Texture!");
        return 0;
    }
    TextureObject* result = new TextureObject();
    result->registerObject(); // before set because this calles add
    if (!result->set(target_texture, true)) {
        result->deleteObject();
        return 0;
    }
    result->mDefaultBlendMode = SDL_BLENDMODE_ADD;
    return result->getId();
}

// SDL_Texture* CreateRayLightTexture(SDL_Renderer* renderer, int length, int width, SDL_Color color, bool diffuse = true);
DefineEngineFunction(CreateRayLightTexture, S32, ( S32 width, S32 height, Color color, bool diffuse), (WHITE, true),
        "Create Ray/Beam Light Texture object" ) {
    SDL_Texture* target_texture = BaseFlux::CreateRayLightTexture(app.getRenderer(), width, height, color, diffuse);
    if (!target_texture) {
        Con::errorf("Failed to create Ray Light Texture!");
        return 0;
    }
    TextureObject* result = new TextureObject();
    result->registerObject(); // before set because this calles add
    if (!result->set(target_texture, true)) {
        result->deleteObject();
        return 0;
    }
    result->mDefaultBlendMode = SDL_BLENDMODE_ADD;
    return result->getId();
}

// SDL_Texture* CreateSpotlightTexture(SDL_Renderer* renderer, int radius, float coneAngleDegrees, SDL_Color color);*/
DefineEngineFunction(CreateSpotlightTexture, S32, (S32 radius, F32 coneAngleDegrees,  Color color, bool diffuse, bool damping)
    , (WHITE, true, false), "Create Spot Light Texture object. diffuse remove the hard ray in the middle, damping make it more foggy" ) {
    SDL_Texture* target_texture = BaseFlux::CreateSpotlightTexture(
        app.getRenderer(), radius, coneAngleDegrees,
        color, diffuse, damping
    );
    if (!target_texture) {
        Con::errorf("Failed to create Spot Light Texture!");
        return 0;
    }
    TextureObject* result = new TextureObject();
    result->registerObject(); // before set because this calles add
    if (!result->set(target_texture, true)) {
        result->deleteObject();
        return 0;
    }
    result->mDefaultBlendMode = SDL_BLENDMODE_ADD;
    return result->getId();
}
// -----------------------------------------------------------------------------
ConsoleFunctionGroupEnd(SDL);

// -----------------------------------------------------------------------------
// this is ported from OmFlux/KorkTest to OmFlux/ElfTest to baseElf
// since it was inital written for KorkTest it use old style ConsoleFunction
// -----------------------------------------------------------------------------
ConsoleFunctionGroupBegin(BaseFlux, "BaseFlux Functions: getFPS, ...");

DefineEngineFunction(getFullScreen, bool, (),, "return true if in fullscreen mode") {
    Uint32 flags = SDL_GetWindowFlags(app.getWindow());
    return (flags & SDL_WINDOW_FULLSCREEN);
}
DefineEngineFunction(setFullScreen, bool,(bool value),, "bool value") {
    return SDL_SetWindowFullscreen(app.getWindow(),value);
}

DefineEngineFunction(getGameTime, F32, (), , "Get the game time (sec since start)") {
    return (F32) BaseFlux::getGameTime();
}

DefineEngineFunction(getFrameTime, F32,(),, "get the current frame time in seconds") {
    return (F32) BaseFlux::getFrameTime();
}

// in SDL3_core.cpp
// // DefineEngineFunction(SDL_GetTicks, U64, (),, "Ticks from SDL") {
// //     return SDL_GetTicks();
// // }


DefineEngineFunction(getRealTime, S32, (),, "get current time from script engine") {
    return Sim::getCurrentTime();
}

DefineEngineFunction(getFPS, S32,(),, "Get the current fps") {
    return (S32)BaseFlux::getFPS();
}
DefineEngineFunction(GetMousePosition, /*Point2I*/ ConsoleVector, (),, "") {
    return { (F32)gMousePos.x,(F32)gMousePos.y,0.f,0.f };
}


DefineEngineFunction(setWindowSize, bool, (S32 x, S32 y), , "") {
   return  SDL_SetWindowSize(app.getWindow(), x, y);
}

DefineEngineFunction(setScreenSize, bool, (S32 logicalWidth, S32 logicalHeight, S32 mode),((S32)SDL_LOGICAL_PRESENTATION_STRETCH) ,
                     "Set SDL_SetRenderLogicalPresentation which does scale the screen.\n"
                     "Warning: ImGui windows (like console) looks a bit strange than!") {
   return SDL_SetRenderLogicalPresentation(app.getRenderer(), logicalWidth, logicalHeight
            ,(SDL_RendererLogicalPresentation) mode);
}
DefineEngineFunction(unSetScreenSize, bool, (), ,
        "Unset SDL_SetRenderLogicalPresentation to use window size\n"
        "Same as setScreenSize(0,0,0);") {
    return SDL_SetRenderLogicalPresentation(app.getRenderer(), 0, 0, SDL_LOGICAL_PRESENTATION_DISABLED);
}

DefineEngineFunction(GetScreenSize, /*Point2I*/ ConsoleVector, (), , "Get the current scaled screen size (SDL_GetRenderLogicalPresentation)") {
    const Point2I& p= ElfSDL3::GetScreenSize();
    return {(F32)p.x, (F32)p.y,0.f,0.f};
}



DefineEngineFunction(getWindowSize, /*Point2I*/ConsoleVector, (), , "") {
    int x, y;
    SDL_GetWindowSize(app.getWindow(), &x, &y);
    return {(F32)x,(F32)y,0.0,0.0};
}

DefineEngineFunction(setVSync, void, (bool value), , "bool value") {
    SDL_SetRenderVSync(app.getRenderer(), (int)value);
}

DefineEngineFunction(getScriptPath, String,(),, "get the current path from the script engine") {
    return Torque::FS::GetCwd().getFullPath();
}

DefineEngineFunction(getAppPath, String,(),, "Get the directory where the application was run from.") {
    return BaseFlux::Tools::getBasePath().c_str();
}



DefineEngineFunction(dumpPathes, void,(),, "print all known pathes on console output") {
    // "base:/ || assets:/ || sound:/ || texture:/ || pref:/ || script:/"

    Con::printf(" --- APP pathes: ---");
    Con::printf("base:/    = %s", getFullPath("base:/").c_str());
    Con::printf("assets:/  = %s", getFullPath("assets:/").c_str());
    Con::printf("sound:/   = %s", getFullPath("sound:/").c_str());
    Con::printf("texture:/ = %s", getFullPath("texture:/").c_str());
    Con::printf("pref:/    = %s", getFullPath("pref:/").c_str());
    Con::printf("script:/  = %s", getFullPath("script:/").c_str());

    Con::printf(" --- OS pathes: ---");
    Con::printf("home:/      = %s", getFullPath("home:/").c_str());
    Con::printf("desktop:/   = %s", getFullPath("desktop:/").c_str());
    Con::printf("documents:/ = %s", getFullPath("documents:/").c_str());
    Con::printf("download:/  = %s", getFullPath("download:/").c_str());
    Con::printf("music:/     = %s", getFullPath("music:/").c_str());
    Con::printf("pictures:/  = %s", getFullPath("pictures:/").c_str());
    Con::printf("videos:/    = %s", getFullPath("videos:/").c_str());

}

DefineEngineFunction(getFullPath, String,(String pathIdent),("base:/"),
        "get the  path defined in settings.\n"
        "@param pathIdent:\n"
        "base:/ || assets:/ || sound:/ || texture:/ || pref:/ || script:/ \n"
        "|| home:/ || desktop:/ || documents:/ || download:/ || music:/ \n"
        "|| pictures || videos"
){
    return getFullPath(pathIdent);
}
// ----------------- include = exec with nocalls ----------------------

DefineEngineFunction( include,bool, (String fileName, bool noCalls),(true), "include(fileName)" "exec a file without calls " ){
    return Con::executeFile(fileName, noCalls);
}
// // ----------------- debuglog ----------------------
// //-----------------------------------------------------------------------------
//
// DefineEngineStringlyVariadicFunction( dEcho, void, 2, 0, "debug echo ( string message... ) ")
// {
//     #ifdef TORQUE_DEBUG
//     U32 len = 0;
//     S32 i;
//     for(i = 1; i < argc; i++)
//         len += dStrlen(argv[i]);
//
//     char *ret = Con::getReturnBuffer(len + 1);
//     ret[0] = 0;
//     for(i = 1; i < argc; i++)
//         dStrcat(ret, argv[i], (U64)(len + 1));
//
//     Con::printf("%s", ret);
//     ret[0] = 0;
//     #endif
// }
//
// //-----------------------------------------------------------------------------
//
// DefineEngineStringlyVariadicFunction( dWarn, void, 2, 0, "debug warn( string message... ) " )
// {
//     #ifdef TORQUE_DEBUG
//     U32 len = 0;
//     S32 i;
//     for(i = 1; i < argc; i++)
//         len += dStrlen(argv[i]);
//
//     char *ret = Con::getReturnBuffer(len + 1);
//     ret[0] = 0;
//     for(i = 1; i < argc; i++)
//         dStrcat(ret, argv[i], (U64)(len + 1));
//
//     Con::warnf(ConsoleLogEntry::General, "%s", ret);
//     ret[0] = 0;
//     #endif
// }
//
// //-----------------------------------------------------------------------------
//
// DefineEngineStringlyVariadicFunction( dError, void, 2, 0, "(debug error  string message... ) ")
// {
//     #ifdef TORQUE_DEBUG
//     U32 len = 0;
//     S32 i;
//     for(i = 1; i < argc; i++)
//         len += dStrlen(argv[i]);
//
//     char *ret = Con::getReturnBuffer(len + 1);
//     ret[0] = 0;
//     for(i = 1; i < argc; i++)
//         dStrcat(ret, argv[i], (U64)(len + 1));
//
//     Con::errorf(ConsoleLogEntry::General, "%s", ret);
//     ret[0] = 0;
//     #endif
// }
//-----------------------------------------------------------------------------
ConsoleFunctionGroupEnd(BaseFlux);
// -----------------------------------------------------------------------------
void postInitBindings_SDL() {
    ElfSDL3::WindowMap.add(app.getWindow());
    ElfSDL3::RendererMap.add(app.getRenderer());

}

// added at bottom
void InitBindings_SDL() {

    ElfSDL3::InitRenderer();
    // we add out window and render here  ;)
    // Too early ^^
    // ElfSDL3::WindowMap.add(app.getWindow());
    // ElfSDL3::RendererMap.add(app.getRenderer());

    registerColors();




    gSettingsObject = new SettingsObject();
    gSettingsObject->registerObject(); //make available on Console
    Con::setIntVariable("$Settings", gSettingsObject->getId());
    // -----
}
void ShutdownBindings_SDL() {
    ElfSDL3::ShutDownRenderer();
    Con::setIntVariable("$Settings", 0);
    gSettingsObject->deleteObject();
}
//-----------------------------------------------------------------------------
