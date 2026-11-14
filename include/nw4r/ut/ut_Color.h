#ifndef NW4R_UT_COLOR_H
#define NW4R_UT_COLOR_H
#include <nw4r/types_nw4r.h>

#include <revolution/GX.h>

namespace nw4r {
namespace ut {

struct Color : public GXColor {
public:
    Color() {
        *this = WHITE;
    }
    Color(u32 color) {
        *this = color;
    }
    Color(int red, int green, int blue, int alpha) {
        Set(red, green, blue, alpha);
    }
    Color(const GXColor& rColor) {
        *this = rColor;
    }

    ~Color() {}

    void Set(int red, int green, int blue, int alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

#if defined(NW4R_LITTLE_ENDIAN)
    /* A u32 colour is 0xRRGGBBAA; the bytes in memory are r, g, b, a either way. */
    Color& operator=(u32 color) {
        Set(color >> 24 & 0xFF, color >> 16 & 0xFF, color >> 8 & 0xFF, color & 0xFF);
        return *this;
    }
    Color& operator=(const GXColor& rColor) {
        r = rColor.r;
        g = rColor.g;
        b = rColor.b;
        a = rColor.a;
        return *this;
    }
#else
    Color& operator=(u32 color) {
        ToU32ref() = color;
        return *this;
    }
    Color& operator=(const GXColor& rColor) {
        *this = *reinterpret_cast<const u32*>(&rColor);
        return *this;
    }
#endif

    Color operator|(u32 color) const {
        return Color(ToU32() | color);
    }
    Color operator&(u32 color) const {
        return Color(ToU32() & color);
    }

    u32& ToU32ref() {
        return *reinterpret_cast<u32*>(this);
    }
    const u32& ToU32ref() const {
        return *reinterpret_cast<const u32*>(this);
    }

#if defined(NW4R_LITTLE_ENDIAN)
    u32 ToU32() const {
        return (u32)r << 24 | (u32)g << 16 | (u32)b << 8 | a;
    }

    operator u32() const {
        return ToU32();
    }
#else
    u32 ToU32() const {
        return ToU32ref();
    }

    operator u32() const {
        return ToU32ref();
    }
#endif

    static const u32 RED   = 0xFF0000FF;
    static const u32 GREEN = 0x00FF00FF;
    static const u32 BLUE  = 0x0000FFFF;

    static const u32 CYAN    = 0x00FFFFFF;
    static const u32 MAGENTA = 0xFF00FFFF;
    static const u32 YELLOW  = 0xFFFF00FF;

    static const u32 BLACK = 0x000000FF;
    static const u32 GRAY  = 0x808080FF;
    static const u32 WHITE = 0xFFFFFFFF;

    static const u32 ELEMENT_MIN = 0x00000000;
    static const u32 ELEMENT_MAX = 0xFFFFFFFF;
};

}
}

#endif
