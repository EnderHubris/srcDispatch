#ifndef INCLUDED_LOCK_STATEMENT_POLICY_HPP
#define INCLUDED_LOCK_STATEMENT_POLICY_HPP

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
 * <lock>
 *     <init> ... </init>
 *     <block> ... </block>
 * </lock>
 * 
 */

namespace srcDispatch {

    struct LockData : public StatementData {
    };

    class LockStmtPolicy : public StatementPolicy<LockData, srcDispatch::ParserState::lock> {
    public:
        LockStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : StatementPolicy<LockData, srcDispatch::ParserState::lock>(listeners) {}
    };

};

#endif