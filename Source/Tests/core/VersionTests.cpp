#include <BlueJay.h>

#include <cstdio>
#include <cstring>

namespace {

int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++g_failures;
    }
}

}  // namespace

int main() {
    // Header constants agree with each other.
    Check(bluejay::kVersionMajor == BLUEJAY_VERSION_MAJOR, "major matches macro");
    Check(bluejay::kVersionMinor == BLUEJAY_VERSION_MINOR, "minor matches macro");
    Check(bluejay::kVersionPatch == BLUEJAY_VERSION_PATCH, "patch matches macro");

    // The linked library agrees with the header it was built against.
    Check(std::strcmp(bluejay::VersionString(), BLUEJAY_VERSION_STRING) == 0,
          "library version string matches header");

    return g_failures == 0 ? 0 : 1;
}
