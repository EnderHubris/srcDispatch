#define BOOST_TEST_MODULE call tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <csharp/UsingStmtPolicy.hpp>
#include <csharp/LockStmtPolicy.hpp>
#include <csharp/FixedStmtPolicy.hpp>

#include <java/SyncStmtPolicy.hpp>

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

    const srcDispatch::UsingData& data = *std::any_cast<std::shared_ptr<srcDispatch::UsingData>>(
        runner.GetClassInfo().at(0)->methods.at(0)->block->statements.at(0).GetElement()
    );
    BOOST_TEST(data.init->inits.size() == 1);
    BOOST_TEST(data.block->statements.size() == 1);
}

BOOST_AUTO_TEST_CASE(lock_stmt_common) {

    srcDispatch::DispatchRunner runner("C#");
    std::string src = R"(
    class Program {
        static object mutex = new object();
        static void Main() {
            lock (mutex) {
                int abc = 10;
            }
        }
    }
    )";
    runner.RunDispatcher({{src,src}});

    BOOST_TEST(runner.GetClassInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 1, "[!] Missing Statements!");

    const srcDispatch::LockData& data = *std::any_cast<std::shared_ptr<srcDispatch::LockData>>(
        runner.GetClassInfo().at(0)->methods.at(0)->block->statements.at(0).GetElement()
    );
    BOOST_TEST(data.init->inits.size() == 1);
    BOOST_TEST(data.block->statements.size() == 1, "[!] Missing Statements!");
}

BOOST_AUTO_TEST_CASE(fixed_stmt_common) {

    srcDispatch::DispatchRunner runner("C#");
    std::string src = R"(
    class Program {
        static void Main() {
            fixed (int* p = numbers) {
                *ptr = 10;
            }
        }
    }
    )";
    runner.RunDispatcher({{src,src}});

    BOOST_TEST(runner.GetClassInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 1, "[!] Missing Statements!");

    const srcDispatch::FixedData& data = *std::any_cast<std::shared_ptr<srcDispatch::FixedData>>(
        runner.GetClassInfo().at(0)->methods.at(0)->block->statements.at(0).GetElement()
    );
    BOOST_TEST(data.init->inits.size() == 1);
    BOOST_TEST(data.block->statements.size() == 1, "[!] Missing Statements!");
}

BOOST_AUTO_TEST_CASE(sync_stmt_common) {

    srcDispatch::DispatchRunner runner("Java");
    std::string src = R"(
    class Program {
        static void Main() {
            synchronized (item) {
                item = 10;
            }
        }
    }
    )";
    runner.RunDispatcher({{src,src}});

    BOOST_TEST(runner.GetClassInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 1, "[!] Missing Statements!");
}