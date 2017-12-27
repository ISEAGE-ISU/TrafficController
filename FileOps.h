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

#define MANIFEST_FILE "/tmp/manifest"
#define FIRMWARE_FILE "/tmp/firmware.bin"

namespace CDC {
  struct FileOps {
    static unsigned char key[64];
    static unsigned int key_len;

    static bool check_hmac(const std::string &fileName, const std::string &inHMAC);
    static bool unpack_firmware(const std::string &fileName);
    static bool verify_hmac();
    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
    static bool tftp_download(const std::string &url, const char *save_file, double *fileSize);
  };
}

#endif // FILE_OPS_H
