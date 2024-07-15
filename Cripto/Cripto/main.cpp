#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <string.h>
#include <sstream>
#include "AES.h"
#include "SHA256.h"
#include "RSA.h"
#include <iomanip>

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define IDC_PLAINTEXT 101
#define IDC_CIPHERTEXT 102
#define IDC_DECRYPTEDTEXT 103
#define IDC_ENCRYPTAES 104
#define IDC_DECRYPTAES 105
#define IDC_SHA256 106
#define IDC_ENCRYPTRSA 107
#define IDC_DECRYPTRSA 108
#define IDC_OUTPUT 109
#define IDC_AESKEY 110
#define IDC_RSAKEY 111
#define _CRT_SECURE_NO_WARNINGS 

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASS wc = { 0 };
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"CryptoAppClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc)) return -1;

    CreateWindow(L"CryptoAppClass", L"Crypto Application", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 600, NULL, NULL, hInst, NULL);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hPlaintext, hCiphertext, hDecryptedText, hOutput, hAESKey, hRSAKey;

    switch (msg) {
    case WM_CREATE:
        CreateWindow(L"static", L"Plaintext:", WS_VISIBLE | WS_CHILD, 20, 20, 120, 25, hwnd, NULL, NULL, NULL);
        hPlaintext = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 20, 300, 25, hwnd, (HMENU)IDC_PLAINTEXT, NULL, NULL);

        CreateWindow(L"static", L"AES Key:", WS_VISIBLE | WS_CHILD, 20, 60, 120, 25, hwnd, NULL, NULL, NULL);
        hAESKey = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 60, 300, 25, hwnd, (HMENU)IDC_AESKEY, NULL, NULL);

        CreateWindow(L"static", L"Ciphertext:", WS_VISIBLE | WS_CHILD, 20, 100, 120, 25, hwnd, NULL, NULL, NULL);
        hCiphertext = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 100, 300, 25, hwnd, (HMENU)IDC_CIPHERTEXT, NULL, NULL);

        CreateWindow(L"static", L"Decrypted Text:", WS_VISIBLE | WS_CHILD, 20, 140, 120, 25, hwnd, NULL, NULL, NULL);
        hDecryptedText = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 140, 300, 25, hwnd, (HMENU)IDC_DECRYPTEDTEXT, NULL, NULL);

        CreateWindow(L"button", L"Encrypt AES", WS_VISIBLE | WS_CHILD, 470, 20, 100, 25, hwnd, (HMENU)IDC_ENCRYPTAES, NULL, NULL);
        CreateWindow(L"button", L"Decrypt AES", WS_VISIBLE | WS_CHILD, 470, 60, 100, 25, hwnd, (HMENU)IDC_DECRYPTAES, NULL, NULL);

        CreateWindow(L"button", L"SHA-256", WS_VISIBLE | WS_CHILD, 470, 100, 100, 25, hwnd, (HMENU)IDC_SHA256, NULL, NULL);

        CreateWindow(L"static", L"RSA Key (Public):", WS_VISIBLE | WS_CHILD, 20, 180, 120, 25, hwnd, NULL, NULL, NULL);
        hRSAKey = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 180, 300, 25, hwnd, (HMENU)IDC_RSAKEY, NULL, NULL);

        CreateWindow(L"button", L"Encrypt RSA", WS_VISIBLE | WS_CHILD, 470, 140, 100, 25, hwnd, (HMENU)IDC_ENCRYPTRSA, NULL, NULL);
        CreateWindow(L"button", L"Decrypt RSA", WS_VISIBLE | WS_CHILD, 470, 180, 100, 25, hwnd, (HMENU)IDC_DECRYPTRSA, NULL, NULL);

        hOutput = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 20, 220, 550, 200, hwnd, (HMENU)IDC_OUTPUT, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_ENCRYPTAES) {
            char plaintext[256], key[256], ciphertext[256];
            GetWindowTextA(hPlaintext, plaintext, 256);
            GetWindowTextA(hAESKey, key, 256);

            uint8_t input[16] = { 0 }, output[16] = { 0 }, aes_key[16] = { 0 };
            strncpy_s((char*)input, sizeof(input), plaintext, _TRUNCATE);
            strncpy_s((char*)aes_key, sizeof(aes_key), key, _TRUNCATE);

            AES_Encrypt(input, aes_key, output);

            std::ostringstream oss;
            for (int i = 0; i < 16; ++i) {
                oss << std::hex << std::setw(2) << std::setfill('0') << (int)output[i];
            }
            SetWindowTextA(hCiphertext, oss.str().c_str());
        }
        else if (LOWORD(wParam) == IDC_DECRYPTAES) {
            char ciphertext[256], key[256], decryptedtext[256];
            GetWindowTextA(hCiphertext, ciphertext, 256);
            GetWindowTextA(hAESKey, key, 256);

            uint8_t input[16], output[16], aes_key[16];
            for (int i = 0; i < 16; ++i) {
                input[i] = std::stoi(std::string(ciphertext).substr(i * 2, 2), nullptr, 16);
            }
            strncpy_s((char*)aes_key, sizeof(aes_key), key, _TRUNCATE);

            AES_Decrypt(input, aes_key, output);

            SetWindowTextA(hDecryptedText, (char*)output);
        }
        else if (LOWORD(wParam) == IDC_SHA256) {
            char plaintext[256];
            GetWindowTextA(hPlaintext, plaintext, 256);

            std::string hash = sha256(plaintext);
            SetWindowTextA(hOutput, hash.c_str());
        }
        else if (LOWORD(wParam) == IDC_ENCRYPTRSA) {
            char plaintext[256], rsakey[256];
            GetWindowTextA(hPlaintext, plaintext, 256);
            GetWindowTextA(hRSAKey, rsakey, 256);

            int n, e, d;
            std::istringstream iss(rsakey);
            iss >> n >> e;

            int ciphertext = encryptRSA(std::stoi(plaintext), e, n);
            SetWindowTextA(hCiphertext, std::to_string(ciphertext).c_str());
        }
        else if (LOWORD(wParam) == IDC_DECRYPTRSA) {
            char ciphertext[256], rsakey[256];
            GetWindowTextA(hCiphertext, ciphertext, 256);
            GetWindowTextA(hRSAKey, rsakey, 256);

            int n, e, d;
            std::istringstream iss(rsakey);
            iss >> n >> d;

            int decryptedtext = decryptRSA(std::stoi(ciphertext), d, n);
            SetWindowTextA(hDecryptedText, std::to_string(decryptedtext).c_str());
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

