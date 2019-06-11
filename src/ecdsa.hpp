#ifndef TETHYS_UTILS_ECDSA_HPP
#define TETHYS_UTILS_ECDSA_HPP

#include "type_converter.hpp"

#include <botan-2/botan/auto_rng.h>
#include <botan-2/botan/data_src.h>
#include <botan-2/botan/ecdsa.h>
#include <botan-2/botan/emsa1.h>
#include <botan-2/botan/exceptn.h>
#include <botan-2/botan/pk_keys.h>
#include <botan-2/botan/pkcs8.h>
#include <botan-2/botan/pubkey.h>
#include <botan-2/botan/x509cert.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
static const string EMSA = "EMSA1(SHA-256)"s;

class ECDSA {
public:
  static vector<uint8_t> doSign(string_view ecdsa_secret_key_pem, string_view msg, string_view passphrase = "") {
    try {
      auto msg_bytes = TypeConverter::stringToBytes(msg);

      auto ecdsa_secret_key = getPrivateKey(ecdsa_secret_key_pem, passphrase);
      auto sig = doSign(ecdsa_secret_key, msg_bytes);

      return sig;
    } catch (Botan::Exception &exception) {
      onError(exception);

      return vector<uint8_t>();
    }
  }

  static vector<uint8_t> doSign(string_view ecdsa_secret_key_pem, const vector<uint8_t> &msg_bytes, string_view passphrase = ""sv) {
    try {
      auto ecdsa_secret_key = getPrivateKey(ecdsa_secret_key_pem, passphrase);

      return doSign(ecdsa_secret_key, msg_bytes);
    } catch (Botan::Exception &exception) {
      onError(exception);

      return vector<uint8_t>();
    }
  }

  static bool doVerify(string_view ecdsa_cert_pem, string_view msg, const vector<uint8_t> &signature) {
    try {
      Botan::DataSource_Memory cert_datasource(ecdsa_cert_pem.data());
      Botan::X509_Certificate cert(cert_datasource);
      Botan::ECDSA_PublicKey public_key(cert.subject_public_key_algo(), cert.subject_public_key_bitstring());

      const vector<uint8_t> msg_bytes(msg.begin(), msg.end());
      return doVerify(public_key, msg_bytes, signature);
    } catch (Botan::Exception &exception) {
      onError(exception);

      return false;
    }
  }

  static bool doVerify(string_view ecdsa_cert_pem, const vector<uint8_t> &msg_bytes, const vector<uint8_t> &signature) {
    try {
      Botan::DataSource_Memory cert_datasource(ecdsa_cert_pem.data());
      Botan::X509_Certificate cert(cert_datasource);
      Botan::ECDSA_PublicKey public_key(cert.subject_public_key_algo(), cert.subject_public_key_bitstring());

      return doVerify(public_key, msg_bytes, signature);
    } catch (Botan::Exception &exception) {
      onError(exception);

      return false;
    }
  }

  static bool doVerify(Botan::Public_Key &ecdsa_public_key, const vector<uint8_t> &msg_bytes, const vector<uint8_t> &signature) {
    Botan::PK_Verifier verifier(ecdsa_public_key, EMSA, Botan::Signature_Format::DER_SEQUENCE);

    return verifier.verify_message(msg_bytes, signature);
  }

private:
  static Botan::ECDSA_PrivateKey getPrivateKey(string_view ecdsa_secret_key_pem, string_view passphrase = ""sv) {
    try {
      Botan::DataSource_Memory signkey_datasource(ecdsa_secret_key_pem.data());
      unique_ptr<Botan::Private_Key> signkey(Botan::PKCS8::load_key(signkey_datasource, passphrase.data()));

      return Botan::ECDSA_PrivateKey(signkey->algorithm_identifier(), signkey->private_key_bits());
    } catch (Botan::Exception &exception) {
      onError(exception);

      throw;
    }
  }

  static vector<uint8_t> doSign(Botan::ECDSA_PrivateKey &ecdsa_secret_key, const vector<uint8_t> &msg_bytes) {
    Botan::AutoSeeded_RNG auto_rng;
    Botan::PK_Signer signer(ecdsa_secret_key, auto_rng, EMSA, Botan::Signature_Format::DER_SEQUENCE);

    return signer.sign_message(msg_bytes, auto_rng);
  }

  static void onError(Botan::Exception &exception) {
    // TODO: Logging
    cout << exception.what() << endl;
  }
};

#endif
