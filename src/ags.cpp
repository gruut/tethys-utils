#include <algorithm>
#include <bitset>
#include <chrono>
#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <cassert>

#include <botan-2/botan/data_src.h>
#include <botan-2/botan/ecc_key.h>
#include <botan-2/botan/ecdsa.h>
#include <botan-2/botan/exceptn.h>
#include <botan-2/botan/hash.h>
#include <botan-2/botan/hex.h>
#include <botan-2/botan/oids.h>
#include <botan-2/botan/pem.h>
#include <botan-2/botan/pk_keys.h>
#include <botan-2/botan/pkcs8.h>
#include <botan-2/botan/pubkey.h>
#include <botan-2/botan/rng.h>
#include <botan-2/botan/x509cert.h>

#include "ags.hpp"

optional<string> AGS::sign(const string &sk_pem, const string &msg, const string &pass) {
  try {
    Botan::BigInt sk = getPrivateKey(sk_pem, pass);
    return sign(sk, msg);
  } catch (Botan::Exception &exception) {
    cout << "error on PEM to SK: " << exception.what() << endl;
    return {};
  }
}

optional<string> AGS::sign(Botan::BigInt &sk, const string &msg) {
  Signature sig;
  vector<Botan::BigInt> temp_vec;
  try {
    Botan::PointGFp pk = group_domain.blinded_base_point_multiply(sk, rng, temp_vec);
    Botan::BigInt r = group_domain.random_scalar(rng);
    Botan::PointGFp a = group_domain.blinded_base_point_multiply(r, rng, temp_vec);
    sig.d = hash(a);

    Botan::BigInt e = hash(pk, msg);
    Botan::BigInt result = group_domain.multiply_mod_order(r, sig.d) - group_domain.multiply_mod_order(e, sk);
    sig.z = group_domain.mod_order(result);

    return sig.toString();
  } catch (Botan::Exception &exception) {
    cout << "error on gamma sign: " << exception.what() << endl;
    return {};
  }
}

bool AGS::verify(const string &encoded_pk, const string &msg, const string &sig_str) {
  try {
    Botan::PointGFp pk = getPublicKey(encoded_pk);
    return verify(pk, msg, sig_str);
  } catch (Botan::Exception &exception) {
    cout << "error on PEM to PK: " << exception.what() << endl;
    return false;
  }
}

bool AGS::verify(Botan::PointGFp &pk, const string &msg, const string &sig_str) {
  try {
    Signature sig = sigStrToSignature(sig_str);

    Botan::BigInt d_inv = group_domain.inverse_mod_order(sig.d);
    Botan::BigInt z_mul_d_inv = group_domain.multiply_mod_order(sig.z, d_inv);
    Botan::BigInt e_mul_d_inv = group_domain.multiply_mod_order(hash(pk, msg), d_inv);
    Botan::PointGFp A = group_domain.point_multiply(z_mul_d_inv, pk, e_mul_d_inv);
    return hash(A) == sig.d;
  } catch (Botan::Exception &exception) {
    cout << "error on gamma signature verify: " << exception.what() << endl;
    return false;
  }
}

optional<vector<AggregateSig>> AGS::aggregate(vector<AggregateSet> &agg_set, Botan::BigInt &res_z) {
  vector<AggregateSig> agg_sig_vec;
  AggregateSig agg_sig;
  Botan::BigInt d_inv;
  Botan::PointGFp pk, a;
  try {
    for (auto &item : agg_set) {
      auto sig = item.sig;

      d_inv = group_domain.inverse_mod_order(sig.d);
      agg_sig.msg = item.msg;
      pk = getPublicKey(item.encoded_pk);

      a = group_domain.point_multiply(group_domain.multiply_mod_order(sig.z, d_inv), pk,
                                      group_domain.multiply_mod_order(hash(pk, agg_sig.msg), d_inv));
      agg_sig.a_aff_x = a.get_affine_x().to_dec_string();
      agg_sig.a_aff_y = a.get_affine_y().to_dec_string();

      agg_sig.encoded_pk = item.encoded_pk;

      res_z = group_domain.mod_order(res_z + sig.z);

      agg_sig_vec.emplace_back(agg_sig);
    }
  } catch (Botan::Exception &exception) {
    cout << "error on aggregate: " << exception.what() << endl;
    return {};
  }
  return agg_sig_vec;
}

bool AGS::aggregateVerify(vector<AggregateSig> &agg_set, Botan::BigInt &sum_of_z) {
  vector<Botan::BigInt> d_list;
  vector<Botan::BigInt> e_list;
  vector<Botan::PointGFp> pk_list;
  vector<Botan::PointGFp> a_list;
  vector<Botan::BigInt> temp_vec;

  try {
    Botan::PointGFp z_mul_P = group_domain.blinded_base_point_multiply(sum_of_z, rng, temp_vec);
    Botan::PointGFp result = group_domain.zero_point();
    vector<Botan::PointGFp> precomputed_table;
    Botan::PointGFp pk;

    if (!isUniqueElements(agg_set)) {
      return false;
    }

    size_t n = agg_set.size();
    size_t idx = 5;
    size_t rem = idx;
    size_t count = n / idx + 1;

    Botan::PointGFp tmp_a;

    for (size_t i = 0; i < count; ++i) {
      if (n < idx)
        rem = n;

      for (size_t j = i * idx; j < rem + i * idx; ++j) {
        pk = getPublicKey(agg_set.at(j).encoded_pk);
        tmp_a = group_domain.point(Botan::BigInt(agg_set.at(j).a_aff_x), Botan::BigInt(agg_set.at(j).a_aff_y));
        d_list.emplace_back(hash(tmp_a));
        e_list.emplace_back(hash(pk, agg_set.at(j).msg));
        pk_list.emplace_back(pk);
        a_list.emplace_back(tmp_a);
      }

      precomputed_table = precompute(pk_list);
      result += shamirTrick(e_list, precomputed_table);
      precomputed_table = precompute(a_list);
      result -= shamirTrick(d_list, precomputed_table);

      pk_list.clear();
      a_list.clear();
      d_list.clear();
      e_list.clear();
      n -= idx;
    }
    return -z_mul_P == result;

  } catch (Botan::Exception &exception) {
    cout << "error on aggregate verify: " << exception.what() << endl;
    return false;
  }
}

vector<Botan::PointGFp> AGS::precompute(vector<Botan::PointGFp> &point_list) {
  vector<Botan::PointGFp> precomputed_table;
  size_t num_p_size = pow(2, point_list.size());

  for (size_t i = 0; i < num_p_size; ++i) {
    Botan::PointGFp temp_point = group_domain.zero_point();
    bitset<10> precomputed_bit(i);

    for (size_t j = 0; j < point_list.size(); ++j) {
      if (precomputed_bit[j])
        temp_point += point_list.at(j);
    }

    precomputed_table.emplace_back(temp_point);
  }
  return precomputed_table;
}

Botan::PointGFp AGS::shamirTrick(vector<Botan::BigInt> &bigint_list, vector<Botan::PointGFp> &precomputed_table) {
  Botan::PointGFp R = group_domain.zero_point();
  vector<Botan::BigInt> workspace(Botan::PointGFp::WORKSPACE_SIZE);
  int value = 0;
  size_t bigint_size = 0;
  vector<int> temp_vec;

  for (size_t i = 0; i < bigint_list.size(); ++i) {
    if (bigint_size < bigint_list.at(i).bits())
      bigint_size = bigint_list.at(i).bits();
  }

  for (int i = bigint_size - 1; i >= 0; --i) {
    R.mult2(workspace);
    temp_vec.clear();

    for (int j = bigint_list.size() - 1; j >= 0; --j) {
      temp_vec.emplace_back(bigint_list.at(j).get_bit(i));
    }

    value = findIdx(temp_vec);

    if (value != 0)
      R += precomputed_table.at(value);
  }
  return R;
}

int AGS::findIdx(vector<int> &vec) {
  int idx = 0;

  for (size_t i = 0; i < vec.size(); ++i) {
    idx += vec.at(i) * (int)pow(2, vec.size() - 1 - i);
  }
  return idx;
}

Botan::BigInt AGS::hash(Botan::PointGFp &point) {
  unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-256"));
  hash->update(point.get_affine_x().to_hex_string());
  hash->update(point.get_affine_y().to_hex_string());
  return Botan::BigInt::decode(hash->final_stdvec());
}

Botan::BigInt AGS::hash(Botan::PointGFp &point, const string &msg) {
  unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-256"));
  hash->update(point.get_affine_x().to_hex_string());
  hash->update(point.get_affine_y().to_hex_string());
  hash->update(msg);
  return Botan::BigInt::decode(hash->final_stdvec());
}

Botan::BigInt AGS::hash(const string &encoded_pk, const string &msg) {
  try {
    Botan::PointGFp pk = getPublicKey(encoded_pk);
    return hash(pk, msg);
  } catch (Botan::Exception &exception) {
    throw;
  }
}

Botan::BigInt AGS::getPrivateKey(const string &sk_pem, const string &pass) {
  try {
    Botan::DataSource_Memory key_data(sk_pem);
    unique_ptr<Botan::Private_Key> key(Botan::PKCS8::load_key(key_data, pass));
    Botan::ECDSA_PrivateKey private_key(key->algorithm_identifier(), key->private_key_bits());
    return private_key.private_value();
  } catch (Botan::Exception &exception) {
    throw;
  }
}

Botan::PointGFp AGS::getPublicKey(const string &encoded_pk) {
  try {
    vector<uint8_t> vec_x(encoded_pk.begin() + 1, encoded_pk.begin() + 33);
    vector<uint8_t> vec_y(encoded_pk.begin() + 33, encoded_pk.end());
    auto x = Botan::BigInt::decode(vec_x);
    auto y = Botan::BigInt::decode(vec_y);
    return group_domain.point(x, y);
  } catch (Botan::Exception &exception) {
    throw;
  }
}

Botan::PointGFp AGS::getPublicKeyFromPem(const string &pk_pem) {
  try {
    Botan::DataSource_Memory cert_datasource(pk_pem);
    Botan::X509_Certificate cert(cert_datasource);
    Botan::ECDSA_PublicKey public_key(cert.subject_public_key_algo(), cert.subject_public_key_bitstring());
    return public_key.public_point();
  } catch (Botan::Exception &exception) {
    throw;
  }
}

bool AGS::isUniqueElements(vector<AggregateSig> &aggregate_set) {
  vector<string> t_list;
  vector<string> a_list;

  for (auto &item : aggregate_set) {
    t_list.emplace_back(item.encoded_pk + item.msg);
    a_list.emplace_back(item.a_aff_x + item.a_aff_y);
  }

  sort(t_list.begin(), t_list.end());
  auto it_t = unique(t_list.begin(), t_list.end());
  if (it_t != t_list.end())
    return false;

  sort(a_list.begin(), a_list.end());
  auto it_a = unique(a_list.begin(), a_list.end());
  if (it_a != a_list.end())
    return false;

  return t_list.size() == a_list.size();
}

Signature AGS::sigStrToSignature(const string &sig) {
  auto decoded_sig = TypeConverter::decodeBase<64>(sig);

  auto mid_index = 64;

  auto d_str = decoded_sig.substr(0, mid_index);
  auto z_str = decoded_sig.substr(mid_index, decoded_sig.length() - mid_index);

  Signature s;
  s.d = Botan::BigInt("0x" + d_str);
  //s.d.sig_words();

  s.z = Botan::BigInt("0x" + z_str);
  //s.z.sig_words();

  return s;
}
