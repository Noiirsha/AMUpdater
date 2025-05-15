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

    int size() const {
        return currentSize;
    }
};