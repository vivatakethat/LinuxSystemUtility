/*
 * Copyright (©) 2015 Nate Rosenblum
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef BASE64PP_H_
#define BASE64PP_H_

#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

#ifndef _WIN32
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

namespace base64pp {

	static const char* kEncoding =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class Base64 {
public:
    /** Base64-encode the string. */
    static std::string encode(std::string const& data) NOEXCEPT;

    /** Decode base64-encoded string, throwing on error. */
    static std::string decode(std::string const& data);

    static uint8_t decode_char(char c) {
        if (c >= 'A' && c <= 'Z') {
            return c - 'A';
        } else if (c >= 'a' && c <= 'z') {
            return c - 'a' + 26;
        } else if (c >= '0' && c <= '9') {
            return c - '0' + 52;
        } else if (c == '+') {
            return 62;
        } else if (c == '/') {
            return 63;
        } else {
            throw std::runtime_error("Invalid encoding");
        }
    }
private:
    
};

template<typename Integral, int Bits>
class BitWriter {
public:
    void append(Integral value) {
        int remain = Bits;
        while (remain > 0) {
            if (avail_ == 0) {
                data_.push_back(0);
                avail_ = 8;
            }

            auto take = /*std::*/min(remain, avail_);
            auto shift = avail_ - take;
            auto drop = remain - take;

            data_.back() |= ((value >> drop) << shift);

            remain -= take;
            avail_ -= take;
        }
    }

    std::string to_string() const {
        // Only returning fully written bytes
        return avail_ == 0 ? std::string(data_.begin(), data_.end()) :
            std::string(data_.begin(), data_.end() - 1);
    }
private:
    int avail_ = 0;
    std::vector<char> data_;
};

template<typename Integral, int Bits>
class BitStream {
public:
    explicit BitStream(std::string const& data) : data_(data) {
        if (data_.size() > 0) {
            avail_ = 8;
        }
    }

    Integral next(bool *more) {
        Integral ret = 0;
        int needed = Bits;
        while (needed > 0) {
            // Number of bits to use from the current value
            auto take = /*std::*/min(needed, avail_);

            // Shift over to accumulator's leftmost available position
            auto shift = needed - take;

            // Get the high [take] bits from the current value
            uint8_t interesting =
                (data_[idx_] >> (avail_ - take)) & ((1 << take) - 1);

            ret |= (interesting << shift);

            needed -= take;
            avail_ -= take;

            if (avail_ == 0) {
                if (++idx_ == data_.size()) {
                    *more = false;
                    return ret;
                }
                avail_ = 8;
            }
        }
        *more = true;
        return ret;
    }
private:
    size_t idx_ = 0;
    int avail_ = 0;
    std::string const& data_;
};

inline std::string Base64::encode(std::string const& data) NOEXCEPT {
    if (data.empty()) {
        return std::string();
    }

    size_t bits = data.size() * 8;
    uint8_t rchars = (bits % 24) / 8;
    size_t storage = (bits / 24) * 4 + (rchars == 0 ? 0 : 4);

    bool more = false;
    BitStream<uint8_t, 6> bs(data);

    std::vector<char> ret(storage);
    auto iter = ret.begin();
    do {
        uint8_t bits = bs.next(&more);
        (*iter++) = kEncoding[bits];
    } while (more);

    if (rchars > 0) {
        for (int i = 0; i < 3 - rchars; ++i) {
            (*iter++) = '=';
        }
    }

    return std::string(ret.begin(), ret.end());
}

inline std::string Base64::decode(std::string const& data) {
    if (data.empty()) {
        return std::string();
    }

    auto rit = data.rbegin();
    int eq = 0;
    while (*rit++ == '=') {
        if (++eq > 2) {
            throw std::runtime_error("Invalid encoding");
        }
    }

    BitWriter<uint8_t, 6> writer;
    for (int i = 0; i < data.size() - eq; ++i) {
        uint8_t bits = decode_char(data[i]);
        writer.append(bits);
    }
    return writer.to_string();
}

} // base64pp namespace

#endif // BASE64PP_H_
