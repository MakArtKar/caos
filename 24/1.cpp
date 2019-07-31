#include <mutex>
#include <thread>
#include <vector>

// In fact, const or static const doesn't mean constsness,
// as well as using macros, so only enum values here are allowed
enum { SIZE = 3, COUNT = 1000000 };

double result[SIZE];

std::mutex mutex;

void process(int a, int b, int c, int d) {
    for (int i = 0; i < COUNT; ++i) {
        std::lock_guard<std::mutex> lock(mutex);

        result[a] += b;
        result[c] -= d;
    }
}

int main() {
    std::vector<std::thread> ids;
    ids.reserve(SIZE);

    for (int j = 0; j < SIZE; ++j)
        ids.emplace_back(process,
                j, 80 + 20 * j,
                (j + 1) % SIZE, 90 + 20 * j);

    for (auto & id : ids)
        id.join();

    printf("%.10g %.10g %.10g\n", result[0], result[1], result[2]);
}