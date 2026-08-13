// networkworker.cpp
#include "networkworker.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/evp.h>

#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

namespace {

constexpr int AES_KEY_LEN = 32;
constexpr int GCM_IV_LEN  = 12;
constexpr int GCM_TAG_LEN = 16;

std::vector<unsigned char> load_aes_key(const std::string &path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("cannot open AES key file: " + path);
    std::string hex;
    f >> hex;
    if (hex.size() != AES_KEY_LEN * 2) {
        throw std::runtime_error("AES key file must contain " + std::to_string(AES_KEY_LEN * 2) +
                                  " hex chars (got " + std::to_string(hex.size()) + ")");
    }
    std::vector<unsigned char> key(AES_KEY_LEN);
    for (int i = 0; i < AES_KEY_LEN; ++i)
        key[i] = static_cast<unsigned char>(std::stoi(hex.substr(i * 2, 2), nullptr, 16));
    return key;
}

std::string aes_decrypt(const std::vector<unsigned char> &key, const std::string &data) {
    if (data.size() < static_cast<size_t>(GCM_IV_LEN + GCM_TAG_LEN))
        throw std::runtime_error("data too short for IV + tag");

    const unsigned char *iv = reinterpret_cast<const unsigned char *>(data.data());
    const unsigned char *ciphertext = reinterpret_cast<const unsigned char *>(data.data()) + GCM_IV_LEN;
    int ciphertext_len = static_cast<int>(data.size() - GCM_IV_LEN - GCM_TAG_LEN);
    const unsigned char *tag = reinterpret_cast<const unsigned char *>(data.data()) +
                                data.size() - GCM_TAG_LEN;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_CIPHER_CTX_new failed");
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1 ||
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, GCM_IV_LEN, nullptr) != 1 ||
        EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptInit failed");
    }

    std::vector<unsigned char> out(ciphertext_len + 16);
    int out_len1 = 0, out_len2 = 0;
    if (EVP_DecryptUpdate(ctx, out.data(), &out_len1, ciphertext, ciphertext_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptUpdate failed");
    }
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, GCM_TAG_LEN,
                             const_cast<unsigned char *>(tag)) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("set tag failed");
    }
    if (EVP_DecryptFinal_ex(ctx, out.data() + out_len1, &out_len2) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptFinal failed: authentication tag mismatch "
                                  "(data tampered, corrupted, or wrong AES key)");
    }
    EVP_CIPHER_CTX_free(ctx);
    return std::string(reinterpret_cast<char *>(out.data()), out_len1 + out_len2);
}

} // namespace

NetworkWorker::NetworkWorker(QObject *parent) : QObject(parent) {}

void NetworkWorker::connectAndFetch(const QString &hostQ, const QString &portQ,
                                     const QString &usernameQ, const QString &passwordQ,
                                     const QString &remoteFilenameQ,
                                     const QString &caCertPathQ, const QString &aesKeyPathQ) {
    try {
        const std::string host = hostQ.toStdString();
        const std::string port = portQ.toStdString();
        const std::string username = usernameQ.toStdString();
        const std::string password = passwordQ.toStdString();
        const std::string remote_filename = remoteFilenameQ.toStdString();
        const std::string ca_file = caCertPathQ.toStdString();

        std::vector<unsigned char> aes_key = load_aes_key(aesKeyPathQ.toStdString());

        boost::asio::io_context io_context;

        ssl::context ctx(ssl::context::tls_client);
        ctx.load_verify_file(ca_file);

        ssl::stream<tcp::socket> socket(io_context, ctx);
        socket.set_verify_mode(ssl::verify_peer);
        SSL_set_tlsext_host_name(socket.native_handle(), host.c_str());
        socket.set_verify_callback(ssl::host_name_verification(host));

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::connect(socket.lowest_layer(), endpoints);
        socket.handshake(ssl::stream_base::client);

        // --- AUTH ---
        std::string auth_request = "AUTH " + username + " " + password + "\n";
        boost::asio::write(socket, boost::asio::buffer(auth_request));

        boost::asio::streambuf auth_buf;
        boost::asio::read_until(socket, auth_buf, "\n");
        std::istream auth_stream(&auth_buf);
        std::string auth_status;
        auth_stream >> auth_status;

        if (auth_status != "AUTH_OK") {
            std::string reason;
            std::getline(auth_stream, reason);
            emit loginFailed(QString::fromStdString(reason.empty() ? "Invalid credentials" : reason));
            return;
        }

        // --- GET the file ---
        std::string request = "GET " + remote_filename + "\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        boost::asio::streambuf response_buf;
        boost::asio::read_until(socket, response_buf, "\n");

        std::istream response_stream(&response_buf);
        std::string status;
        response_stream >> status;

        if (status != "OK") {
            std::string rest;
            std::getline(response_stream, rest);
            emit errorOccurred(QString::fromStdString("Server error:" + rest));
            return;
        }

        std::size_t total_size = 0;
        response_stream >> total_size;
        response_stream.ignore(1);

        std::string encrypted_data;
        encrypted_data.reserve(total_size);

        if (response_buf.size() > 0) {
            std::ostringstream leftover;
            leftover << &response_buf;
            encrypted_data += leftover.str();
        }

        boost::system::error_code ec;
        while (encrypted_data.size() < total_size) {
            char buf[4096];
            std::size_t n = socket.read_some(boost::asio::buffer(buf), ec);
            if (ec == boost::asio::ssl::error::stream_truncated) break;
            if (ec == boost::asio::error::eof) break;
            if (ec) throw boost::system::system_error(ec);
            encrypted_data.append(buf, n);
        }

        std::string plaintext = aes_decrypt(aes_key, encrypted_data);

        QByteArray pdfData(plaintext.data(), static_cast<int>(plaintext.size()));
        emit fileReceived(pdfData);

    } catch (const std::exception &e) {
        emit errorOccurred(QString::fromStdString(std::string("Connection error: ") + e.what()));
    }
}
