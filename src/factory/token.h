/**
*    This is a lightweight chat client.
*    Copyright (C) 2022-2025  氢聊-Hcolda.com
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>

namespace qingliao
{
    struct BigNumImpl;

    class BigNum
    {
    public:
        BigNum(long long num = 0);
        BigNum(const std::string& num, bool is_dec_not_hex);
        BigNum(const BigNum&);
        BigNum(BigNum&&) noexcept;
        ~BigNum();

        std::string getNum(bool is_dec_not_hex) const;

        BigNum gcd(const BigNum&) const;
        BigNum exp(const BigNum&) const;
        BigNum sqrt() const;

        static BigNum generate_rand(int bits);
        static BigNum generate_prime(int bits);

        bool is_prime() const;

        BigNum& operator=(const BigNum&);
        BigNum& operator=(BigNum&&) noexcept;

        BigNum& operator++();
        BigNum& operator--();

        friend BigNum operator+(const BigNum&, const BigNum&);
        friend BigNum operator+(const BigNum&, BigNum&&);
        friend BigNum operator+(BigNum&&, const BigNum&);
        friend BigNum operator+(BigNum&&, BigNum&&);

        friend BigNum operator-(const BigNum&, const BigNum&);
        friend BigNum operator-(const BigNum&, BigNum&&);
        friend BigNum operator-(BigNum&&, const BigNum&);
        friend BigNum operator-(BigNum&&, BigNum&&);

        friend BigNum operator*(const BigNum&, const BigNum&);
        /*friend BigNum operator*(const BigNum&, BigNum&&);
        friend BigNum operator*(BigNum&&, const BigNum&);
        friend BigNum operator*(BigNum&&, BigNum&&);*/

        friend BigNum operator/(const BigNum&, const BigNum&);

        friend BigNum operator%(const BigNum&, const BigNum&);

        // friend BigNum operator&(const BigNum&, const BigNum&);

        // friend BigNum operator|(const BigNum&, const BigNum&);

        // friend BigNum operator^(const BigNum&, const BigNum&);

        friend bool operator==(const BigNum&, const BigNum&);
        friend bool operator<(const BigNum&, const BigNum&);
        friend bool operator>(const BigNum&, const BigNum&);
        friend bool operator!=(const BigNum&, const BigNum&);

        friend void swap(BigNum&, BigNum&);

    private:
        std::shared_ptr<BigNumImpl> m_impl;
    };

    template<class TokenType = size_t>
    class TokenMaker
    {
    public:
        TokenMaker() = default;
        ~TokenMaker() = default;

        TokenType makeNewToken()
        {
            return m_token_recorder++;
        }

    private:
        TokenType m_token_recorder;
    };
}

#endif // !TOKEN_H
