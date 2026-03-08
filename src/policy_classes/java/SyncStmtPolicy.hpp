#ifndef INCLUDED_SYNC_STATEMENT_POLICY_HPP
#define INCLUDED_SYNC_STATEMENT_POLICY_HPP

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
 * <synchronized>
 *     <init> ... </init>
 *     <block> ... </block>
 * </synchronized>
 * 
 */

namespace srcDispatch {

    struct SyncData : public StatementData {
    };

    class SyncStmtPolicy : public StatementPolicy<SyncData, srcDispatch::ParserState::synchronized_stmt> {
    public:
        SyncStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : StatementPolicy<SyncData, srcDispatch::ParserState::synchronized_stmt>(listeners) {}
    };

};

#endif