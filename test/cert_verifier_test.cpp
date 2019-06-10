#include "../lib/catch.hpp"
#include "../src/cert_verifier.hpp"

TEST_CASE("CertVerifier") {
  const string cert_pem_1 = "-----BEGIN CERTIFICATE-----\n"
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

  const string rsa_auth_cert_pem = "-----BEGIN CERTIFICATE-----\n"
                                   "MIIFEDCCAz+gAwIBAgIRAL2RcbL4kFwgq5ypdBQ8AxwwRgYJKoZIhvcNAQEKMDmg\n"
                                   "DzANBglghkgBZQMEAgEFAKEcMBoGCSqGSIb3DQEBCDANBglghkgBZQMEAgEFAKID\n"
                                   "AgEgowMCAQEwPTEVMBMGA1UEAxMMLy8vLy8vLy8vLzg9MQswCQYDVQQGEwJLUjEX\n"
                                   "MBUGA1UEChMOR3J1dXQgTmV0d29ya3MwHhcNMTgxMjE1MTMzMDUzWhcNMjgxMjEy\n"
                                   "MTMzMDUzWjA9MRUwEwYDVQQDEwwvLy8vLy8vLy8vOD0xCzAJBgNVBAYTAktSMRcw\n"
                                   "FQYDVQQKEw5HcnV1dCBOZXR3b3JrczCCAaIwDQYJKoZIhvcNAQEBBQADggGPADCC\n"
                                   "AYoCggGBALSTWl5lJH6XmBb/SpI5QGvSfljGA5RucagZZoSMGdIPiCMBsfci9BwG\n"
                                   "ZiYL8jY5+og7FZMrB6d7QzgjNm0oaLJU4rV+T5u+EExnWUliXHLqgjiU5gFtcqB7\n"
                                   "vmstBtKuh2dQpD1ZxhOiBuQhaqWG3pfuBfltyx7qR4WaEZzFzsCW5OH67OU5zTQf\n"
                                   "fA3dEscgXFWQ07tGHzkfgq57SqDq8cSoyLmyBsFdLEELou8qVrhH5w9fHsme2n+B\n"
                                   "lEegZmbc24lueRXxGv42DZO9DL21L1GpLNxSFgj9FKz4YbK2Jhx/dtTrOrg+Yled\n"
                                   "HViEYKka13jlLPgln64R1aAyy40jpWhjmpHYmJqwscoQ0DTQEC/vEUGHJJrAsW25\n"
                                   "1LnTYB/yB+VWLuAFueZFYm8AHvyt93sOin90OXpSV1tXkjoz7/2Rem5SMVQPlnyR\n"
                                   "gm68xxoDgNMOauFZ5JmSgx5sPFHHxwlxwKOyw1EAN9XOVtVGl6w9mXPjbzu+pDfC\n"
                                   "SkOM+mtBlwIDAQABo4GYMIGVMCEGA1UdDgQaBBgYJqo0iW9v/30tECWTkkYcGNgp\n"
                                   "5t6Ms/0wDgYDVR0PAQH/BAQDAgEGMCcGA1UdEQQgMB6BEWNvbnRhY3RAZ3J1dXQu\n"
                                   "bmV0gglncnV1dC5uZXQwEgYDVR0TAQH/BAgwBgEB/wIBATAjBgNVHSMEHDAagBgY\n"
                                   "Jqo0iW9v/30tECWTkkYcGNgp5t6Ms/0wRgYJKoZIhvcNAQEKMDmgDzANBglghkgB\n"
                                   "ZQMEAgEFAKEcMBoGCSqGSIb3DQEBCDANBglghkgBZQMEAgEFAKIDAgEgowMCAQED\n"
                                   "ggGBAIeyCjLC5Y689nLGp+LrFFz5T2O2B4Va+rP2txsfcwlbHKEERuB+QA/uPI+s\n"
                                   "9UJboSjLMH62XINa1XKBEqAvzJDX9L2te37nOLREU7I/Qvrym2NqpgMv9Np2NQwp\n"
                                   "G814DZ5eU15Ucopwxm5B8FLn3efjYly+kyv759wYQZfbdHtrh+5BndwfH7r7i3u+\n"
                                   "Fxtgw0pOwgWExYlEF7spnH8KxbvBlXwc3u3zBNsW4gJrABjMVGald/Ix3N+ajDFb\n"
                                   "eU61gFtL1FdvE0tTNJm+MN4P97p9Mkhvkf79jnbFIIP+/XPF57e6xMVjtktHzJTH\n"
                                   "O7l7rUySeRKo061enzCnoLgx/DAPdsy6S8vTDguQmvH9+hk2a6L2+4TyI0oC0hmc\n"
                                   "dAarJFQPV4VGr5Eczs8cqVMBlzLsAMmj0QoFh3S+cTdgWxjkxrHbe2IHIXj2MJUZ\n"
                                   "oJI4+Emvh0kf8C57nJ/5/oG/GP+MYWZmiXJhwrfhz/wgdgxAkPSDxNLfGB7n12Dt\n"
                                   "AA1+pw==\n"
                                   "-----END CERTIFICATE-----";

  const string cert_pem_2 = "-----BEGIN CERTIFICATE-----\n"
                            "MIIB7zCCAZWgAwIBAgIRALivDtFadz1jn8AgiL2e5N4wCgYIKoZIzj0EAwIwPTEV\n"
                            "MBMGA1UEAxMMLy8vLy8vLy8vLzg9MQswCQYDVQQGEwJLUjEXMBUGA1UEChMOR3J1\n"
                            "dXQgTmV0d29ya3MwHhcNMTkwNjEwMDUyMDM5WhcNMjkwNjA3MDUyMDM5WjA9MRUw\n"
                            "EwYDVQQDEwxUVVZTUjBWU0xURT0xCzAJBgNVBAYTAktSMRcwFQYDVQQKEw5HcnV1\n"
                            "dCBOZXR3b3JrczBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABCVqohk8BT/vMMYx\n"
                            "lBcEsvjZef96gyEtxQYU9plctjoUeOCdiZZpd8k/4lgL1sC7fF2yRxQcPfdJL++J\n"
                            "ti5zvhOjdjB0MCEGA1UdDgQaBBiMkrEydDck12EHLQz09ktgzO8NkxX9JpIwHAYD\n"
                            "VR0RBBUwE4ERY29udGFjdEBncnV1dC5uZXQwDAYDVR0TAQH/BAIwADAjBgNVHSME\n"
                            "HDAagBidbz+YP15U0l49+kDL9FLlkLPm6XmGtu8wCgYIKoZIzj0EAwIDSAAwRQIh\n"
                            "AJy20yVYwqm3YaZM8DfCObitYRUsjBI4lFIX7Xg1bKDdAiA+KzIhn/5LDr9KYgCJ\n"
                            "rENigjt/4RtYARD87ABYA4VEFw==\n"
                            "-----END CERTIFICATE-----";

  const string ecdsa_auth_cert_pem = "-----BEGIN CERTIFICATE-----\n"
                                     "MIICEjCCAbigAwIBAgIRAIq3xNnVeSEfY0Eb0bJejs8wCgYIKoZIzj0EAwIwPTEV\n"
                                     "MBMGA1UEAxMMLy8vLy8vLy8vLzg9MQswCQYDVQQGEwJLUjEXMBUGA1UEChMOR3J1\n"
                                     "dXQgTmV0d29ya3MwHhcNMTkwNjEwMDUxODMwWhcNMjkwNjA3MDUxODMwWjA9MRUw\n"
                                     "EwYDVQQDEwwvLy8vLy8vLy8vOD0xCzAJBgNVBAYTAktSMRcwFQYDVQQKEw5HcnV1\n"
                                     "dCBOZXR3b3JrczBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABL+QL7Hb6DflolvU\n"
                                     "GM3eylwKHP+x9jcMEhCx4AtVVJHcjC5+OkQeB6SHEjDn5HhXTFSoqeK0D51WXyXD\n"
                                     "nn7MugejgZgwgZUwIQYDVR0OBBoEGJ1vP5g/XlTSXj36QMv0UuWQs+bpeYa27zAO\n"
                                     "BgNVHQ8BAf8EBAMCAQYwJwYDVR0RBCAwHoERY29udGFjdEBncnV1dC5uZXSCCWdy\n"
                                     "dXV0Lm5ldDASBgNVHRMBAf8ECDAGAQH/AgEBMCMGA1UdIwQcMBqAGJ1vP5g/XlTS\n"
                                     "Xj36QMv0UuWQs+bpeYa27zAKBggqhkjOPQQDAgNIADBFAiBnHwmlivhFhQaAehmc\n"
                                     "O4HxTT/A0KIkY4hDllxPUfOczwIhALD29f67I+VfG8BAHBsUVPT89fs70j6CspuN\n"
                                     "enCmly3M\n"
                                     "-----END CERTIFICATE-----";

  SECTION("If auth cert algo is RSA") {
    auto actual = CertVerifier::doVerify(cert_pem_1, rsa_auth_cert_pem);
    auto expected = true;

    REQUIRE(actual == expected);
  }

  SECTION("IF auth cert algo is ECDSA") {
    auto actual = CertVerifier::doVerify(cert_pem_2, ecdsa_auth_cert_pem);
    auto expected = true;

    REQUIRE(actual == expected);
  }
}
