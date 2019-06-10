#ifndef TETHYS_UTILS_CERT_VERIFIER_HPP
#define TETHYS_UTILS_CERT_VERIFIER_HPP

#include <botan-2/botan/data_src.h>
#include <botan-2/botan/ecdsa.h>
#include <botan-2/botan/oids.h>
#include <botan-2/botan/rsa.h>
#include <botan-2/botan/x509cert.h>
#include <string_view>

using namespace std;

class CertVerifier {
public:
  static bool doVerify(string_view cert_to_be_verified_pem, string_view auth_cert_pem) {
    try {
      Botan::DataSource_Memory auth_cert_datasource(auth_cert_pem.data());
      Botan::DataSource_Memory cert_datasource(cert_to_be_verified_pem.data());

      Botan::X509_Certificate auth_cert(auth_cert_datasource);
      Botan::X509_Certificate cert(cert_datasource);

      string algo_type = Botan::OIDS::oid2str(auth_cert.subject_public_key_algo().get_oid());

      if (algo_type.length() < 3)
        return false;

      if (algo_type.find("RSA") != string::npos) {
        Botan::RSA_PublicKey by_pk(auth_cert.subject_public_key_algo(), auth_cert.subject_public_key_bitstring());
        return cert.check_signature(by_pk);
      } else if (algo_type.find("ECDSA") != string::npos) {
        Botan::ECDSA_PublicKey by_pk(auth_cert.subject_public_key_algo(), auth_cert.subject_public_key_bitstring());
        return cert.check_signature(by_pk);
      }

      return false;

    } catch (...) {
      return false;
    }
  }
};

#endif // TETHYS_UTILS_CERT_VERIFIER_HPP
