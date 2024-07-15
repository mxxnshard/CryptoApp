#include "RSA.h"
#include <random>
#include <tuple>

/*int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;

}
int modmul(int a, int b, int c) {
    int result = 0;
    a = a % c;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result + a) % c;
        }
        a = (a * 2) % c;
        b /= 2;
    }
    return result;
}

int modexp(int base, int exp, int mod) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = modmul(result, base, mod);
        }
        base = modmul(base, base, mod);
        exp /= 2;
    }
    return result;
}

std::tuple<int, int, int> generateRSAKeys() {
    int p = 61; 
    int q = 53; 
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = 3; 
    while (gcd(e, phi) != 1) {
        e += 2;
    }
    int d = 1;
    while ((d * e) % phi != 1) {
        d++;
    }
    return std::make_tuple(n, e, d);
}

int encryptRSA(int plaintext, int e, int n) {
    return modexp(plaintext, e, n);
}

int decryptRSA(int ciphertext, int d, int n) {
    return modexp(ciphertext, d, n);
}*/

#include <iostream>
#include <tuple>
#include <random>
#include <vector>
#include <stdexcept>

// Функция для нахождения наибольшего общего делителя
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Функция для умножения по модулю
int modmul(int a, int b, int mod) {
    int result = 0;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result + a) % mod;
        }
        a = (a * 2) % mod;
        b /= 2;
    }
    return result;
}

// Функция для возведения в степень по модулю
int modexp(int base, int exp, int mod) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = modmul(result, base, mod);
        }
        base = modmul(base, base, mod);
        exp /= 2;
    }
    return result;
}

// Функция для генерации простого числа
int generatePrime() {
    // Набор небольших простых чисел для примера
    std::vector<int> primes = { 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, primes.size() - 1);
    return primes[dis(gen)];
}

// Функция для проверки простоты числа
bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

// Функция для генерации ключей RSA
std::tuple<int, int, int> generateRSAKeys(int publicE) {
    int p = generatePrime();
    int q = generatePrime();
    while (p == q) {
        q = generatePrime();
    }

    int n = p * q;
    int phi = (p - 1) * (q - 1);

    // Проверка, что publicE и φ(n) взаимно просты
    if (gcd(publicE, phi) != 1) {
        throw std::invalid_argument("Введенное значение e не взаимно простое с φ(n). Попробуйте другое значение.");
    }

    // Вычисление приватного ключа d
    int d = 1;
    while ((d * publicE) % phi != 1) {
        d++;
        if (d > phi) {
            throw std::invalid_argument("Не удалось найти d. Попробуйте другое значение e.");
        }
    }

    return std::make_tuple(n, publicE, d);
}

// Функция для шифрования с помощью RSA
int encryptRSA(int plaintext, int e, int n) {
    return modexp(plaintext, e, n);
}

// Функция для расшифровки с помощью RSA
int decryptRSA(int ciphertext, int d, int n) {
    return modexp(ciphertext, d, n);
}

int main() {
    try {
        int e;
        std::cout << "Введите публичный ключ e: ";
        std::cin >> e;

        // Проверка, что e является простым числом
        if (!isPrime(e)) {
            throw std::invalid_argument("Введенное значение e не является простым числом. Попробуйте другое значение.");
        }

        int n, publicE, d;
        std::tie(n, publicE, d) = generateRSAKeys(e);

        std::cout << "Публичный ключ (n, e): (" << n << ", " << publicE << ")\n";
        std::cout << "Приватный ключ d: " << d << "\n";

        int plaintext;
        std::cout << "Введите сообщение для шифрования (целое число): ";
        std::cin >> plaintext;

        int ciphertext = encryptRSA(plaintext, publicE, n);
        std::cout << "Зашифрованное сообщение: " << ciphertext << "\n";

        int decryptedMessage = decryptRSA(ciphertext, d, n);
        std::cout << "Расшифрованное сообщение: " << decryptedMessage << "\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка: " << e.what() << '\n';
    }

    return 0;
}