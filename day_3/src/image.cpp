#include "image.h"
#include <iostream>

// Подключаем stb_image только в этом файле
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image() {
    width = 0;
    height = 0;
    channels = 0;
    pixelData = nullptr;
}

Image::~Image() {
    cleanup();
}

void Image::cleanup() {
    if (pixelData != nullptr) {
        delete[] pixelData;
        pixelData = nullptr;
    }
    width = 0;
    height = 0;
}

bool Image::load(const std::string& filename) {
    // Очищаем предыдущие данные
    cleanup();
    
    // Загружаем изображение через stb_image
    // Принудительно загружаем как RGB (3 канала)
    int loadedChannels;
    unsigned char* rawData = stbi_load(filename.c_str(), &width, &height, &loadedChannels, 3);
    
    if (rawData == nullptr) {
        std::cerr << "Ошибка загрузки изображения: " << stbi_failure_reason() << std::endl;
        return false;
    }
    
    channels = 3;
    
    // Копируем данные в наш формат (int)
    int totalValues = width * height * channels;
    pixelData = new int[totalValues];
    
    for (int i = 0; i < totalValues; i++) {
        pixelData[i] = rawData[i];
    }
    
    // Освобождаем память stb
    stbi_image_free(rawData);
    
    std::cout << "Изображение загружено: " << width << "x" << height << std::endl;
    return true;
}

bool Image::save(const std::string& filename) {
    if (pixelData == nullptr) {
        std::cerr << "Нет данных для сохранения" << std::endl;
        return false;
    }
    
    // Конвертируем обратно в unsigned char для stb
    int totalValues = width * height * channels;
    unsigned char* rawData = new unsigned char[totalValues];
    
    for (int i = 0; i < totalValues; i++) {
        // Ограничиваем значения диапазоном 0-255
        int value = pixelData[i];
        if (value < 0) value = 0;
        if (value > 255) value = 255;
        rawData[i] = (unsigned char)value;
    }
    
    // Сохраняем как PNG
    int result = stbi_write_png(filename.c_str(), width, height, channels, rawData, width * channels);
    
    delete[] rawData;
    
    if (result == 0) {
        std::cerr << "Ошибка сохранения изображения" << std::endl;
        return false;
    }
    
    std::cout << "Изображение сохранено: " << filename << std::endl;
    return true;
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

int Image::getPixelCount() const {
    return width * height;
}

int Image::coordsToIndex(int x, int y) const {
    return (y * width + x) * channels;
}

Pixel Image::getPixel(int x, int y) const {
    Pixel p;
    if (pixelData == nullptr || x < 0 || x >= width || y < 0 || y >= height) {
        return p;
    }
    
    int index = coordsToIndex(x, y);
    p.r = pixelData[index];
    p.g = pixelData[index + 1];
    p.b = pixelData[index + 2];
    return p;
}

void Image::setPixel(int x, int y, Pixel pixel) {
    if (pixelData == nullptr || x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    
    int index = coordsToIndex(x, y);
    pixelData[index] = pixel.r;
    pixelData[index + 1] = pixel.g;
    pixelData[index + 2] = pixel.b;
}

Pixel Image::getPixelByIndex(int index) const {
    int x = index % width;
    int y = index / width;
    return getPixel(x, y);
}

void Image::setPixelByIndex(int index, Pixel pixel) {
    int x = index % width;
    int y = index / width;
    setPixel(x, y, pixel);
}

bool Image::isLoaded() const {
    return pixelData != nullptr;
}