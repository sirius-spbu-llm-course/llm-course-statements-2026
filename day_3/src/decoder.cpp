#include "decoder.h"
#include "constants.h"
#include <iostream>

// Вспомогательная функция: извлекает закодированное значение из канала
int decodeValueFromChannel(int channelValue) {
    int remainder = channelValue % 10;
    return remainder;
}

std::string decodeMessage(const Image& image) {
    
    std::string resultMessage = "";
    int totalPixels = image.getPixelCount();
    
    int currentByte = 0;
    int bitsCollected = 0;
    
    // Читаем пиксели по порядку до маркера конца
    for (int pixelIndex = 0; pixelIndex < totalPixels; pixelIndex++) {
        Pixel pixel = image.getPixelByIndex(pixelIndex);
        
        int value = decodeValueFromChannel(pixel.r);
        
        if (value == MARKER_END) {
            std::cout << "Маркер конца найден на позиции " << pixelIndex << std::endl;
            break;
        }
        
        // Значение должно быть 0 или 1
        if (value != 0 && value != 1) {
            std::cerr << "Ошибка: неожиданное значение " << value << std::endl;
            break;
        }
        
        // Добавляем бит к текущему байту
        currentByte = (currentByte << 1) | value;
        bitsCollected = bitsCollected + 1;
        
        // Если собрали 8 бит - это готовый символ
        if (bitsCollected == BITS_PER_CHAR) {
            char decodedChar = (char)currentByte;
            resultMessage = resultMessage + decodedChar;
            
            currentByte = 0;
            bitsCollected = 0;
        }
    }
    
    std::cout << "Декодирование завершено!" << std::endl;
    std::cout << "  Символов декодировано: " << resultMessage.length() << std::endl;
    
    return resultMessage;
}