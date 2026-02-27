#ifndef INCLUDED_CAPTURE_POLICY_HPP
#define INCLUDED_CAPTURE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <NamePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

/**
 * 
 * <capture>
 *     <argument><name> ... </name></argument>
 * </capture>
 * 
 */

namespace srcDispatch {

    struct CaptureData : public ElementData {

        std::vector<DeltaElement<std::shared_ptr<NameData>>> names;
    };

    class CapturePolicy : 
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {
        
    private:
        CaptureData data;

        std::unique_ptr<NamePolicy>    namePolicy;

    public:
        CapturePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeCapturePolicy();
        }
        ~CapturePolicy(){}

    protected:
        std::any DataInner() const override { return std::make_shared<CaptureData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
            using namespace srcDispatch;

            if(typeid(NamePolicy) == typeid(*policy)) {
                data.names.emplace_back(ctx.diffStack.back().operation, policy->Data<NameData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {}
    
    private:
        void InitializeCapturePolicy() {
            using namespace srcDispatch;

            openEventMap[ParserState::capture] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = CaptureData{};
                data.startPosition = ctx.startPosition;
                data.endPosition = ctx.endPosition;
                CollectNameHandlers();
            };

            // end of policy
            closeEventMap[ParserState::capture] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return ;

                depth = 0;
                NotifyAll(ctx);
                InitializeCapturePolicy();
            };
        }

        void CollectNameHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
                if(!depth) return; 

                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
        }
    };

};

#endif