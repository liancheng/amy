#include <gtest/gtest.h>

int main(int argc, char* argv[]) try {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} catch (std::exception& e) {
    std::cout << "EXCEPTION: " << e.what() << std::endl;
    throw;
}
