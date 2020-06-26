#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <ctime>

class Random {
    private:
        Random();
        std::mt19937 myRange;
    public:
        static Random& get();
        Random(const Random& other) = delete;
        Random& operator=(const Random& other) = delete;
        int intInRange(int low, int high);
        float floatInRange(float low, float high);   
};

Random::Random() {
    myRange.seed((unsigned)std::time(nullptr));
}

Random& Random::get() {
    static Random r;
    return r;
}

int Random::intInRange(int low, int high) {
    std::uniform_int_distribution<int> dist(low, high);
    return dist(myRange);
}

float Random::floatInRange(float low, float high) {
    std::uniform_real_distribution<float> dist(low, high);
    return dist(myRange);
}

#endif // RANDOM_H