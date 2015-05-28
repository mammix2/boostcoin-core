#include "mixer.h"

CCriticalSection cs_mapMixTxs;
std::map<int64, CMixTx> mapMixTxs;

bool genrsa(RSA* rsa) {
  BIGNUM* e;

  if ((e = BN_new()) != NULL) {
    if (BN_set_word(e, 17) == 1) {
      if (RSA_generate_key_ex(rsa, 2048, e, NULL) > 0) {
	return true;
      }
      else {
	std::cerr << "Cannot generate RSA key (code " << ERR_get_error() << ")" << std::endl;
      }
    }
    else {
      std::cerr << "Cannot set exponent (code " << ERR_get_error() << ")" << std::endl;
    }
  }
  else {
    std::cerr << "Cannot allocate exponent (code " << ERR_get_error() << ")" << std::endl;
  }
  return false;
}

bool strenc(RSA* rsa, std::string from, std::string& to) {
  int tosize, numParts;
  unsigned char* tos;
  int writeSize = RSA_size(rsa);
  int encodeSize = writeSize - 42;
  bool success = true;
  std::string part;

  int currentEncode = from.size();  
  numParts = 0; tosize = 0;
  while (currentEncode > 0) {
    tosize += encodeSize;
    currentEncode -= encodeSize;
    ++numParts;
  }
  tos = (unsigned char*)malloc(writeSize);
  to = "";

  std::cout << "String to encode: " << from << std::endl;

  int i = 0;
  for (; i < numParts-1; ++i) {
    if (RSA_public_encrypt(encodeSize,
			   reinterpret_cast<const unsigned char*>
			   (from.c_str()+(encodeSize*i)),
			   tos, rsa, RSA_PKCS1_OAEP_PADDING) < 0) {
      success = false;
      break;
    }
    std::string prn; prn.assign(from.c_str()+(encodeSize*i), encodeSize);
    std::cout << "Part " << i << ": " << prn << std::endl;
    part.assign(reinterpret_cast<char*>(tos), writeSize);
    to += part;
  }

  if (success) {
    std::cout << from.size() << std::endl
	      << i << std::endl
	      << encodeSize << std::endl
	      << "Final encrypting..." << std::endl;
    if (RSA_public_encrypt(from.size()-(encodeSize*i),
			   reinterpret_cast<const unsigned char*>
			   (from.c_str()+(encodeSize*i)),
			   tos, rsa, RSA_PKCS1_OAEP_PADDING)) {
      std::string prn; prn.assign(from.c_str()+(encodeSize*i), from.size()-(encodeSize*i));
      std::cout << "Part " << i << ": " << prn << std::endl;
      part.assign(reinterpret_cast<char*>(tos), writeSize);
      to += part;
      free(tos);
      std::cout << "Size of encoded string: " << to.size() << std::endl;
      std::cout << "Is it good? " << to.size() % writeSize << std::endl;
      return true;
    }
    else {
      std::cerr << "Could not encrypt string: " << ERR_error_string(ERR_get_error(), NULL) << std::endl;
    }
  }
  else {
    std::cerr << "Could not encrypt string: " << ERR_error_string(ERR_get_error(), NULL) << std::endl;
  }
  return false;
}

bool strdec(RSA* rsa, std::string from, std::string& to) {
  int readSize = RSA_size(rsa);
  int decodeSize = readSize - 42;
  int numParts = from.size() / readSize; // integer division should work without remainder
  unsigned char* tos = (unsigned char*)malloc(decodeSize);
  std::string part;
  bool success = true;
  to = "";

  std::cout << "Parts to decrypt: " << numParts << std::endl;

  if (from.size() % readSize == 0) {
    for (int i = 0; i < numParts; ++i) {
      int written = RSA_private_decrypt(readSize,
					reinterpret_cast<const unsigned char*>
					(from.c_str()+(i*readSize)),
					tos, rsa, RSA_PKCS1_OAEP_PADDING);
      if (written < 0) {
	success = false;
	break;
      }
      part.assign(reinterpret_cast<char*>(tos), written);
      std::cout << "Part " << i << ": " << part << std::endl;
      to += part;
    }
    if (success) {
      free(tos);
      return true;
    }
    else {
      std::cerr << "Could not decrypt string: " << ERR_error_string(ERR_get_error(), NULL) << std::endl;
    }
  }
  else {
    std::cerr << "String is not the right size" << std::endl;
  }
  return false;
}

bool rsa2hex(RSA* rsa, std::string& hex) {
  char* n = BN_bn2hex(rsa->n);
  if (n) {
    char* e = BN_bn2hex(rsa->e);
    if (e) {
      std::cout << "In rsa2hex, n = " << n << " and e = " << e << std::endl;
      hex = n;
      hex += ":";
      hex += e;
      return true;
    }
    else {
      std::cerr << "Could not convert exponent to hex" << std::endl;
    }
  }
  else {
    std::cerr << "Could not convert modulus to hex" << std::endl;
  }
  return false;
}

bool hex2rsa(RSA* rsa, std::string hex) {
  size_t colon = hex.find(':');

  if (colon != std::string::npos) {
    std::string nstr = hex.substr(0, colon);
    std::string estr = hex.substr(colon+1);

    std::cout << "In hex2rsa, nstr = " << nstr << " and e = " << estr << std::endl;

    if (BN_hex2bn(&(rsa->n), nstr.c_str())) {
      if (BN_hex2bn(&(rsa->e), estr.c_str())) {
	return true;
      }
      else {
	std::cerr << "Could not parse exponent to RSA" << std::endl;
      }
    }
    else {
      std::cerr << "Could not parse modulus to RSA" << std::endl;
    }
  }
  else {
    std::cerr << "Could not find colon." << std::endl;
  }
  return false;
}
