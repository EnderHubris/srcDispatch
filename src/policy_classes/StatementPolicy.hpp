#ifndef INCLUDED_STATEMENT_POLICY_HPP
#define INCLUDED_STATEMENT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <InitPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct StatementData : public ElementData {

        DeltaElement<std::shared_ptr<InitData>>  init;
        DeltaElement<std::shared_ptr<BlockData>> block;

        template<class type>
        friend class DeltaElement;
    };

    template <typename StatementDataParam, srcDispatch::ParserState DispatchEvent>
    class StatementPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    protected:
        StatementDataParam data;

        std::unique_ptr<InitPolicy>      initPolicy;
        std::unique_ptr<BlockPolicy>     blockPolicy;

    public:
        StatementPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeStatementPolicyHandlers();
        }

    protected:
        std::any DataInner() const override { return std::make_shared<StatementDataParam>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
            using namespace srcDispatch;

            if(typeid(InitPolicy) == typeid(*policy)) {
                data.init.Update(ctx.diffStack.back().operation, policy->Data<InitData>());
            } else if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

        void InitializeStatementPolicyHandlers() {

            using namespace srcDispatch;

            openEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = StatementDataParam{};
                data.startPosition = ctx.startPosition;
                data.endPosition = ctx.endPosition;
                CollectInitHandlers();
                CollectBlockHandlers();
            };

            // end of policy
            closeEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return ;

                depth = 0;
                NotifyAll(ctx);
                InitializeStatementPolicyHandlers();
            };
        }

        void CollectInitHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
                if(!depth) return; 

                if(!initPolicy) {
                    initPolicy = make_unique_policy<InitPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(initPolicy.get());
            };
        }

        void CollectBlockHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
                if(!depth) return; 

                if(!blockPolicy) {
                    blockPolicy = make_unique_policy<BlockPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(blockPolicy.get());
            };
        }
    };

}

#endif