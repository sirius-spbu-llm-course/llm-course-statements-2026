#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

TESTS_PASSED=0
TESTS_TOTAL=3
FAILED=0

echo "------------------------------------------------"
echo "Запуск тестов GitHub Classroom для Steganography"
echo "------------------------------------------------"


echo -e "${GREEN}Сборка проекта...${NC}"

make clean 2>/dev/null

make

if [ ! -f ./stego ]; then
    echo -e "${RED}Ошибка: Исполняемый файл ./stego не найден после сборки.${NC}"
    exit 1
fi
echo -e "${GREEN}Сборка прошла успешно.${NC}\n"

cleanup() {
    rm -f test1_out.png test2_out.png test3_seed1337.png test3_seed420.png
}
trap cleanup EXIT

# ТЕСТ 1: Проверка lena512.png с сообщением "First test"
echo "- Тест 1: Encode/Decode 'First test' на images/lena512.png"
MSG_1="First test"
IMAGE_1="images/lena512.png"
OUT_1="test1_out.png"

./stego encode -i "$IMAGE_1" -o "$OUT_1" -m "$MSG_1" > /dev/null 2>&1

if [ ! -f "$OUT_1" ]; then
    echo -e "${RED}[FAIL] Файл $OUT_1 не был создан.${NC}"
    FAILED=1
else
    DECODED_OUTPUT=$(./stego decode -i "$OUT_1")
    
    if echo "$DECODED_OUTPUT" | grep -q "$MSG_1"; then
        echo -e "${GREEN}[PASS] Сообщение успешно декодировано.${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}[FAIL] Ожидалось: '$MSG_1', Получен вывод:${NC}"
        echo "$DECODED_OUTPUT"
        FAILED=1
    fi
fi
echo ""

# ТЕСТ 2: Проверка white5x5.png с сообщением "Hello world!"
# Если не реализовано улучшенное кодирование, то не влезет

echo "- Тест 2: Encode/Decode 'Hello world!' на images/white5x5.png"
MSG_2="Hello world!"
IMAGE_2="images/white5x5.png"
OUT_2="test2_out.png"

if [ ! -f "$IMAGE_2" ]; then
    echo "Внимание: $IMAGE_2 не найден, тест может не пройти."
fi

./stego encode -i "$IMAGE_2" -o "$OUT_2" -m "$MSG_2" > /dev/null 2>&1

if [ ! -f "$OUT_2" ]; then
    echo -e "${RED}[FAIL] Файл $OUT_2 не создан (возможно, сообщение не вмещается).${NC}"
    FAILED=1
else
    DECODED_OUTPUT=$(./stego decode -i "$OUT_2")
    
    if echo "$DECODED_OUTPUT" | grep -q "$MSG_2"; then
        echo -e "${GREEN}[PASS] Сообщение успешно декодировано из маленькой картинки.${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}[FAIL] Ожидалось: '$MSG_2', Получен вывод:${NC}"
        echo "$DECODED_OUTPUT"
        FAILED=1
    fi
fi
echo ""

# ТЕСТ 3: Проверка Seed (Task 3)
# 1. Кодируем с Seed 1337. 
# 2. Пытаемся декодировать с Seed 420. Должно НЕ получиться.
# 3. Пытаемся декодировать с Seed 1337. Должно получиться.

echo "- Тест 3: Проверка реализации --seed (Task 3)"
MSG_3="SecretSeedMessage"
IMAGE_3="images/lena512.png"
OUT_3="test3_seed1337.png"

./stego encode -i "$IMAGE_3" -o "$OUT_3" -m "$MSG_3" --seed 1337 > /dev/null 2>&1

if [ ! -f "$OUT_3" ]; then
    echo -e "${RED}[FAIL] Не удалось создать файл с seed.${NC}"
    FAILED=1
else
    DECODE_WRONG=$(./stego decode -i "$OUT_3" --seed 420)
    
    DECODE_RIGHT=$(./stego decode -i "$OUT_3" --seed 1337)

    if echo "$DECODE_RIGHT" | grep -q "$MSG_3"; then
        if echo "$DECODE_WRONG" | grep -q "$MSG_3"; then
            echo -e "${RED}[FAIL] Сообщение расшифровано с неверным seed (420). Реализация seed не работает.${NC}"
            FAILED=1
        else
            echo -e "${GREEN}[PASS] Seed работает корректно (с 1337 читается, с 420 нет).${NC}"
            ((TESTS_PASSED++))
        fi
    else
        echo -e "${RED}[FAIL] Не удалось декодировать сообщение даже с правильным seed 1337.${NC}"
        echo "Вывод: $DECODE_RIGHT"
        FAILED=1
    fi
fi
echo ""

echo "------------------------------------------------"
if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}Все тесты пройдены! ($TESTS_PASSED/$TESTS_TOTAL)${NC}"
    exit 0
else
    echo -e "${RED}Есть проваленные тесты. ($TESTS_PASSED/$TESTS_TOTAL)${NC}"
    exit 1
fi