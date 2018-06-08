#include "uint128_t.h"
#include <math.h>

int hex_char_value(char c){
    if(c >= '0' && c <= '9')
        return c - '0';
    else if(c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    else if(c >= 'A' && c <= 'F')
        return (c - 'A' + 10);

    return 0;
}

uint128_t hex2decimal(std::string& szHex){
    uint128_t number = 0;
    if(szHex[0] == '0' && (szHex[1] == 'x' || szHex[1] == 'X')){
        szHex= szHex.substr(2);
    }
    size_t size = szHex.size();
    for(uint32_t i = 0; i < size; i++) {
        uint128_t tmp = 1;
        for(int j = size - i - 1; j > 0; j--){
            tmp *= 16;
        }
        uint128_t tmp1 = tmp * hex_char_value(szHex[i]);
        number += tmp1;
    }
    return number;
}

const uint64_t ETH_TO_GWEI = 1000000000;
const double GWEI_TO_ETH = 1000000000.0;

double Hexstrwei2Doubleth(std::string &hexstr){
    uint128_t number = hex2decimal(hexstr);
    uint64_t u64val = number / ETH_TO_GWEI;
    return (u64val / GWEI_TO_ETH);
}

std::string Doubleth2hexStrwei(double number){
    uint64_t u64val = static_cast<uint64_t>(number * ETH_TO_GWEI);
    uint128_t u128val = u64val * ETH_TO_GWEI;
    return u128val.str(16);
}

int main(){

//**************************************************************************
    std::string str1 = "0x2345678901234567890123";
    uint128_t numb1 = hex2decimal(str1);
    double dnumb1 = pow(10, 18);
    uint64_t numb11 = static_cast<uint64_t>(dnumb1);
    uint64_t numb111 = numb1 / numb11;
    uint64_t numb1111 = numb1 % numb11;
    double dnumb11 = numb111 + numb1111 / dnumb1;

    std::cout << str1 << "*** " << numb1 << " " << dnumb1 << " " << numb11 << " " << numb111 << " " << numb1111 << " "<< std::to_string(dnumb11) << "\n";
//*******************************************************************************

    std::string str2 = "0x1489759";
    uint128_t numb2 = hex2decimal(str2);
    double dnumb2 = pow(10, 8);
    uint64_t numb22 = static_cast<uint64_t>(dnumb2);
    uint64_t numb222 = numb2 / numb22;
    uint64_t numb2222 = numb2 % numb22;
    double dnumb22 = numb222 + numb2222 / dnumb2;

    std::cout << str2 << "*** " << numb2 << " " << dnumb2 << " " << numb22 << " " << numb222 << " " << numb2222 << " "<< std::to_string(dnumb22) << "\n";

//*********************************************************************************

    auto numb3 = static_cast<uint64_t>(pow(10, 18));
    uint64_t numb33 = static_cast<uint64_t>(3500000.0101 * 1000000000);
    if(numb3 >= 1000000000) {
        uint128_t numb333 = numb3 / 1000000000;
        uint128_t numb3333 = numb333 * numb33;
        std::string str3 = numb3333.str(16, 64);
        std::cout <<numb3 << " " << numb33 << " " << numb333 << " " << numb3333 << " " << str3 << "\n";

    }
//******************************************************************************************

    auto numb4 = static_cast<uint64_t>(pow(10, 9 - 8));
    uint64_t numb44 = static_cast<uint64_t>(3500000.0101 * 1000000000);
    uint128_t numb444 = numb44 / numb4;

    std::cout << numb4 << " " << numb44 << " " << numb444 << " "<<"\n";
//*****************************************************************************************


}