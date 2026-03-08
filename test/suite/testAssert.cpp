#define BOOST_TEST_MODULE call tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <java/AssertPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(assert_common) {

    srcDispatch::DispatchRunner runner("Java");
    std::string src = R"(
    class Program {
        static void Main() {
            assert dog.speak() == "bark" : "Whoops!"
        }
    }
    )";
    runner.RunDispatcher({{src,src}});

    BOOST_TEST(runner.GetClassInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 1);
}