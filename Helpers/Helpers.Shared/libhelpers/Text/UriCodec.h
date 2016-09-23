#pragma once

#include <string>
#include <assert.h>

// based on http://stackoverflow.com/questions/18307429/encode-decode-url-in-c
// http://www.codeguru.com/cpp/cpp/algorithms/strings/article.php/c12759/URI-Encoding-and-Decoding.htm
class UriCodec{
	static const char HEX2DEC[256];
	static const char SAFE[256];
public:
	/*static std::string Decode(const std::string &src);
	static std::string Encode(const std::string &src);*/
	static std::string Decode(const char *src, size_t length);
	static std::string Encode(const char *src, size_t length);
};