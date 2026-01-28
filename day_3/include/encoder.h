#ifndef ENCODER_H
#define ENCODER_H

#include "image.h"
#include <string>

bool encodeMessage(Image& image, const std::string& message);

bool canFitMessage(const Image& image, const std::string& message);

#endif // ENCODER_H