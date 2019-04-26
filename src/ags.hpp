#ifndef GRUUT_UTILS_AGS_HPP
#define GRUUT_UTILS_AGS_HPP

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <botan-2/botan/auto_rng.h>
#include <botan-2/botan/bigint.h>
#include <botan-2/botan/data_src.h>
#include <botan-2/botan/ec_group.h>
#include <botan-2/botan/ecc_key.h>
#include <botan-2/botan/ecdsa.h>
#include <botan-2/botan/exceptn.h>
#include <botan-2/botan/hash.h>
#include <botan-2/botan/hex.h>
#include <botan-2/botan/oids.h>
#include <botan-2/botan/pem.h>
#include <botan-2/botan/pk_keys.h>
#include <botan-2/botan/pkcs8.h>
#include <botan-2/botan/point_gfp.h>
#include <botan-2/botan/pubkey.h>
#include <botan-2/botan/rng.h>
#include <botan-2/botan/x509cert.h>

using namespace std;

struct Signature {
  Botan::BigInt d;
  Botan::BigInt z;
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

  optional<Signature> sign(const string &sk_pem, const string &msg, const string &pass = "");
  optional<Signature> sign(Botan::BigInt &sk, const string &msg);

  bool verify(const string &encoded_pk, const string &msg, Signature &sig);
  bool verify(Botan::PointGFp &pk, const string &msg, Signature &sig);

  optional<vector<AggregateSig>> aggregate(vector<AggregateSet> &agg_set, Botan::BigInt &res_z);
  bool aggregateVerify(vector<AggregateSig> &agg_set, Botan::BigInt &sum_of_z);

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
  Botan::PointGFp getPublicKeyFromPem(const string &pk_pem);

  bool isUniqueElements(vector<AggregateSig> &aggregate_set);
};

#endif // GRUUT_UTILS_AGS_HPP
