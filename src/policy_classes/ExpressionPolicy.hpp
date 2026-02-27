// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ExpressionPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_EXPRESSION_POLICY_HPP
#define INCLUDED_EXPRESSION_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>
#include <CallPolicy.hpp>
#include <NamePolicy.hpp>
#include <LambdaPolicy.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

namespace srcDispatch {

    struct CallData;
    class CallPolicy;

    struct NameData;
    class NamePolicy;

    struct LambdaData;
    class LambdaPolicy;

    // A vector of elements in the expression.
    // Names, operators, calls in the correct order.
    // Need for determining variable use, variable modification, calls

    struct ExpressionData : public ElementData {

        std::vector<DeltaElement<std::any>> expr;

        std::shared_ptr<ExpressionData> copyAs(srcDispatch::DiffOperation operation) const;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const;
    };

    class ExpressionPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ExpressionData data;

        std::unique_ptr<NamePolicy> namePolicy;
        std::unique_ptr<OperatorPolicy> operatorPolicy;
        std::unique_ptr<LiteralPolicy> literalPolicy;
        std::unique_ptr<CallPolicy> callPolicy;
        std::unique_ptr<LambdaPolicy> lambdaPolicy;

    public:
        ExpressionPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
        ~ExpressionPolicy();

    protected:
        std::any DataInner() const override { return std::make_shared<ExpressionData>(data); }
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeExpressionPolicyHandlers();
        void CollectCallHandlers();
        void CollectNameHandlers();
        void CollectOperatorHandlers();
        void CollectLiteralHandlers();
        void CollectLambdaHandlers();
    };

}

#endif
