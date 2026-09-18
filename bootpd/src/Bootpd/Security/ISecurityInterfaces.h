#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace bootp::Security
{
    using ByteVector = std::vector<uint8_t>;

    enum class Algorithm {
        DSA,
        RSA_MD5,
        RSA_SHA256,
        RSA_SHA384,
        RSA_SHA512,
        ED25519
    };

    struct SignatureResult {
        bool valid;
        std::string algorithm;
        std::string error_message;
        uint64_t verify_time_ms;
    };

    // IBootImageVerifier - Boot Integrity Services
    class IBootImageVerifier {
    public:
        virtual ~IBootImageVerifier() = default;
        
        virtual bool Initialize() = 0;
        virtual void Shutdown() = 0;
        
        // Core BIS functions
        virtual bool VerifyBootImage(
            const ByteVector& image,
            const ByteVector& signature,
            const ByteVector& public_key,
            Algorithm algo) = 0;
        
        virtual bool IsSecureBootEnabled() const = 0;
        virtual std::string GetBootPolicy() const = 0;
        virtual std::string GetVersion() const = 0;
    };

    // ICertificateManager - Certificate handling
    class ICertificateManager {
    public:
        virtual ~ICertificateManager() = default;
        
        virtual bool LoadCertificate(const std::string& pem_path) = 0;
        virtual bool LoadPrivateKey(const std::string& pem_path) = 0;
        virtual bool ValidateCertificate() = 0;
        
        virtual ByteVector GetPublicKey() const = 0;
        virtual std::string GetCertificateSubject() const = 0;
        
        virtual bool SignData(const ByteVector& data, 
                            ByteVector& signature,
                            Algorithm algo) = 0;
        virtual bool VerifySignature(const ByteVector& data,
                                  const ByteVector& signature,
                                  Algorithm algo) = 0;
    };

    // IEventPublisher - For publishing security events
    class IEventPublisher {
    public:
        virtual ~IEventPublisher() = default;
        
        using EventCallback = std::function<void(const std::string& event_type, 
                                                 const std::string& data)>;
        
        virtual void PublishBootVerificationResult(
            bool success,
            const std::string& image_id,
            const std::string& error = "") = 0;
        
        virtual void PublishCertificateValidation(
            bool success,
            const std::string& cert_subject,
            const std::string& error = "") = 0;
        
        virtual void Subscribe(EventCallback callback) = 0;
    };
}
