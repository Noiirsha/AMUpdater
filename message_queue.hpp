#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#define MAX_SIZE 10

class MessageQueue {
private:
    std::string messages[MAX_SIZE];
    int currentSize;
    int oldestIndex;

public:
    MessageQueue() : currentSize(0), oldestIndex(0) {}

    void push(const std::string& message) {
        if (currentSize < MAX_SIZE) {
            messages[(oldestIndex + currentSize) % MAX_SIZE] = message;
            currentSize++;
        }
        else {
            messages[oldestIndex] = message;
            oldestIndex = (oldestIndex + 1) % MAX_SIZE;
        }
    }

    std::vector<std::string> getAllMessagesAsVector() const {
        std::vector<std::string> result;
        result.reserve(currentSize);

        for (int i = 0; i < currentSize; ++i) {
            int index = (oldestIndex + i) % MAX_SIZE;
            result.push_back(messages[index]);
        }

        return result;
    }

    void overwriteLatest(const std::string& newContent) {
        if (currentSize == 0) {
            push(newContent);  // 队列为空，直接添加
        }
        else {
            // 计算最新消息的位置
            int latestIndex = (oldestIndex + currentSize - 1) % MAX_SIZE;
            messages[latestIndex] = newContent;
        }
    }

    int size() const {
        return currentSize;
    }

};

MessageQueue content_strings;