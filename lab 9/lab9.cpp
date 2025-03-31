#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#include <cstring>

typedef unsigned char byte;

class MD5 {
private:
    uint32_t state[4];    // Хеш-значения
    uint32_t count[2];    // Количество битов
    byte buffer[64];      // Буфер для данных

    // Таблица констант
    static const uint32_t T[64];

    // Функции для MD5
    static uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) | (~x & z);
    }

    static uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
        return (x & z) | (y & ~z);
    }

    static uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
        return x ^ y ^ z;
    }

    static uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
        return y ^ (x | ~z);
    }

    // Сдвиг циклический
    static uint32_t rotate_left(uint32_t x, uint32_t n) {
        return (x << n) | (x >> (32 - n));
    }

    // Основная обработка блока
    void transform(const byte block[64]) {
        uint32_t a = state[0];
        uint32_t b = state[1];
        uint32_t c = state[2];
        uint32_t d = state[3];

        uint32_t x[16];
        for (int i = 0; i < 16; ++i) {
            x[i] = (uint32_t)block[i * 4] | ((uint32_t)block[i * 4 + 1] << 8) |
                   ((uint32_t)block[i * 4 + 2] << 16) | ((uint32_t)block[i * 4 + 3] << 24);
        }

        for (int i = 0; i < 64; ++i) {
            uint32_t f, g;

            if (i < 16) {
                f = F(b, c, d);
                g = i;
            } else if (i < 32) {
                f = G(b, c, d);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = H(b, c, d);
                g = (3 * i + 5) % 16;
            } else {
                f = I(b, c, d);
                g = (7 * i) % 16;
            }

            uint32_t temp = d;
            d = c;
            c = b;
            b = b + rotate_left(a + f + T[i] + x[g], (i < 16 || (i > 31 && i < 48)) ? 7 : 5);
            a = temp;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
    }

    // Дополняем сообщение
    void update(const byte* input, size_t length) {
        uint32_t index = count[0] / 8 % 64;
        if ((count[0] += length * 8) < (length * 8)) {
            ++count[1];
        }
        count[1] += length >> 29;

        size_t first_part = 64 - index;
        size_t i = 0;
        if (length >= first_part) {
            std::memcpy(buffer + index, input, first_part);
            transform(buffer);
            for (i = first_part; i + 63 < length; i += 64) {
                transform(input + i);
            }
            index = 0;
        }

        std::memcpy(buffer + index, input + i, length - i);
    }

    // Паддинг и завершение
    void finalize() {
        byte padding[64] = {0x80};  // Заполняем один байт 0x80, остальные нули
        size_t index = count[0] / 8 % 64;
        size_t padding_length = (index < 56) ? (56 - index) : (120 - index);
        update(padding, padding_length);

        byte length[8];
        for (int i = 0; i < 8; ++i) {
            length[i] = (byte)((count[i / 4] >> ((i % 4) * 8)) & 0xFF);
        }
        update(length, 8);
    }

public:
    MD5() {
        state[0] = 0x67452301;
        state[1] = 0xEFCDAB89;
        state[2] = 0x98BADCFE;
        state[3] = 0x10325476;
        count[0] = count[1] = 0;
    }

    void update(const std::string& input) {
        update(reinterpret_cast<const byte*>(input.c_str()), input.length());
    }

    std::string digest() {
        finalize();
        std::stringstream result;
        for (int i = 0; i < 4; ++i) {
            result << std::setw(8) << std::setfill('0') << std::hex << state[i];
        }
        return result.str();
    }
};

const uint32_t MD5::T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0x9b9d2c7e, 0xf9b1f89d, 0x6e1b7c68, 0x8e24cfe0, 0x865e16d7,
    0x8dc5d7c1, 0x7d84c87e, 0x268f5db1, 0x92722c85, 0x1c63b1a9, 0xf1c9cf4b, 0x2ad7e334, 0x9490b58a,
    0x8ab8486e, 0x8e9f1b7c, 0x5da4a5db, 0x3d234cf0, 0x50d5f33f, 0xf5fd0c43, 0x53c0cdd4, 0x2f8ddf52,
    0xf77f9bb2, 0x9c1d0351, 0x65b13748, 0xd5d8b3f0, 0xd30f9c2b, 0x7be0f80e, 0xf0707275, 0x635742f5,
    0x8e4f0e38, 0x9e8d7b30, 0x789db6b5, 0x2e2b8278, 0x55ddf2f6, 0xe4379e28, 0xb0a8d381, 0x9ba60b77
};

// Генерация случайных строк
std::string generate_random_string1(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    
    std::string str(length, '\0');
    for (size_t i = 0; i < length; ++i) {
        str[i] = static_cast<char>(dist(gen));
    }
    return str;
}

std::string generate_random_string(size_t length) {
    static const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return result;
}


// Тест 1: Сравнение хешей для пар строк с разными отличиями
void test_hash_collisions() {
    MD5 md5;
    std::ofstream outfile("collisions_test.csv");
    outfile << "Num Differing Characters,Max Matching Hash Length\n";

    std::vector<size_t> differences = {1, 2, 4, 8, 16};

    for (size_t diff : differences) {
        size_t matching_length_max = 0;

        for (int i = 0; i < 1000; ++i) {
            std::string str1 = generate_random_string(128);
            std::string str2 = str1;
            for (size_t j = 0; j < diff; ++j) {
                str2[j] = (str2[j] == 'a') ? 'b' : 'a'; // изменяем символы
            }

            md5.update(str1);  // Передаем строку для хеширования
            std::string hash1 = md5.digest();  // Получаем хеш

            md5.update(str2);  // Передаем измененную строку
            std::string hash2 = md5.digest();  // Получаем хеш

            // Сравниваем хеши
            size_t matching_length = 0;
            for (size_t j = 0; j < hash1.size(); ++j) {
                if (hash1[j] == hash2[j]) {
                    matching_length++;
                } else {
                    break;
                }
            }
            matching_length_max = std::max(matching_length_max, matching_length);
        }

        outfile << diff << "," << matching_length_max << std::endl;
    }

    std::cout << "Test complete: collisions_test.csv\n";
}


// Тест 2: Генерация хешей для случайных строк и подсчет коллизий
void test_hash_duplicates() {
    MD5 md5;
    std::ofstream outfile("duplicates_test.csv");
    outfile << "Num Hashes,Collisions Count\n";

    for (int i = 2; i <= 6; ++i) {
        size_t N = pow(10, i);
        std::unordered_map<std::string, int> hash_count;
        size_t collisions = 0;

        for (size_t j = 0; j < N; ++j) {
            std::string random_str = generate_random_string(256);
            md5.update(random_str);  // Передаем строку для хеширования
            std::string hash = md5.digest();  // Получаем хеш
            hash_count[hash]++;
        }

        // Подсчитываем количество коллизий
        for (auto& pair : hash_count) {
            if (pair.second > 1) {
                collisions += pair.second - 1;
            }
        }

        outfile << N << "," << collisions << std::endl;
    }

    std::cout << "Test complete: duplicates_test.csv\n";
}


// Тест 3: Измерение времени вычисления хеша для строк разных длин
void test_hash_speed() {
    MD5 md5;
    std::ofstream outfile("speed_test.csv");
    outfile << "String Length,Average Time (ms)\n";

    std::vector<size_t> lengths = {64, 128, 256, 512, 1024, 2048, 4096, 8192};

    for (size_t len : lengths) {
        std::chrono::duration<double> total_time(0);

        for (int i = 0; i < 1000; ++i) {
            std::string random_str = generate_random_string(len);
            auto start = std::chrono::high_resolution_clock::now();
            md5.update(random_str);  // Передаем строку для хеширования
            md5.digest();  // Получаем хеш (результат игнорируется)
            auto end = std::chrono::high_resolution_clock::now();

            total_time += (end - start);
        }

        double average_time = total_time.count() * 1000.0 / 1000; // время в миллисекундах
        outfile << len << "," << average_time << std::endl;
    }

    std::cout << "Test complete: speed_test.csv\n";
}


int main() {
    test_hash_collisions();
    test_hash_duplicates();
    test_hash_speed();

    return 0;
}
