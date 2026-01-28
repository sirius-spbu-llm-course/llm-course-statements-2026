#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <string>

struct ProgramArgs {
    std::string mode;           // "encode" / "decode"
    std::string inputFile;      // Входной файл изображения
    std::string outputFile;     // Выходной файл (для encode)
    std::string message;        // Секретное сообщение
    int seed;
    bool valid;                 // Успешно ли распаршены аргументы
    std::string errorMessage;   // Сообщение об ошибке
};

ProgramArgs parseArguments(int argc, char* argv[]);

void printUsage();

void printModeHelp(const std::string& mode);

#endif // ARGS_PARSER_H