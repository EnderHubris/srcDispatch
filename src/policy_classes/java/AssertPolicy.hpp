#ifndef INCLUDED_ASSERT_POLICY_HPP
#define INCLUDED_ASSERT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <ExprTypePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct AssertData : public ElementData {

        DeltaElement<std::shared_ptr<ExpressionData>> expr;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return expr.ToString(operation);
        }
    };

    class AssertPolicy : public ExprTypePolicy<AssertData, srcDispatch::ParserState::assert> {
    public:
        AssertPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ExprTypePolicy<AssertData, srcDispatch::ParserState::assert>(listeners) {
        }

    };

}

#endif