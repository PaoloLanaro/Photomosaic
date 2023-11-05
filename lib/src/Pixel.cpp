//
// Created by planaro on 10/4/2023.
//


#include "Pixel.h"

Pixel::Pixel(const unsigned int& red, const unsigned int& green,
             const unsigned int& blue, const unsigned int& alpha) : redValue(red), greenValue(green), blueValue(blue) {
    if (alpha == -1) {
        alphaValue = 255;
    } else {
        alphaValue = alpha;
    }
}

void Pixel::setRedValue(const unsigned int& value) {
    redValue = value;
}

void Pixel::setGreenValue(const unsigned int& value) {
    greenValue = value;
}

void Pixel::setBlueValue(const unsigned int& value) {
    blueValue = value;
}

void Pixel::setAlpha(const unsigned int& value) {
    alphaValue = value;
}

const unsigned int& Pixel::getRedValue() const {
    return redValue;
}

const unsigned int& Pixel::getGreenValue() const {
    return greenValue;
}

const unsigned int& Pixel::getBlueValue() const {
    return blueValue;
}

const unsigned int& Pixel::getAlphaValue() const {
    return alphaValue;
}

bool Pixel::operator<(const Pixel &other) const {
    if (redValue < other.redValue) return true;
    if (redValue > other.redValue) return false;

    if (greenValue < other.greenValue) return true;
    if (greenValue > other.greenValue) return false;

    return blueValue < other.blueValue;
}

bool Pixel::operator==(const Pixel &other) const {
    bool redBool = this->redValue == other.redValue;
    bool greenBool = this->greenValue == other.greenValue;
    bool blueBool = this->blueValue == other.blueValue;
    bool alphaBool = this->alphaValue == other.alphaValue;

    return redBool && greenBool && blueBool && alphaBool;
}

bool Pixel::operator>(const Pixel &other) const {
    if (redValue > other.redValue) return true;
    if (redValue < other.redValue) return false;

    if (greenValue > other.greenValue) return true;
    if (greenValue < other.greenValue) return false;

    return blueValue > other.blueValue;
}
