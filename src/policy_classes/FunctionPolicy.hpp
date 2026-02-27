// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file FunctionPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_FUNCTION_POLICY_HPP
#define INCLUDED_FUNCTION_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <Access.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <GenericPolicy.hpp>
#include <NamePolicy.hpp>
#include <TypePolicy.hpp>
#include <CallPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

namespace srcDispatch {

    struct FunctionData : public ElementData {

        enum FunctionType { CONSTRUCTOR, DESTRUCTOR, OPERATOR, FUNCTION };

        /*** @todo fix */
        // std::vector<DeltaElement<std::string>> namespaces;
        std::vector<std::string> namespaces;

        std::string language;
        std::string filename;

        std::vector<DeltaElement<std::shared_ptr<GenericData>>> generics;
        DeltaElement<FunctionType>                              type;
        DeltaElement<std::shared_ptr<Access>>                   accessSpecifier;

        std::vector<DeltaElement<std::shared_ptr<std::string>>> leadingSpecifiers;
        std::vector<DeltaElement<std::shared_ptr<std::string>>> trailingSpecifiers;

        DeltaElement<bool> isDecl;
        DeltaElement<bool> isPureVirtual;
        DeltaElement<bool> isDelete;

        /*** @todo implement */
        std::set<std::string> stereotypes;

        DeltaElement<std::shared_ptr<TypeData>> returnType;
        DeltaElement<std::shared_ptr<NameData>> name;
        std::vector<DeltaElement<std::shared_ptr<DeclData>>> parameters;
        std::vector<DeltaElement<std::shared_ptr<CallData>>> memberInitList;
        DeltaElement<std::shared_ptr<BlockData>> block;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const;
    };

    class FunctionPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        FunctionData data;

        bool beforeParameters;

        std::unique_ptr<GenericPolicy> genericPolicy;
        std::unique_ptr<TypePolicy>    typePolicy;
        std::unique_ptr<NamePolicy>    namePolicy;
        std::unique_ptr<DeclPolicy>    declPolicy;
        std::unique_ptr<CallPolicy>    callPolicy;
        std::unique_ptr<BlockPolicy>   blockPolicy;

    public:
        FunctionPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
        ~FunctionPolicy();

    protected:
        std::any DataInner() const override;
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override;

    private:
        void InitializeFunctionPolicyHandlers();
        void CollectXMLAttributeHandlers();
        void CollectGenericHandlers();
        void CollectTypeHandlers();
        void CollectNameHandlers();
        void CollectParameterHandlers();
        void CollectCallHandlers();
        void CollectOtherHandlers();
        void CollectBlockHandlers();
    };

}

#endif
