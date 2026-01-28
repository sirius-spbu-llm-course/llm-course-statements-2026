#include "encoder.h"
#include "constants.h"
#include <iostream>

bool canFitMessage(const Image& image, const std::string& message) {
    // Подсчитываем сколько пикселей нужно:
    // - 8 пикселей на каждый символ (по одному пикселю на бит)
    // - 1 пиксель на маркер конца
    
    int messageLength = message.length();
    int bitsForMessage = messageLength * BITS_PER_CHAR;
    int totalPixelsNeeded = bitsForMessage + 1;
    
    int availablePixels = image.getPixelCount();
    
    if (totalPixelsNeeded > availablePixels) {
        std::cerr << "Нужно пикселей: " << totalPixelsNeeded << std::endl;
        std::cerr << "Доступно пикселей: " << availablePixels << std::endl;
        return false;
    }
    
    return true;
}

// Вспомогательная функция: округляет значение канала и добавляет закодированное значение
int encodeValueInChannel(int channelValue, int encodedValue) {
    int rounded = (channelValue / 10) * 10;
    int result = rounded + encodedValue;
    return result;
}

bool encodeMessage(Image& image, const std::string& message) {

    if (!canFitMessage(image, message)) {
        return false;
    }
    
    int currentPixelIndex = 0;
    int messageLength = message.length();
    
    
    for (int charIndex = 0; charIndex < messageLength; charIndex++) {
        char currentChar = message[charIndex];
        int charCode = (int)currentChar;
        
        for (int bitPosition = 7; bitPosition >= 0; bitPosition--) {
            // Извлекаем бит
            int bitValue = (charCode >> bitPosition) & 1;
            
            // Получаем пиксель
            Pixel pixel = image.getPixelByIndex(currentPixelIndex);
            
            // Кодируем бит в красный канал
            pixel.r = encodeValueInChannel(pixel.r, bitValue);
            
            image.setPixelByIndex(currentPixelIndex, pixel);
            currentPixelIndex = currentPixelIndex + 1;
        }
    }
    
    Pixel endPixel = image.getPixelByIndex(currentPixelIndex);
    endPixel.r = encodeValueInChannel(endPixel.r, MARKER_END);
    image.setPixelByIndex(currentPixelIndex, endPixel);
    currentPixelIndex = currentPixelIndex + 1;
    
    std::cout << "Кодирование завершено!" << std::endl;
    std::cout << "  Символов закодировано: " << messageLength << std::endl;
    std::cout << "  Пикселей использовано: " << currentPixelIndex << std::endl;
    std::cout << "  Пикселей в изображении: " << image.getPixelCount() << std::endl;
    
    return true;
}