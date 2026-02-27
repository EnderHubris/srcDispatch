#include <LambdaPolicy.hpp>

#include <DeclPolicy.hpp>
#include <BlockPolicy.hpp>

namespace srcDispatch {

    LambdaPolicy::LambdaPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners), data{} {
        InitializeLambdaPolicy();
    }
    LambdaPolicy::~LambdaPolicy(){}

    std::any LambdaPolicy::DataInner() const { return std::make_shared<LambdaData>(data); }

    void LambdaPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        using namespace srcDispatch;

        if(typeid(CapturePolicy) == typeid(*policy)) {
            data.capture.Update(ctx.diffStack.back().operation, policy->Data<CaptureData>());
        } else if(typeid(TypePolicy) == typeid(*policy)) {
            data.returnType.Update(ctx.diffStack.back().operation, policy->Data<TypeData>());
        } else if(typeid(DeclPolicy) == typeid(*policy)) {
            data.parameters.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclData>());
        } else if(typeid(BlockPolicy) == typeid(*policy)) {
            data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void LambdaPolicy::NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {}
    
    void LambdaPolicy::InitializeLambdaPolicy() {
        using namespace srcDispatch;

        openEventMap[ParserState::lambda] = [this](srcSAXEventContext& ctx) {
            if(depth) return;

            depth = ctx.depth;
            data = LambdaData{};
            data.startPosition = ctx.startPosition;
            data.endPosition = ctx.endPosition;

            CollectCaptureHandlers();
            CollectTypeHandlers();
            CollectParameterHandlers();
            CollectBlockHandlers();
        };

        // end of policy
        closeEventMap[ParserState::lambda] = [this](srcSAXEventContext& ctx) {
            if(!depth || depth != ctx.depth) return ;

            depth = 0;
            NotifyAll(ctx);
            InitializeLambdaPolicy();
        };
    }

    void LambdaPolicy::CollectCaptureHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::capture] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!capturePolicy) {
                capturePolicy = make_unique_policy<CapturePolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(capturePolicy.get());
        };
    }

    void LambdaPolicy::CollectTypeHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            
            if(!typePolicy) {
                typePolicy = make_unique_policy<TypePolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(typePolicy.get()); 
        };
    }

    void LambdaPolicy::CollectParameterHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            
            openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
                if(!declPolicy) {
                    declPolicy = make_unique_policy<DeclPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(declPolicy.get());
            };
        };
        
        closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            NopOpenEvents({ParserState::parameter});
        };
    }

    void LambdaPolicy::CollectBlockHandlers() {
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