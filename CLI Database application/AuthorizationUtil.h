#pragma once
#include <iostream>
#include <openssl/rand.h>
#include <vector>


class AuthorizationUtil
{
public:
	static std::string generateAuthorizationKey()
	{
		const int keyLength = 32;
		std::vector<unsigned char> buffer(keyLength);

		if (RAND_bytes(buffer.data(), keyLength) != 1) {
			std::cerr << "Error: Unable to generate random key." << std::endl;
			return "";
		}

		char hexBuffer[keyLength * 2 + 1];
		for (int i = 0; i < keyLength; i++) {
			sprintf_s(hexBuffer + i * 2, sizeof(hexBuffer) - i * 2, "%02x", buffer[i]);
		}
		hexBuffer[keyLength * 2] = '\0';

		return std::string(hexBuffer);
	}
private:
};