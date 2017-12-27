#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <curl/curl.h>
#include <iostream>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <fstream>
#include <sstream>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <archive.h>
#include <archive_entry.h>
#include <experimental/filesystem>

namespace CDC {
  struct FileOps {
    static constexpr unsigned char key[64] = {
      0x73, 0x7f, 0xf8, 0xc2, 0x1a, 0xcc, 0xa1, 0x57, 0x64, 0x95, 0xe3, 0xea,
      0xa3, 0x16, 0xac, 0x20, 0x7c, 0x25, 0x72, 0xb6, 0xdc, 0x3a, 0x97, 0xd0,
      0x02, 0x1f, 0x70, 0x8e, 0x40, 0xf9, 0xf1, 0xb0, 0xe6, 0x23, 0xd1, 0xa0,
      0x0e, 0xc9, 0x80, 0x9b, 0xad, 0x0d, 0x8a, 0x38, 0xb5, 0xe6, 0x96, 0xea,
      0x8b, 0x66, 0x08, 0x99, 0xf9, 0x16, 0x1e, 0x4d, 0x9b, 0x91, 0x04, 0xfb,
      0xc5, 0x97, 0x0e, 0x57
    };
    static constexpr unsigned int key_len = 64;

    static bool check_hmac(const std::string &fileName, const std::string &inHMAC);
    static bool unpack_firmware(const std::string &fileName);
    static bool verify_hmac();
    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
    static bool tftp_download(const std::string &url, const char *save_file, double *fileSize);
  };
}

#endif // FILE_OPS_H
