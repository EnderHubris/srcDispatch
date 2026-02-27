#define BOOST_TEST_MODULE call tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <LambdaPolicy.hpp>

/**
 * @details - Lambda Policy does not support implicit returns within pseudo-blocks
 *            i.e a lambda in C#/Java with pseudo-blocks will have its statement
 *            container of size 0
 */

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(lambda_common) {

    srcDispatch::DispatchRunner cpp_runner;
    cpp_runner.RunDispatcher({{
        R"(
        int main() {
            auto foo = [&abc](int def) -> int {
                return def + abc;
            };
            std::generate(apples.begin(), apples.end(), [] () {
                return 1;
            });
        }
        )",
        R"(
        int main() {
            auto foo = [&abc](int def) -> int {
                return def + abc;
            };
            std::generate(apples.begin(), apples.end(), [] () {
                return 1;
            });
        }
        )"
    }});

    BOOST_TEST(cpp_runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(cpp_runner.GetFunctionInfo().at(0)->name.ToString() == "main");
    BOOST_TEST(cpp_runner.GetFunctionInfo().at(0)->block->statements.size() == 2);

    {
        auto& stmts = cpp_runner.GetFunctionInfo().at(0)->block->statements;
    
        {
            // dig into the foo decl and extract the expr
            auto decl_stmt = std::any_cast<std::shared_ptr<srcDispatch::DeclStmtData>>(stmts.at(0).GetElement());
            BOOST_TEST(decl_stmt->decls.size() == 1);
            BOOST_TEST(decl_stmt->decls.at(0)->init->expr.size() == 1);
    
            auto lambdaData = std::any_cast<std::shared_ptr<srcDispatch::LambdaData>>(
                decl_stmt->decls.at(0)->init->expr.at(0).GetElement()
            );
            BOOST_TEST(lambdaData);
    
            BOOST_TEST(lambdaData->capture->names.size() == 1);
            BOOST_TEST(lambdaData->parameters.size() == 1);
            BOOST_TEST(lambdaData->returnType.ToString() == "int");
            BOOST_TEST(lambdaData->block->statements.size() == 1);
        }
    
        {
            // dig into the expr_stmt and extract lambda call-back
            auto expr_stmt = std::any_cast<std::shared_ptr<srcDispatch::ExprStmtData>>(stmts.at(1).GetElement());
            BOOST_TEST(expr_stmt->expr->expr.size() == 1);
    
            // extract call
            auto callData = std::any_cast<std::shared_ptr<srcDispatch::CallData>>(
                expr_stmt->expr->expr.at(0).GetElement()
            );
            BOOST_TEST(callData);
            BOOST_TEST(callData->arguments.size() == 3);
    
            auto lambdaData = std::any_cast<std::shared_ptr<srcDispatch::LambdaData>>(
                callData->arguments.at(2)->expr.at(0).GetElement()
            );
            BOOST_TEST(lambdaData);
    
            BOOST_TEST(lambdaData->capture->names.size() == 0);
            BOOST_TEST(lambdaData->parameters.size() == 0);
            BOOST_TEST(lambdaData->block->statements.size() == 1);
        }
    }

//-----------------------------------------------------------------------------

    srcDispatch::DispatchRunner csharp_runner("C#");
    csharp_runner.RunDispatcher({{
        R"(
        class Program {
            static void Main() {
                Func<int, int> add = (a) => { return a + 10; };         // lambda here
                numbers.Select(n => { return n * n; });   // lambda here
            }
        }
        )",
        R"(
        class Program {
            static void Main() {
                Func<int, int> add = (a) => { return a + 10; };         // lambda here
                numbers.Select(n => { return n * n; });   // lambda here
            }
        }
        )"
    }});

    BOOST_TEST(csharp_runner.GetClassInfo().size() == 1);
    BOOST_TEST(csharp_runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(csharp_runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(csharp_runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 2);

    {
        auto& stmts = csharp_runner.GetClassInfo().at(0)->methods.at(0)->block->statements;
    
        {
            // dig into the foo decl and extract the expr
            auto decl_stmt = std::any_cast<std::shared_ptr<srcDispatch::DeclStmtData>>(stmts.at(0).GetElement());
            BOOST_TEST(decl_stmt->decls.size() == 1);
            BOOST_TEST(decl_stmt->decls.at(0)->init->expr.size() == 1);
    
            auto lambdaData = std::any_cast<std::shared_ptr<srcDispatch::LambdaData>>(
                decl_stmt->decls.at(0)->init->expr.at(0).GetElement()
            );
            BOOST_TEST(lambdaData);
    
            BOOST_TEST(lambdaData->parameters.size() == 1);
            BOOST_TEST(lambdaData->block->statements.size() == 1);
        }
    
        {
            // dig into the expr_stmt and extract lambda call-back
            auto expr_stmt = std::any_cast<std::shared_ptr<srcDispatch::ExprStmtData>>(stmts.at(1).GetElement());
            BOOST_TEST(expr_stmt->expr->expr.size() == 1);
    
            // extract call
            auto callData = std::any_cast<std::shared_ptr<srcDispatch::CallData>>(
                expr_stmt->expr->expr.at(0).GetElement()
            );
            BOOST_TEST(callData);
            BOOST_TEST(callData->arguments.size() == 1);
    
            auto lambdaData = std::any_cast<std::shared_ptr<srcDispatch::LambdaData>>(
                callData->arguments.at(0)->expr.at(0).GetElement()
            );
            BOOST_TEST(lambdaData);
    
            BOOST_TEST(lambdaData->parameters.size() == 1);
            BOOST_TEST(lambdaData->block->statements.size() == 1);
        }
    }

//-----------------------------------------------------------------------------

    srcDispatch::DispatchRunner java_runner("Java");
    java_runner.RunDispatcher({{
        R"(
        public class Origami {
            public static void main(String[] args) {
                Function<String, Integer> stringLength = s -> { return s.length(); };
                square.compose(n -> { return n + 3; });
            }
        }
        )",
        R"(
        public class Origami {
            public static void main(String[] args) {
                Function<String, Integer> stringLength = s -> { return s.length(); };
                square.compose(n -> { return n + 3; });
            }
        }
        )"
    }});

    BOOST_TEST(java_runner.GetClassInfo().size() == 1);
    BOOST_TEST(java_runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(java_runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "main");
    BOOST_TEST(java_runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 2);

    {
        auto& stmts = java_runner.GetClassInfo().at(0)->methods.at(0)->block->statements;
    
        {
            // dig into the foo decl and extract the expr
            auto decl_stmt = std::any_cast<std::shared_ptr<srcDispatch::DeclStmtData>>(stmts.at(0).GetElement());
            BOOST_TEST(decl_stmt->decls.size() == 1);
            BOOST_TEST(decl_stmt->decls.at(0)->init->expr.size() == 1);
    
            auto lambdaData = std::any_cast<std::shared_ptr<srcDispatch::LambdaData>>(
                decl_stmt->decls.at(0)->init->expr.at(0).GetElement()
            );
            BOOST_TEST(lambdaData);
    
            BOOST_TEST(lambdaData->parameters.size() == 1);
            BOOST_TEST(lambdaData->block->statements.size() == 1);
        }
    
        {
            // dig into the expr_stmt and extract lambda call-back
            auto expr_stmt = std::any_cast<std::shared_ptr<srcDispatch::ExprStmtData>>(stmts.at(1).GetElement());
            BOOST_TEST(expr_stmt->expr->expr.size() == 1);
    
            // extract call
            auto callData = std::any_cast<std::shared_ptr<srcDispatch::CallData>>(
                expr_stmt->expr->expr.at(0).GetElement()
            );
            BOOST_TEST(callData);
            BOOST_TEST(callData->arguments.size() == 1);
    
            auto lambdaData = std::any_cast<std::shared_ptr<srcDispatch::LambdaData>>(
                callData->arguments.at(0)->expr.at(0).GetElement()
            );
            BOOST_TEST(lambdaData);
    
            BOOST_TEST(lambdaData->parameters.size() == 1);
            BOOST_TEST(lambdaData->block->statements.size() == 1);
        }
    }
}