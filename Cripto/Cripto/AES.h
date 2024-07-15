#pragma once
#ifndef AES_H
#define AES_H

#include <cstdint>

void AES_Encrypt(uint8_t* input, const uint8_t* key, uint8_t* output);
void AES_Decrypt(uint8_t* input, const uint8_t* key, uint8_t* output);


#endif 