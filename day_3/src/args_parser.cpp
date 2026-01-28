#include "args_parser.h"
#include <iostream>
#include <cstring>

void printUsage() {
    std::cout << "Стеганография в изображениях" << std::endl;
    std::cout << std::endl;
    std::cout << "Использование:" << std::endl;
    std::cout << "  Кодирование: ./stego encode -i <input> -o <output> -m <message> [-s seed]" << std::endl;
    std::cout << "  Декодирование: ./stego decode -i <input> [-s seed]" << std::endl;
    std::cout << std::endl;
    std::cout << "Параметры:" << std::endl;
    std::cout << "  -i, --input    Входной файл изображения" << std::endl;
    std::cout << "  -o, --output   Выходной файл изображения (для encode)" << std::endl;
    std::cout << "  -m, --message  Сообщение для кодирования" << std::endl;
    std::cout << "  -s, --seed     Сид для генератора случайных позиций (по умолчанию: 0) [не используется]" << std::endl;
    std::cout << "  -h, --help     Показать эту справку" << std::endl;
}

void printModeHelp(const std::string& mode) {
    if (mode == "encode") {
        std::cout << "Режим кодирования:" << std::endl;
        std::cout << "  Скрывает сообщение внутри изображения" << std::endl;
    } else if (mode == "decode") {
        std::cout << "Режим декодирования:" << std::endl;
        std::cout << "  Извлекает скрытое сообщение из изображения" << std::endl;
    }
}

ProgramArgs parseArguments(int argc, char* argv[]) {
    ProgramArgs args;
    args.valid = false;
    args.seed = 0;
    args.errorMessage = "";
    
    if (argc < 2) {
        args.errorMessage = "Недостаточно аргументов";
        return args;
    }
    
    args.mode = argv[1];
    
    if (args.mode == "-h" || args.mode == "--help") {
        printUsage();
        args.valid = false;
        return args;
    }
    
    if (args.mode != "encode" && args.mode != "decode") {
        args.errorMessage = "Неизвестный режим: " + args.mode;
        return args;
    }
    
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            args.inputFile = argv[++i];
        }
        else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            args.outputFile = argv[++i];
        }
        else if ((arg == "-m" || arg == "--message") && i + 1 < argc) {
            args.message = argv[++i];
        }
        else if ((arg == "-s" || arg == "--seed") && i + 1 < argc) {
            args.seed = std::stoi(argv[++i]);
        }
        else if (arg == "-h" || arg == "--help") {
            printModeHelp(args.mode);
            return args;
        }
        else {
            args.errorMessage = "Неизвестный аргумент: " + arg;
            return args;
        }
    }
    
    if (args.inputFile.empty()) {
        args.errorMessage = "Не указан входной файл (-i)";
        return args;
    }
    
    if (args.mode == "encode") {
        if (args.outputFile.empty()) {
            args.errorMessage = "Не указан выходной файл (-o)";
            return args;
        }
        if (args.message.empty()) {
            args.errorMessage = "Не указано сообщение (-m)";
            return args;
        }
    }
    
    args.valid = true;
    return args;
}