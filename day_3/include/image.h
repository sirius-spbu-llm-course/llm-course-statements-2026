#ifndef IMAGE_H
#define IMAGE_H

#include <string>

struct Pixel {
    int r;  // (0-255)
    int g;  // (0-255)
    int b;  // (0-255)
    
    Pixel() : r(0), g(0), b(0) {}
    Pixel(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

class Image {
public:
    Image();
    ~Image();
    
    // Запрещаем копирование
    Image(const Image& other) = delete;
    Image& operator=(const Image& other) = delete;
    
    // Загружает изображение из файла
    bool load(const std::string& filename);
    
    // Сохраняет изображение в файл PNG
    bool save(const std::string& filename);
    
    // Возвращает ширину изображения
    int getWidth() const;
    
    // Возвращает высоту изображения
    int getHeight() const;
    
    // Возвращает общее количество пикселей
    int getPixelCount() const;
    
    // Получает пиксель по координатам
    Pixel getPixel(int x, int y) const;
    
    // Устанавливает пиксель по координатам
    void setPixel(int x, int y, Pixel pixel);
    
    // Получает пиксель по индексу (для последовательного доступа)
    Pixel getPixelByIndex(int index) const;
    
    // Устанавливает пиксель по индексу
    void setPixelByIndex(int index, Pixel pixel);
    
    // Проверяет, загружено ли изображение
    bool isLoaded() const;

private:
    int width;
    int height;
    int channels;
    
    // Храним пиксели как массив int (R, G, B, R, G, B, ...)
    int* pixelData;
    
    // Освобождает память
    void cleanup();
    
    // Преобразует координаты в индекс массива
    int coordsToIndex(int x, int y) const;
};

#endif // IMAGE_H