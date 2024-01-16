#pragma once
#include <iostream>
#include <openssl/rand.h>
#include <vector>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/base.h>
#include <random>
#include <sstream>
#include <iomanip>

class AuthorizationUtil
{
public:
	static auto generateAuthorizationKey(const std::string& user_id, std::string& secretKey)
    {
        auto token = jwt::create()
            .set_type("JWS")
            .set_issuer("auth0")
            .set_payload_claim("user_id", jwt::claim(std::string("user_id")))
            .sign(jwt::algorithm::hs256{secretKey});

        return token;
	}

    static std::string getSecretKey() 
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        // Generate a 256-bit key (32 bytes)
        std::ostringstream oss;
        for (int i = 0; i < 4; ++i) {
            oss << std::hex << std::setw(16) << std::setfill('0') << dis(gen);
        }

        return oss.str();
    }
private:
};