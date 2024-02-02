#include "Utils.hpp"

namespace engine
{
    long long Utils::getTimestampNS() {
        return std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }

}
