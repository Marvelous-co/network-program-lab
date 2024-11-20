#include "util.h"

int convertNum(std::string str) {
    int n = str.size();
    int num = 0;
    for (int i = 0; i < n; i++) {
        if (!(str[i] >= '0' && str[i] <= '9')) {
            return -1;
        }
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

bool isValidIPv4(const std::string ip) {
    std::stringstream ss(ip);
    std::string segment;
    int count = 0;

    while (std::getline(ss, segment, '.')) { // 按 '.' 分割字符串
        count++;
        if (segment.empty() || segment.size() > 3) return false;

        // 检查是否是数字
        for (char c : segment) {
            if (!isdigit(c)) return false;
        }

        // 转换为整数并检查范围
        int num = std::stoi(segment);
        if (num < 0 || num > 255) return false;

        // 检查是否有前导零
        if (segment[0] == '0' && segment.size() > 1) return false;
    }

    return count == 4; // 必须是 4 段
}

bool isValidIPAndPort(const std::string ipStr, const std::string portStr) {
    int port = convertNum(portStr);
    if (port < 0 || port > 65535)
        return false;
    return isValidIPv4(ipStr);
}



