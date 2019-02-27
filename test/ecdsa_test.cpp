#include "../lib/catch.hpp"
#include "../src/ecdsa.hpp"

using namespace std;

TEST_CASE("ECDSA") {
  const string ecdsa_cert_pem = "-----BEGIN CERTIFICATE-----\n"
                                "MIIDMDCCAZigAwIBAgIRAOKO1fHMKM2wJiFW8BoKF/4wDQYJKoZIhvcNAQELBQAw\n"
                                "PTEVMBMGA1UEAxMMLy8vLy8vLy8vLzg9MQswCQYDVQQGEwJLUjEXMBUGA1UEChMO\n"
                                "R3J1dXQgTmV0d29ya3MwHhcNMTkwMTA0MDU1NzI4WhcNMjkwMTAxMDU1NzI4WjA9\n"
                                "MRUwEwYDVQQDEwxUVVZTUjBWU0xURT0xCzAJBgNVBAYTAktSMRcwFQYDVQQKEw5H\n"
                                "cnV1dCBOZXR3b3JrczBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABPjiXGecyzz6\n"
                                "rSSIrYysgi6x+AmeJdCAvp0tA/6gHPlkJSrCRomOhc1CEn4o0snV2INbsoOZFk+0\n"
                                "/+eGgl6daxmjdjB0MCEGA1UdDgQaBBhJ9/9a4Sii228d56Bdci280G5U24Q7PLYw\n"
                                "HAYDVR0RBBUwE4ERY29udGFjdEBncnV1dC5uZXQwDAYDVR0TAQH/BAIwADAjBgNV\n"
                                "HSMEHDAagBgYJqo0iW9v/30tECWTkkYcGNgp5t6Ms/0wDQYJKoZIhvcNAQELBQAD\n"
                                "ggGBAInl5sBETRGxd8M023BzDKngKeip4ojtPBjE5wOu0O3tSo+6Z3rob1u0Br6o\n"
                                "gQamb6K3xynsLH4i3oKVMf/3oVdMtuxNYLio6RxsZyjS7r5u1u5OaA6Dk5DlQHqN\n"
                                "PnGXX5/q6HHdmqlo7KkllmjHkAxCshZqxmXO3XA+FkVN0T/rbjsk4X2xdq8dAG8T\n"
                                "b083TNf6htrT0fSzy96kcnyG26MnO/SVIlH6XncukZuHg7WtFDGuyvvfB6DTYxaW\n"
                                "fc/Ay1DtIdPNlb4BasvSh3KkSlgOGluDIMnj2Z5DcvYV1JF1JedNiqC+aiWth/m9\n"
                                "/id1/WXBAuWPXJxdJg+txN65bfLxeY5L8iNcAONYL1jX3fzZLBaLVuh9dV6fSo5x\n"
                                "MIQ9mHMhTUrCq+lbV5ihwTwYBAk8zJJoD/M6V/d/2imvMSU4vWMK2bWqj035fYWo\n"
                                "wJqkw2WHOoNK3DJHznphy9LiuwmV9ZQY3Z2LO3wJcIS5YqYQrEczbzyGKS0F3hz7\n"
                                "3L1vcg==\n"
                                "-----END CERTIFICATE-----";

  const string ecdsa_private_key_pem = "-----BEGIN PRIVATE KEY-----\n"
                                       "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQg9rFM4gkxNEW7Tau4\n"
                                       "YkE1mR9IF7T3u+RiMoL9cedMeDOhRANCAAT44lxnnMs8+q0kiK2MrIIusfgJniXQ\n"
                                       "gL6dLQP+oBz5ZCUqwkaJjoXNQhJ+KNLJ1diDW7KDmRZPtP/nhoJenWsZ\n"
                                       "-----END PRIVATE KEY-----";

  const string msg = "Hello world";

  SECTION("It should work") {
    auto signature = ECDSA::doSign(ecdsa_private_key_pem, msg);
    auto actual = ECDSA::doVerify(ecdsa_cert_pem, msg, signature);

    bool expected = true;
    REQUIRE(actual == expected);
  }
}
