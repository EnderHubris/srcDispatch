#ifndef INCLUDED_USING_STATEMENT_POLICY_HPP
#define INCLUDED_USING_STATEMENT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <StatementPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

/**
 * 
 * <using_stmt>
 *     <init> ... </init>
 *     <block> ... </block>
 * </using_stmt>
 * 
 */

namespace srcDispatch {

    struct UsingData : public StatementData {
    };

    class UsingStmtPolicy : public StatementPolicy<UsingData, srcDispatch::ParserState::using_stmt> {
    public:
        UsingStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : StatementPolicy<UsingData, srcDispatch::ParserState::using_stmt>(listeners) {}
    };

};

#endif