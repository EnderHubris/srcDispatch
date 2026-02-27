#define BOOST_TEST_MODULE call tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <UsingStmtPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(using_stmt_common) {

    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        R"(
        class Program {
            static void Main() {
                using (StreamWriter writer = new StreamWriter("example.txt")) {
                    writer.WriteLine("Hello, world!");
                }
                return;
            }
        }
        )",
        R"(
        class Program {
            static void Main() {
                using (StreamWriter writer = new StreamWriter("example.txt")) {
                    writer.WriteLine("Hello, world!");
                }
                return;
            }
        }
        )"
    }});

    BOOST_TEST(runner.GetClassInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 2);

    const srcDispatch::UsingData& usingData = *std::any_cast<std::shared_ptr<srcDispatch::UsingData>>(
        runner.GetClassInfo().at(0)->methods.at(0)->block->statements.at(0).GetElement()
    );
    BOOST_TEST(usingData.block->statements.size() == 1);
}