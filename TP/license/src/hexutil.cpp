#include "hexutil.h"

char intToHexChar(int x) {
    static const char HEX[16] = {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };
    return HEX[x];
}
int hexCharToInt(char hex) {
    hex = toupper(hex);
    if (isdigit(hex))
        return (hex - '0');
    if (isalpha(hex))
        return (hex - 'A' + 10);
    return 0;
}

string bytesToHexString(const byte *in, size_t size) {
    string str;
    for (size_t i = 0; i < size; ++i) {
        int t = in[i];
        int a = t / 16;
        int b = t % 16;
        str.append(1, intToHexChar(a));
        str.append(1, intToHexChar(b));
    }
    return str;
}

vector<byte> hexStringToBytes(const string &str) {

    vector<string> vec;
    vector<byte>output;
    string::size_type currPos = 0, prevPos = 0;
    while ((currPos = str.find(' ', prevPos)) != string::npos) {
        string b(str.substr(prevPos, currPos - prevPos));
        vec.push_back(b);
        prevPos = currPos + 1;
    }
    if (prevPos < str.size()) {
        string b(str.substr(prevPos));
        vec.push_back(b);
    }
    typedef vector<string>::size_type sz_type;
    sz_type size = vec.size();
    for (sz_type i = 0; i < size; ++i) {
        int a = hexCharToInt(vec[i][0]);
        int b = hexCharToInt(vec[i][1]);
        output.push_back(a * 16 + b);

    }
    return output;
}
