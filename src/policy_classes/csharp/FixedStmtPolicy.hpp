#ifndef INCLUDED_FIXED_STATEMENT_POLICY_HPP
#define INCLUDED_FIXED_STATEMENT_POLICY_HPP

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
 * <fixed>
 *     <init> ... </init>
 *     <block> ... </block>
 * </fixed>
 * 
 */

namespace srcDispatch {

    struct FixedData : public StatementData {
    };

    class FixedStmtPolicy : public StatementPolicy<FixedData, srcDispatch::ParserState::fixed> {
    public:
        FixedStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : StatementPolicy<FixedData, srcDispatch::ParserState::fixed>(listeners) {}
    };

};

#endif