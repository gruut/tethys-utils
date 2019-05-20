#ifndef GRUUT_UTILS_AGS_HPP
#define GRUUT_UTILS_AGS_HPP

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#include "type_converter.hpp"
#include <botan-2/botan/auto_rng.h>
#include <botan-2/botan/bigint.h>
#include <botan-2/botan/ec_group.h>
#include <botan-2/botan/point_gfp.h>

using namespace std;

struct Signature {
  Botan::BigInt d;
  Botan::BigInt z;

  string toString() {
    auto d_hex = d.to_hex_string();
    if (d_hex.length() < 64) {
      auto padding_size = 64 - d_hex.length();
      for (auto i = 0; i < padding_size; ++i) {
        d_hex = "0" + d_hex;
      }
    }

    auto z_hex = z.to_hex_string();
    if (z_hex.length() < 64) {
      auto padding_size = 64 - z_hex.length();
      for (auto i = 0; i < padding_size; ++i) {
        z_hex = "0" + z_hex;
      }
    }

    return TypeConverter::encodeBase<64>(d_hex + z_hex);
  }
};

struct AggregateSet {
  string encoded_pk;
  string msg;
  Signature sig;
};

struct AggregateSig {
  string encoded_pk;
  string msg;
  string a_aff_x;
  string a_aff_y;
};

class AGS {
public:
  AGS() : group_domain("secp256k1") {}
  ~AGS() = default;

  optional<string> sign(const string &sk_pem, const string &msg, const string &pass = "");
  optional<string> sign(Botan::BigInt &sk, const string &msg);

  bool verify(const string &encoded_pk, const string &msg, const string &);
  bool verifyPEM(const string &pem, const string &msg, const string &sig_str);
  bool verify(Botan::PointGFp &pk, const string &msg, const string &);

  optional<vector<AggregateSig>> aggregate(vector<AggregateSet> &agg_set, Botan::BigInt &res_z);
  bool aggregateVerify(vector<AggregateSig> &agg_set, Botan::BigInt &sum_of_z);

  Signature sigStrToSignature(const string &sig);

  Botan::PointGFp getPublicKeyFromPem(const string &pk_pem);

private:
  Botan::AutoSeeded_RNG rng;
  Botan::EC_Group group_domain;

  vector<Botan::PointGFp> precompute(vector<Botan::PointGFp> &point_list);
  Botan::PointGFp shamirTrick(vector<Botan::BigInt> &bigint_list, vector<Botan::PointGFp> &precomputed_table);
  int findIdx(vector<int> &vec);

  Botan::BigInt hash(Botan::PointGFp &point);
  Botan::BigInt hash(Botan::PointGFp &point, const string &msg);
  Botan::BigInt hash(const string &encoded_pk, const string &msg);

  Botan::BigInt getPrivateKey(const string &sk_pem, const string &pass = "");
  Botan::PointGFp getPublicKey(const string &encoded_pk);

  bool isUniqueElements(vector<AggregateSig> &aggregate_set);
};

#endif // GRUUT_UTILS_AGS_HPP
