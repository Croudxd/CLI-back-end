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
            .set_payload_claim("user_id", jwt::claim(user_id))
            .sign(jwt::algorithm::hs256{secretKey});

        return token;
	}

    static std::string decodeAuthorizationKey(const std::string& access_key, std::string secret_key)
    {
        try {
            // Decode the access token
            auto decoded = jwt::decode(access_key, jwt::algorithm::hs256{ secret_key });

            // Extract user_id from the payload
            auto user_id_claim = decoded.get_payload_claim("user_id");
            if (decoded.has_payload_claim("user_id")) {
                return user_id_claim.as_string();
            }
            else {
                // Handle the case where "user_id" claim is not present
                std::cerr << "Error: 'user_id' claim not found in the access token." << std::endl;
                return ""; // or some appropriate error value
            }
        }
        catch (const std::exception& e) {
            // Handle decoding errors
            std::cerr << "Error decoding access token: " << e.what() << std::endl;
            return ""; // or some appropriate error value
        }
    }

    static std::string generateSecretKey() 
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
};