#pragma once
#include <map>
#include <iostream>
#include <string>
#include <cstring>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/err.h>

#include "util.h"
#include "sync.h"

class CMixTx {
 public:
  int stage;
  bool inprogress;
  double amount;
  std::string from;
  std::string to;
  std::string addrName;
  int useboostcoinsend;
  RSA* rsa;

  CMixTx() {}
};

class CMixDisc {
 public:
  double amount;
  int64 mtxid;

  IMPLEMENT_SERIALIZE
    (
     READWRITE(this->amount);
     READWRITE(this->mtxid);
     )

  CMixDisc() {}
};

class CMixAck {
 public:
  int64 mtxid;
  int64 smtxid;

  IMPLEMENT_SERIALIZE
    (
     READWRITE(this->mtxid);
     READWRITE(this->smtxid);
     )

  CMixAck() {};
};

class CMixAccept {
 public:
  int64 mtxid;
  int64 smtxid;
  IMPLEMENT_SERIALIZE
    (
     READWRITE(this->mtxid);
     READWRITE(this->smtxid);
     )

  CMixAccept() {}
};

class CMixKey {
 public:
  int64 mtxid;
  int64 smtxid;
  std::string key;

  IMPLEMENT_SERIALIZE
    (
     READWRITE(this->mtxid);
     READWRITE(this->smtxid);
     READWRITE(this->key);
     )

  CMixKey() {}
};

class CMixStart {
 public:
  int64 mtxid;
  int64 smtxid;
  std::string key;
  std::string to;
  int useboostcoinsend;

  IMPLEMENT_SERIALIZE
    (
     READWRITE(this->mtxid);
     READWRITE(this->smtxid);
     READWRITE(this->key);
     READWRITE(this->to);
     READWRITE(this->useboostcoinsend);
     )

  CMixStart() {};
};

class CMixFin {
 public:
  int64 mtxid;
  std::string address;

  IMPLEMENT_SERIALIZE
    (
     READWRITE(this->mtxid);
     READWRITE(this->address);
     )

  CMixFin() {}
};
/*
class CMixErr {
 public:
  int64 mtxid;
  int64 smtxid;
  int32 code;
  std::string error;

  IMPLEMENT_SERIALIZE
    (
     READWRITE(this->mtxid);
     READWRITE(this->smtxid);
     READWRITE(this->code);
     READWRITE(this->error);
     )
};*/

// they'll always be in order of ID,
// so (mapMixerTxs.end()-1).first + 1 is always a new ID
extern CCriticalSection cs_mapMixTxs;
extern std::map<int64, CMixTx> mapMixTxs;

bool genrsa(RSA* rsa);
bool strenc(RSA* rsa, std::string from, std::string& to);
bool strdec(RSA* rsa, std::string from, std::string& to);
bool rsa2hex(RSA* rsa, std::string& hex);
bool hex2rsa(RSA* rsa, std::string hex);
