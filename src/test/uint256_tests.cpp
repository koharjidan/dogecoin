// Copyright (c) 2011-2013 The Bitcoin Core developers
<<<<<<< HEAD
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
=======
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "arith_uint256.h"
#include "uint256.h"
#include "version.h"
#include "test/test_bitcoin.h"
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707

#include <boost/test/unit_test.hpp>
#include <stdint.h>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <string>
#include <stdio.h>

BOOST_FIXTURE_TEST_SUITE(uint256_tests, BasicTestingSetup)

const unsigned char R1Array[] =
    "\x9c\x52\x4a\xdb\xcf\x56\x11\x12\x2b\x29\x12\x5e\x5d\x35\xd2\xd2"
    "\x22\x81\xaa\xb5\x33\xf0\x08\x32\xd5\x56\xb1\xf9\xea\xe5\x1d\x7d";
const char R1ArrayHex[] = "7D1DE5EAF9B156D53208F033B5AA8122D2d2355d5e12292b121156cfdb4a529c";
const uint256 R1L = uint256(std::vector<unsigned char>(R1Array,R1Array+32));
const uint160 R1S = uint160(std::vector<unsigned char>(R1Array,R1Array+20));

const unsigned char R2Array[] =
    "\x70\x32\x1d\x7c\x47\xa5\x6b\x40\x26\x7e\x0a\xc3\xa6\x9c\xb6\xbf"
    "\x13\x30\x47\xa3\x19\x2d\xda\x71\x49\x13\x72\xf0\xb4\xca\x81\xd7";
const uint256 R2L = uint256(std::vector<unsigned char>(R2Array,R2Array+32));
const uint160 R2S = uint160(std::vector<unsigned char>(R2Array,R2Array+20));

const unsigned char ZeroArray[] =
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
const uint256 ZeroL = uint256(std::vector<unsigned char>(ZeroArray,ZeroArray+32));
const uint160 ZeroS = uint160(std::vector<unsigned char>(ZeroArray,ZeroArray+20));

const unsigned char OneArray[] =
    "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
const uint256 OneL = uint256(std::vector<unsigned char>(OneArray,OneArray+32));
const uint160 OneS = uint160(std::vector<unsigned char>(OneArray,OneArray+20));

const unsigned char MaxArray[] =
    "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
    "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff";
const uint256 MaxL = uint256(std::vector<unsigned char>(MaxArray,MaxArray+32));
const uint160 MaxS = uint160(std::vector<unsigned char>(MaxArray,MaxArray+20));

std::string ArrayToString(const unsigned char A[], unsigned int width)
{
    std::stringstream Stream;
    Stream << std::hex;
    for (unsigned int i = 0; i < width; ++i)
    {
        Stream<<std::setw(2)<<std::setfill('0')<<(unsigned int)A[width-i-1];
    }
    return Stream.str();
}

inline uint160 uint160S(const char *str)
{
    uint160 rv;
    rv.SetHex(str);
    return rv;
}
inline uint160 uint160S(const std::string& str)
{
    uint160 rv;
    rv.SetHex(str);
    return rv;
}

BOOST_AUTO_TEST_CASE( basics ) // constructors, equality, inequality
{
    BOOST_CHECK(1 == 0+1);
    // constructor uint256(vector<char>):
    BOOST_CHECK(R1L.ToString() == ArrayToString(R1Array,32));
    BOOST_CHECK(R1S.ToString() == ArrayToString(R1Array,20));
    BOOST_CHECK(R2L.ToString() == ArrayToString(R2Array,32));
    BOOST_CHECK(R2S.ToString() == ArrayToString(R2Array,20));
    BOOST_CHECK(ZeroL.ToString() == ArrayToString(ZeroArray,32));
    BOOST_CHECK(ZeroS.ToString() == ArrayToString(ZeroArray,20));
    BOOST_CHECK(OneL.ToString() == ArrayToString(OneArray,32));
    BOOST_CHECK(OneS.ToString() == ArrayToString(OneArray,20));
    BOOST_CHECK(MaxL.ToString() == ArrayToString(MaxArray,32));
    BOOST_CHECK(MaxS.ToString() == ArrayToString(MaxArray,20));
    BOOST_CHECK(OneL.ToString() != ArrayToString(ZeroArray,32));
    BOOST_CHECK(OneS.ToString() != ArrayToString(ZeroArray,20));

    // == and !=
    BOOST_CHECK(R1L != R2L && R1S != R2S);
    BOOST_CHECK(ZeroL != OneL && ZeroS != OneS);
    BOOST_CHECK(OneL != ZeroL && OneS != ZeroS);
    BOOST_CHECK(MaxL != ZeroL && MaxS != ZeroS);

    // String Constructor and Copy Constructor
    BOOST_CHECK(uint256S("0x"+R1L.ToString()) == R1L);
    BOOST_CHECK(uint256S("0x"+R2L.ToString()) == R2L);
    BOOST_CHECK(uint256S("0x"+ZeroL.ToString()) == ZeroL);
    BOOST_CHECK(uint256S("0x"+OneL.ToString()) == OneL);
    BOOST_CHECK(uint256S("0x"+MaxL.ToString()) == MaxL);
    BOOST_CHECK(uint256S(R1L.ToString()) == R1L);
    BOOST_CHECK(uint256S("   0x"+R1L.ToString()+"   ") == R1L);
    BOOST_CHECK(uint256S("") == ZeroL);
    BOOST_CHECK(R1L == uint256S(R1ArrayHex));
    BOOST_CHECK(uint256(R1L) == R1L);
    BOOST_CHECK(uint256(ZeroL) == ZeroL);
    BOOST_CHECK(uint256(OneL) == OneL);

    BOOST_CHECK(uint160S("0x"+R1S.ToString()) == R1S);
    BOOST_CHECK(uint160S("0x"+R2S.ToString()) == R2S);
    BOOST_CHECK(uint160S("0x"+ZeroS.ToString()) == ZeroS);
    BOOST_CHECK(uint160S("0x"+OneS.ToString()) == OneS);
    BOOST_CHECK(uint160S("0x"+MaxS.ToString()) == MaxS);
    BOOST_CHECK(uint160S(R1S.ToString()) == R1S);
    BOOST_CHECK(uint160S("   0x"+R1S.ToString()+"   ") == R1S);
    BOOST_CHECK(uint160S("") == ZeroS);
    BOOST_CHECK(R1S == uint160S(R1ArrayHex));

    BOOST_CHECK(uint160(R1S) == R1S);
    BOOST_CHECK(uint160(ZeroS) == ZeroS);
    BOOST_CHECK(uint160(OneS) == OneS);
<<<<<<< HEAD

    // uint64_t constructor
    BOOST_CHECK( (R1L & uint256("0xffffffffffffffff")) == uint256(R1LLow64));
    BOOST_CHECK(ZeroL == uint256(0));
    BOOST_CHECK(OneL == uint256(1));
    BOOST_CHECK(uint256("0xffffffffffffffff") = uint256(0xffffffffffffffffULL));
    BOOST_CHECK( (R1S & uint160("0xffffffffffffffff")) == uint160(R1LLow64));
    BOOST_CHECK(ZeroS == uint160(0));
    BOOST_CHECK(OneS == uint160(1));
    BOOST_CHECK(uint160("0xffffffffffffffff") = uint160(0xffffffffffffffffULL));

    // Assignment (from base_uint)
    uint256 tmpL = ~ZeroL; BOOST_CHECK(tmpL == ~ZeroL);
    tmpL = ~OneL; BOOST_CHECK(tmpL == ~OneL);
    tmpL = ~R1L; BOOST_CHECK(tmpL == ~R1L);
    tmpL = ~R2L; BOOST_CHECK(tmpL == ~R2L);
    tmpL = ~MaxL; BOOST_CHECK(tmpL == ~MaxL);
    uint160 tmpS = ~ZeroS; BOOST_CHECK(tmpS == ~ZeroS);
    tmpS = ~OneS; BOOST_CHECK(tmpS == ~OneS);
    tmpS = ~R1S; BOOST_CHECK(tmpS == ~R1S);
    tmpS = ~R2S; BOOST_CHECK(tmpS == ~R2S);
    tmpS = ~MaxS; BOOST_CHECK(tmpS == ~MaxS);

    // Wrong length must throw exception.
    BOOST_CHECK_THROW(uint256(std::vector<unsigned char>(OneArray,OneArray+31)), uint_error);
    BOOST_CHECK_THROW(uint256(std::vector<unsigned char>(OneArray,OneArray+20)), uint_error);
    BOOST_CHECK_THROW(uint160(std::vector<unsigned char>(OneArray,OneArray+32)), uint_error);
    BOOST_CHECK_THROW(uint160(std::vector<unsigned char>(OneArray,OneArray+19)), uint_error);
}

void shiftArrayRight(unsigned char* to, const unsigned char* from, unsigned int arrayLength, unsigned int bitsToShift) 
{
    for (unsigned int T=0; T < arrayLength; ++T) 
    {
        unsigned int F = (T+bitsToShift/8);
        if (F < arrayLength) 
            to[T]  = from[F] >> (bitsToShift%8);
        else
            to[T] = 0;
        if (F + 1 < arrayLength) 
            to[T] |= from[(F+1)] << (8-bitsToShift%8);
    }
}

void shiftArrayLeft(unsigned char* to, const unsigned char* from, unsigned int arrayLength, unsigned int bitsToShift) 
{
    for (unsigned int T=0; T < arrayLength; ++T) 
    {
        if (T >= bitsToShift/8) 
        {
            unsigned int F = T-bitsToShift/8;
            to[T]  = from[F] << (bitsToShift%8);
            if (T >= bitsToShift/8+1)
                to[T] |= from[F-1] >> (8-bitsToShift%8);
        }
        else {
            to[T] = 0;
        }
    }
}

BOOST_AUTO_TEST_CASE( shifts ) { // "<<"  ">>"  "<<="  ">>="
    unsigned char TmpArray[32];
    uint256 TmpL;
    for (unsigned int i = 0; i < 256; ++i)
    {
        shiftArrayLeft(TmpArray, OneArray, 32, i);
        BOOST_CHECK(uint256(std::vector<unsigned char>(TmpArray,TmpArray+32)) == (OneL << i));
        TmpL = OneL; TmpL <<= i;
        BOOST_CHECK(TmpL == (OneL << i));
        BOOST_CHECK((HalfL >> (255-i)) == (OneL << i));
        TmpL = HalfL; TmpL >>= (255-i);
        BOOST_CHECK(TmpL == (OneL << i));
                    
        shiftArrayLeft(TmpArray, R1Array, 32, i);
        BOOST_CHECK(uint256(std::vector<unsigned char>(TmpArray,TmpArray+32)) == (R1L << i));
        TmpL = R1L; TmpL <<= i;
        BOOST_CHECK(TmpL == (R1L << i));

        shiftArrayRight(TmpArray, R1Array, 32, i);
        BOOST_CHECK(uint256(std::vector<unsigned char>(TmpArray,TmpArray+32)) == (R1L >> i)); 
        TmpL = R1L; TmpL >>= i;
        BOOST_CHECK(TmpL == (R1L >> i));

        shiftArrayLeft(TmpArray, MaxArray, 32, i);
        BOOST_CHECK(uint256(std::vector<unsigned char>(TmpArray,TmpArray+32)) == (MaxL << i));
        TmpL = MaxL; TmpL <<= i;
        BOOST_CHECK(TmpL == (MaxL << i));

        shiftArrayRight(TmpArray, MaxArray, 32, i);
        BOOST_CHECK(uint256(std::vector<unsigned char>(TmpArray,TmpArray+32)) == (MaxL >> i));
        TmpL = MaxL; TmpL >>= i;
        BOOST_CHECK(TmpL == (MaxL >> i));
    }
    uint256 c1L = uint256(0x0123456789abcdefULL);
    uint256 c2L = c1L << 128;
    for (unsigned int i = 0; i < 128; ++i) {
        BOOST_CHECK((c1L << i) == (c2L >> (128-i)));
    }
    for (unsigned int i = 128; i < 256; ++i) {
        BOOST_CHECK((c1L << i) == (c2L << (i-128)));
    }

    uint160 TmpS;
    for (unsigned int i = 0; i < 160; ++i)
    {
        shiftArrayLeft(TmpArray, OneArray, 20, i);
        BOOST_CHECK(uint160(std::vector<unsigned char>(TmpArray,TmpArray+20)) == (OneS << i));
        TmpS = OneS; TmpS <<= i;
        BOOST_CHECK(TmpS == (OneS << i));
        BOOST_CHECK((HalfS >> (159-i)) == (OneS << i));
        TmpS = HalfS; TmpS >>= (159-i);
        BOOST_CHECK(TmpS == (OneS << i));
                    
        shiftArrayLeft(TmpArray, R1Array, 20, i);
        BOOST_CHECK(uint160(std::vector<unsigned char>(TmpArray,TmpArray+20)) == (R1S << i));
        TmpS = R1S; TmpS <<= i;
        BOOST_CHECK(TmpS == (R1S << i));

        shiftArrayRight(TmpArray, R1Array, 20, i);
        BOOST_CHECK(uint160(std::vector<unsigned char>(TmpArray,TmpArray+20)) == (R1S >> i)); 
        TmpS = R1S; TmpS >>= i;
        BOOST_CHECK(TmpS == (R1S >> i));

        shiftArrayLeft(TmpArray, MaxArray, 20, i);
        BOOST_CHECK(uint160(std::vector<unsigned char>(TmpArray,TmpArray+20)) == (MaxS << i));
        TmpS = MaxS; TmpS <<= i;
        BOOST_CHECK(TmpS == (MaxS << i));

        shiftArrayRight(TmpArray, MaxArray, 20, i);
        BOOST_CHECK(uint160(std::vector<unsigned char>(TmpArray,TmpArray+20)) == (MaxS >> i));
        TmpS = MaxS; TmpS >>= i;
        BOOST_CHECK(TmpS == (MaxS >> i));
    }
    uint160 c1S = uint160(0x0123456789abcdefULL);
    uint160 c2S = c1S << 80;
    for (unsigned int i = 0; i < 80; ++i) {
        BOOST_CHECK((c1S << i) == (c2S >> (80-i)));
    }
    for (unsigned int i = 80; i < 160; ++i) {
        BOOST_CHECK((c1S << i) == (c2S << (i-80)));
    }
}

BOOST_AUTO_TEST_CASE( unaryOperators ) // !    ~    -
{
    BOOST_CHECK(!ZeroL);  BOOST_CHECK(!ZeroS);
    BOOST_CHECK(!(!OneL));BOOST_CHECK(!(!OneS));
    for (unsigned int i = 0; i < 256; ++i) 
        BOOST_CHECK(!(!(OneL<<i)));
    for (unsigned int i = 0; i < 160; ++i) 
        BOOST_CHECK(!(!(OneS<<i)));
    BOOST_CHECK(!(!R1L));BOOST_CHECK(!(!R1S));
    BOOST_CHECK(!(!R2S));BOOST_CHECK(!(!R2S)); 
    BOOST_CHECK(!(!MaxL));BOOST_CHECK(!(!MaxS));

    BOOST_CHECK(~ZeroL == MaxL); BOOST_CHECK(~ZeroS == MaxS);

    unsigned char TmpArray[32];
    for (unsigned int i = 0; i < 32; ++i) { TmpArray[i] = ~R1Array[i]; } 
    BOOST_CHECK(uint256(std::vector<unsigned char>(TmpArray,TmpArray+32)) == (~R1L));
    BOOST_CHECK(uint160(std::vector<unsigned char>(TmpArray,TmpArray+20)) == (~R1S));

    BOOST_CHECK(-ZeroL == ZeroL); BOOST_CHECK(-ZeroS == ZeroS);
    BOOST_CHECK(-R1L == (~R1L)+1);
    BOOST_CHECK(-R1S == (~R1S)+1);
    for (unsigned int i = 0; i < 256; ++i) 
        BOOST_CHECK(-(OneL<<i) == (MaxL << i));
    for (unsigned int i = 0; i < 160; ++i) 
        BOOST_CHECK(-(OneS<<i) == (MaxS << i));
}


// Check if doing _A_ _OP_ _B_ results in the same as applying _OP_ onto each
// element of Aarray and Barray, and then converting the result into a uint256.
#define CHECKBITWISEOPERATOR(_A_,_B_,_OP_)                              \
    for (unsigned int i = 0; i < 32; ++i) { TmpArray[i] = _A_##Array[i] _OP_ _B_##Array[i]; } \
    BOOST_CHECK(uint256(std::vector<unsigned char>(TmpArray,TmpArray+32)) == (_A_##L _OP_ _B_##L)); \
    for (unsigned int i = 0; i < 20; ++i) { TmpArray[i] = _A_##Array[i] _OP_ _B_##Array[i]; } \
    BOOST_CHECK(uint160(std::vector<unsigned char>(TmpArray,TmpArray+20)) == (_A_##S _OP_ _B_##S));

#define CHECKASSIGNMENTOPERATOR(_A_,_B_,_OP_)                           \
    TmpL = _A_##L; TmpL _OP_##= _B_##L; BOOST_CHECK(TmpL == (_A_##L _OP_ _B_##L)); \
    TmpS = _A_##S; TmpS _OP_##= _B_##S; BOOST_CHECK(TmpS == (_A_##S _OP_ _B_##S));

BOOST_AUTO_TEST_CASE( bitwiseOperators ) 
{
    unsigned char TmpArray[32];
    
    CHECKBITWISEOPERATOR(R1,R2,|)
    CHECKBITWISEOPERATOR(R1,R2,^)
    CHECKBITWISEOPERATOR(R1,R2,&)
    CHECKBITWISEOPERATOR(R1,Zero,|)
    CHECKBITWISEOPERATOR(R1,Zero,^)
    CHECKBITWISEOPERATOR(R1,Zero,&)
    CHECKBITWISEOPERATOR(R1,Max,|)
    CHECKBITWISEOPERATOR(R1,Max,^)
    CHECKBITWISEOPERATOR(R1,Max,&)
    CHECKBITWISEOPERATOR(Zero,R1,|)
    CHECKBITWISEOPERATOR(Zero,R1,^)
    CHECKBITWISEOPERATOR(Zero,R1,&)
    CHECKBITWISEOPERATOR(Max,R1,|)
    CHECKBITWISEOPERATOR(Max,R1,^)
    CHECKBITWISEOPERATOR(Max,R1,&)

    uint256 TmpL;
    uint160 TmpS;
    CHECKASSIGNMENTOPERATOR(R1,R2,|)
    CHECKASSIGNMENTOPERATOR(R1,R2,^)
    CHECKASSIGNMENTOPERATOR(R1,R2,&)
    CHECKASSIGNMENTOPERATOR(R1,Zero,|)
    CHECKASSIGNMENTOPERATOR(R1,Zero,^)
    CHECKASSIGNMENTOPERATOR(R1,Zero,&)
    CHECKASSIGNMENTOPERATOR(R1,Max,|)
    CHECKASSIGNMENTOPERATOR(R1,Max,^)
    CHECKASSIGNMENTOPERATOR(R1,Max,&)
    CHECKASSIGNMENTOPERATOR(Zero,R1,|)
    CHECKASSIGNMENTOPERATOR(Zero,R1,^)
    CHECKASSIGNMENTOPERATOR(Zero,R1,&)
    CHECKASSIGNMENTOPERATOR(Max,R1,|)
    CHECKASSIGNMENTOPERATOR(Max,R1,^)
    CHECKASSIGNMENTOPERATOR(Max,R1,&)

    uint64_t Tmp64 = 0xe1db685c9a0b47a2ULL; 
    TmpL = R1L; TmpL |= Tmp64;  BOOST_CHECK(TmpL == (R1L | uint256(Tmp64)));
    TmpS = R1S; TmpS |= Tmp64;  BOOST_CHECK(TmpS == (R1S | uint160(Tmp64)));
    TmpL = R1L; TmpL |= 0; BOOST_CHECK(TmpL == R1L);
    TmpS = R1S; TmpS |= 0; BOOST_CHECK(TmpS == R1S);
    TmpL ^= 0; BOOST_CHECK(TmpL == R1L);
    TmpS ^= 0; BOOST_CHECK(TmpS == R1S);
    TmpL ^= Tmp64;  BOOST_CHECK(TmpL == (R1L ^ uint256(Tmp64)));
    TmpS ^= Tmp64;  BOOST_CHECK(TmpS == (R1S ^ uint160(Tmp64)));
=======
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
}

BOOST_AUTO_TEST_CASE( comparison ) // <= >= < >
{
    uint256 LastL;
    for (int i = 255; i >= 0; --i) {
        uint256 TmpL;
        *(TmpL.begin() + (i>>3)) |= 1<<(7-(i&7));
        BOOST_CHECK( LastL < TmpL );
        LastL = TmpL;
    }

    BOOST_CHECK( ZeroL < R1L );
    BOOST_CHECK( R2L < R1L );
    BOOST_CHECK( ZeroL < OneL );
    BOOST_CHECK( OneL < MaxL );
    BOOST_CHECK( R1L < MaxL );
    BOOST_CHECK( R2L < MaxL );

    uint160 LastS;
    for (int i = 159; i >= 0; --i) {
        uint160 TmpS;
        *(TmpS.begin() + (i>>3)) |= 1<<(7-(i&7));
        BOOST_CHECK( LastS < TmpS );
        LastS = TmpS;
    }
<<<<<<< HEAD
    TmpS = R1S;
    BOOST_CHECK(--TmpS == R1S-1);

}

BOOST_AUTO_TEST_CASE( multiply )
{
    BOOST_CHECK((R1L * R1L).ToString() == "62a38c0486f01e45879d7910a7761bf30d5237e9873f9bff3642a732c4d84f10");
    BOOST_CHECK((R1L * R2L).ToString() == "de37805e9986996cfba76ff6ba51c008df851987d9dd323f0e5de07760529c40");
    BOOST_CHECK((R1L * ZeroL) == ZeroL);
    BOOST_CHECK((R1L * OneL) == R1L);
    BOOST_CHECK((R1L * MaxL) == -R1L);
    BOOST_CHECK((R2L * R1L) == (R1L * R2L));
    BOOST_CHECK((R2L * R2L).ToString() == "ac8c010096767d3cae5005dec28bb2b45a1d85ab7996ccd3e102a650f74ff100");
    BOOST_CHECK((R2L * ZeroL) == ZeroL);
    BOOST_CHECK((R2L * OneL) == R2L);
    BOOST_CHECK((R2L * MaxL) == -R2L);

    BOOST_CHECK((R1S * R1S).ToString() == "a7761bf30d5237e9873f9bff3642a732c4d84f10");
    BOOST_CHECK((R1S * R2S).ToString() == "ba51c008df851987d9dd323f0e5de07760529c40");
    BOOST_CHECK((R1S * ZeroS) == ZeroS);
    BOOST_CHECK((R1S * OneS) == R1S);
    BOOST_CHECK((R1S * MaxS) == -R1S);
    BOOST_CHECK((R2S * R1S) == (R1S * R2S));
    BOOST_CHECK((R2S * R2S).ToString() == "c28bb2b45a1d85ab7996ccd3e102a650f74ff100");
    BOOST_CHECK((R2S * ZeroS) == ZeroS);
    BOOST_CHECK((R2S * OneS) == R2S);
    BOOST_CHECK((R2S * MaxS) == -R2S);

    BOOST_CHECK(MaxL * MaxL == OneL);
    BOOST_CHECK(MaxS * MaxS == OneS);

    BOOST_CHECK((R1L * 0) == 0);
    BOOST_CHECK((R1L * 1) == R1L);
    BOOST_CHECK((R1L * 3).ToString() == "7759b1c0ed14047f961ad09b20ff83687876a0181a367b813634046f91def7d4");
    BOOST_CHECK((R2L * 0x87654321UL).ToString() == "23f7816e30c4ae2017257b7a0fa64d60402f5234d46e746b61c960d09a26d070");
    BOOST_CHECK((R1S * 0) == 0);
    BOOST_CHECK((R1S * 1) == R1S);
    BOOST_CHECK((R1S * 7).ToString() == "f7a987f3c3bf758d927f202d7e795faeff084244");
    BOOST_CHECK((R2S * 0xFFFFFFFFUL).ToString() == "1c6f6c930353e17f7d6127213bb18d2883e2cd90");
}

BOOST_AUTO_TEST_CASE( divide )
{
    uint256 D1L("AD7133AC1977FA2B7");
    uint256 D2L("ECD751716");
    BOOST_CHECK((R1L / D1L).ToString() == "00000000000000000b8ac01106981635d9ed112290f8895545a7654dde28fb3a");
    BOOST_CHECK((R1L / D2L).ToString() == "000000000873ce8efec5b67150bad3aa8c5fcb70e947586153bf2cec7c37c57a");
    BOOST_CHECK(R1L / OneL == R1L);
    BOOST_CHECK(R1L / MaxL == ZeroL);
    BOOST_CHECK(MaxL / R1L == 2);
    BOOST_CHECK_THROW(R1L / ZeroL, uint_error);
    BOOST_CHECK((R2L / D1L).ToString() == "000000000000000013e1665895a1cc981de6d93670105a6b3ec3b73141b3a3c5");
    BOOST_CHECK((R2L / D2L).ToString() == "000000000e8f0abe753bb0afe2e9437ee85d280be60882cf0bd1aaf7fa3cc2c4");
    BOOST_CHECK(R2L / OneL == R2L);
    BOOST_CHECK(R2L / MaxL == ZeroL);
    BOOST_CHECK(MaxL / R2L == 1);
    BOOST_CHECK_THROW(R2L / ZeroL, uint_error);

    uint160 D1S("D3C5EDCDEA54EB92679F0A4B4");
    uint160 D2S("13037");
    BOOST_CHECK((R1S / D1S).ToString() == "0000000000000000000000000db9af3beade6c02");
    BOOST_CHECK((R1S / D2S).ToString() == "000098dfb6cc40ca592bf74366794f298ada205c");
    BOOST_CHECK(R1S / OneS == R1S);
    BOOST_CHECK(R1S / MaxS == ZeroS);
    BOOST_CHECK(MaxS / R1S == 1);
    BOOST_CHECK_THROW(R1S / ZeroS, uint_error);
    BOOST_CHECK((R2S / D1S).ToString() == "0000000000000000000000000c5608e781182047");
    BOOST_CHECK((R2S / D2S).ToString() == "00008966751b7187c3c67c1fda5cea7db2c1c069");
    BOOST_CHECK(R2S / OneS == R2S);
    BOOST_CHECK(R2S / MaxS == ZeroS);
    BOOST_CHECK(MaxS / R2S == 1);
    BOOST_CHECK_THROW(R2S / ZeroS, uint_error);
}


bool almostEqual(double d1, double d2) 
{
    return fabs(d1-d2) <= 4*fabs(d1)*std::numeric_limits<double>::epsilon();
=======
    BOOST_CHECK( ZeroS < R1S );
    BOOST_CHECK( R2S < R1S );
    BOOST_CHECK( ZeroS < OneS );
    BOOST_CHECK( OneS < MaxS );
    BOOST_CHECK( R1S < MaxS );
    BOOST_CHECK( R2S < MaxS );
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
}

BOOST_AUTO_TEST_CASE( methods ) // GetHex SetHex begin() end() size() GetLow64 GetSerializeSize, Serialize, Unserialize
{
    BOOST_CHECK(R1L.GetHex() == R1L.ToString());
    BOOST_CHECK(R2L.GetHex() == R2L.ToString());
    BOOST_CHECK(OneL.GetHex() == OneL.ToString());
    BOOST_CHECK(MaxL.GetHex() == MaxL.ToString());
    uint256 TmpL(R1L);
    BOOST_CHECK(TmpL == R1L);
    TmpL.SetHex(R2L.ToString());   BOOST_CHECK(TmpL == R2L);
    TmpL.SetHex(ZeroL.ToString()); BOOST_CHECK(TmpL == uint256());

    TmpL.SetHex(R1L.ToString());
    BOOST_CHECK(memcmp(R1L.begin(), R1Array, 32)==0);
    BOOST_CHECK(memcmp(TmpL.begin(), R1Array, 32)==0);
    BOOST_CHECK(memcmp(R2L.begin(), R2Array, 32)==0);
    BOOST_CHECK(memcmp(ZeroL.begin(), ZeroArray, 32)==0);
    BOOST_CHECK(memcmp(OneL.begin(), OneArray, 32)==0);
    BOOST_CHECK(R1L.size() == sizeof(R1L));
    BOOST_CHECK(sizeof(R1L) == 32);
    BOOST_CHECK(R1L.size() == 32);
    BOOST_CHECK(R2L.size() == 32);
    BOOST_CHECK(ZeroL.size() == 32);
    BOOST_CHECK(MaxL.size() == 32);
    BOOST_CHECK(R1L.begin() + 32 == R1L.end());
    BOOST_CHECK(R2L.begin() + 32 == R2L.end());
    BOOST_CHECK(OneL.begin() + 32 == OneL.end());
    BOOST_CHECK(MaxL.begin() + 32 == MaxL.end());
    BOOST_CHECK(TmpL.begin() + 32 == TmpL.end());
    BOOST_CHECK(R1L.GetSerializeSize(0,PROTOCOL_VERSION) == 32);
    BOOST_CHECK(ZeroL.GetSerializeSize(0,PROTOCOL_VERSION) == 32);

    std::stringstream ss;
    R1L.Serialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ss.str() == std::string(R1Array,R1Array+32));
    TmpL.Unserialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(R1L == TmpL);
    ss.str("");
    ZeroL.Serialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ss.str() == std::string(ZeroArray,ZeroArray+32));
    TmpL.Unserialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ZeroL == TmpL);
    ss.str("");
    MaxL.Serialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ss.str() == std::string(MaxArray,MaxArray+32));
    TmpL.Unserialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(MaxL == TmpL);
    ss.str("");

    BOOST_CHECK(R1S.GetHex() == R1S.ToString());
    BOOST_CHECK(R2S.GetHex() == R2S.ToString());
    BOOST_CHECK(OneS.GetHex() == OneS.ToString());
    BOOST_CHECK(MaxS.GetHex() == MaxS.ToString());
    uint160 TmpS(R1S);
    BOOST_CHECK(TmpS == R1S);
    TmpS.SetHex(R2S.ToString());   BOOST_CHECK(TmpS == R2S);
    TmpS.SetHex(ZeroS.ToString()); BOOST_CHECK(TmpS == uint160());

    TmpS.SetHex(R1S.ToString());
    BOOST_CHECK(memcmp(R1S.begin(), R1Array, 20)==0);
    BOOST_CHECK(memcmp(TmpS.begin(), R1Array, 20)==0);
    BOOST_CHECK(memcmp(R2S.begin(), R2Array, 20)==0);
    BOOST_CHECK(memcmp(ZeroS.begin(), ZeroArray, 20)==0);
    BOOST_CHECK(memcmp(OneS.begin(), OneArray, 20)==0);
    BOOST_CHECK(R1S.size() == sizeof(R1S));
    BOOST_CHECK(sizeof(R1S) == 20);
    BOOST_CHECK(R1S.size() == 20);
    BOOST_CHECK(R2S.size() == 20);
    BOOST_CHECK(ZeroS.size() == 20);
    BOOST_CHECK(MaxS.size() == 20);
    BOOST_CHECK(R1S.begin() + 20 == R1S.end());
    BOOST_CHECK(R2S.begin() + 20 == R2S.end());
    BOOST_CHECK(OneS.begin() + 20 == OneS.end());
    BOOST_CHECK(MaxS.begin() + 20 == MaxS.end());
    BOOST_CHECK(TmpS.begin() + 20 == TmpS.end());
    BOOST_CHECK(R1S.GetSerializeSize(0,PROTOCOL_VERSION) == 20);
    BOOST_CHECK(ZeroS.GetSerializeSize(0,PROTOCOL_VERSION) == 20);

    R1S.Serialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ss.str() == std::string(R1Array,R1Array+20));
    TmpS.Unserialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(R1S == TmpS);
    ss.str("");
    ZeroS.Serialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ss.str() == std::string(ZeroArray,ZeroArray+20));
    TmpS.Unserialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ZeroS == TmpS);
    ss.str("");
    MaxS.Serialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(ss.str() == std::string(MaxArray,MaxArray+20));
    TmpS.Unserialize(ss,0,PROTOCOL_VERSION);
    BOOST_CHECK(MaxS == TmpS);
    ss.str("");
}

BOOST_AUTO_TEST_CASE( conversion )
{
    BOOST_CHECK(ArithToUint256(UintToArith256(ZeroL)) == ZeroL);
    BOOST_CHECK(ArithToUint256(UintToArith256(OneL)) == OneL);
    BOOST_CHECK(ArithToUint256(UintToArith256(R1L)) == R1L);
    BOOST_CHECK(ArithToUint256(UintToArith256(R2L)) == R2L);
    BOOST_CHECK(UintToArith256(ZeroL) == 0);
    BOOST_CHECK(UintToArith256(OneL) == 1);
    BOOST_CHECK(ArithToUint256(0) == ZeroL);
    BOOST_CHECK(ArithToUint256(1) == OneL);
    BOOST_CHECK(arith_uint256(R1L.GetHex()) == UintToArith256(R1L));
    BOOST_CHECK(arith_uint256(R2L.GetHex()) == UintToArith256(R2L));
    BOOST_CHECK(R1L.GetHex() == UintToArith256(R1L).GetHex());
    BOOST_CHECK(R2L.GetHex() == UintToArith256(R2L).GetHex());
}

BOOST_AUTO_TEST_SUITE_END()
