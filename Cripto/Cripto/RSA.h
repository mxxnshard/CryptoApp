#pragma once
#ifndef RSA_H
#define RSA_H

#include <vector>
#include <string>

void generateRSAKeys(int& n, int& e, int& d);
int encryptRSA(int msg, int e, int n);
int decryptRSA(int cipher, int d, int n);

#endif