//
// Created by planaro on 10/4/2023.
//

#ifndef IMAGELIB_PIXEL_H
#define IMAGELIB_PIXEL_H

class Pixel {
private:
    unsigned int redValue{};
    unsigned int greenValue{};
    unsigned int blueValue{};
    unsigned int alphaValue{};
public:
    // Constructors
    Pixel(const unsigned int& red, const unsigned int& green, const unsigned int& blue, const unsigned int& alpha = -1);
    Pixel() = default;

    // setters for RGBA. These are sloppy,
    // but the easiest way I found to allow for pixels to change after already have been created

    void setRedValue(const unsigned int& value);
    void setGreenValue(const unsigned int& value);
    void setBlueValue(const unsigned int& value);
    void setAlpha(const unsigned int& value);

    // getters for RGBA

    const unsigned int& getRedValue() const;
    const unsigned int& getGreenValue() const;
    const unsigned int& getBlueValue() const;
    const unsigned int& getAlphaValue() const;

    // mixed operators for comparisons for the AVLMap

    bool operator<(const Pixel &other) const;
    bool operator==(const Pixel &other) const;
    bool operator>(const Pixel& other) const;
};

#endif //IMAGELIB_PIXEL_H
