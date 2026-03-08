// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testFor.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE for_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <ForPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// // for and control
BOOST_AUTO_TEST_CASE(block_common_for) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(; 1; ) {} }", "void foo() { for(; 1; ) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 0);
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "1");
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "; 1; ");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_for) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { for(; 1; ) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsInsert());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsInsert());
    BOOST_TEST(forData.control->init->inits.size() == 0);
    BOOST_TEST(forData.control->condition.IsInsert());
    BOOST_TEST(forData.control->condition.ToString() == "|1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsInsert());
    BOOST_TEST(forData.control->incr->exprs.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "|; 1; ");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_for) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(; 1; ) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsDelete());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsDelete());
    BOOST_TEST(forData.control->init->inits.size() == 0);
    BOOST_TEST(forData.control->condition.IsDelete());
    BOOST_TEST(forData.control->condition.ToString() == "1|");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsDelete());
    BOOST_TEST(forData.control->incr->exprs.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "; 1; |");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(foreach_common) {

    srcDispatch::DispatchRunner runner("C#");
    std::string src = R"(
    class Program {
        static void Main() {
            foreach (var item in items) {
                Console.WriteLine(item);
            }
        }
    }
    )";
    runner.RunDispatcher({{src,src}});

    BOOST_TEST(runner.GetClassInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 1, "[!] Missing Statements!");

    const srcDispatch::ForData& data = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetClassInfo().at(0)->methods.at(0)->block->statements.at(0).GetElement()
    );
    BOOST_TEST(data.control->init->inits.size() == 1);
    
    {
        const srcDispatch::DeclData& decl = *std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
            data.control->init->inits.at(0).GetElement()
        );
        BOOST_TEST(decl.range->expr.size() == 1);

        const srcDispatch::NameData& nameData = *std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
            decl.range->expr.at(0).GetElement()
        );
        BOOST_TEST(nameData.name.ToString() == "items");
    }
    BOOST_TEST(data.block->statements.size() == 1, "[!] Missing Statements!");
}