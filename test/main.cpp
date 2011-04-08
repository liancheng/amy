#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>

DEFINE_string(workdir, "", "Set the working directory.");

int main(int argc, char* argv[]) try {
    using namespace boost::filesystem;

    testing::InitGoogleTest(&argc, argv);

    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    if (FLAGS_workdir.length()) {
        current_path(path(FLAGS_workdir));
    }

    return RUN_ALL_TESTS();
}
catch (boost::exception& e) {
    std::cout << "EXCEPTION: " << diagnostic_information(e) << std::endl;
    throw;
}
catch (std::exception& e) {
    std::cout << "EXCEPTION: " << e.what() << std::endl;
    throw;
}
