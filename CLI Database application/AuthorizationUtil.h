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
            // Decode the JWT
            auto decodedToken = jwt::decode(access_key);

            // Verify the JWT
            jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256(secret_key))
                .with_issuer("auth0")  // Replace with your expected issuer
                .verify(decodedToken);

            // Access the ID from the decoded JWT
            auto userIDClaim = decodedToken.get_payload_claim("user_id");// Assuming subject contains the ID
            if (userIDClaim.get_type() == jwt::json::type::string)
            {

                return userIDClaim.as_string();
            }
            }
            catch (const std::exception& e) {
                std::cerr << "Error verifying/decoding JWT: " << e.what() << std::endl;
                return ""; // Return an empty string or handle error accordingly
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