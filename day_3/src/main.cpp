#include <iostream>
#include "args_parser.h"
#include "image.h"
#include "encoder.h"
#include "decoder.h"

#ifdef _WIN32
    #include <windows.h>
#endif


int main(int argc, char* argv[]) {   

    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    // Парсим аргументы командной строки
    ProgramArgs args = parseArguments(argc, argv);
    
    // Проверяем результат парсинга
    if (!args.valid) {
        if (!args.errorMessage.empty()) {
            std::cerr << "Ошибка: " << args.errorMessage << std::endl;
            std::cerr << std::endl;
        }
        printUsage();
        return 1;
    }
    
    // Загружаем изображение
    Image image;
    if (!image.load(args.inputFile)) {
        std::cerr << "Не удалось загрузить изображение: " << args.inputFile << std::endl;
        return 1;
    }
    
    // Выполняем нужную операцию
    if (args.mode == "encode") {
        std::cout << "Кодирование сообщения..." << std::endl;
        
        // Проверяем, поместится ли сообщение
        if (!canFitMessage(image, args.message)) {
            std::cerr << "Ошибка: сообщение слишком длинное для этого изображения" << std::endl;
            return 1;
        }
        
        // Кодируем сообщение
        if (!encodeMessage(image, args.message)) {
            std::cerr << "Ошибка при кодировании сообщения" << std::endl;
            return 1;
        }
        
        // Сохраняем результат
        if (!image.save(args.outputFile)) {
            std::cerr << "Ошибка при сохранении изображения" << std::endl;
            return 1;
        }
        
        std::cout << "Готово! Сообщение закодировано в " << args.outputFile << std::endl;
        
    } else if (args.mode == "decode") {
        std::cout << "Декодирование сообщения..." << std::endl;
        
        // Декодируем сообщение
        std::string message = decodeMessage(image);
        
        if (message.empty()) {
            std::cout << "Сообщение не найдено или пустое" << std::endl;
        } else {
            std::cout << "Найденное сообщение: " << message << std::endl;
        }
    }
    
    return 0;
}