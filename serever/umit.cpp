//
// Created by meidai on 18-7-16.
//

#include "umit.h"
string umit::get_time()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%a, %d %b %Y %H:%M:%S GMT",localtime(&timep) );
    return tmp;
}
int  umit::hex2num(string s) {
    int count = s.length();
    int sum = 0;
    for (int i = count - 1; i >= 0; i--)//从十六进制个位开始，每位都转换成十进制
    {
        if (s[i] >= '0' && s[i] <= '9')//数字字符的转换
        {
            sum += (s[i] - 48) * pow(16, count - i - 1);
        } else if (s[i] >= 'A' && s[i] <= 'F')//字母字符的转换
        {
            sum += (s[i] - 55) * pow(16, count - i - 1);
        }
        else if (s[i] >= 'a' && s[i] <= 'f')//字母字符的转换
        {
            sum += (s[i] - 87) * pow(16, count - i - 1);
        }
    }
    return sum;
}