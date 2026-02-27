#ifndef INCLUDED_LAMBDA_POLICY_HPP
#define INCLUDED_LAMBDA_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <CapturePolicy.hpp>
#include <TypePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

/**
 * 
 * <lambda>
 *     <capture> ... </capture>------------------(C++ ONLY)
 *     <parameter_list> ... </parameter_list>
 *     <type> <name> ... </name> </type>---------(C++ ONLY)
 *     <block> ... </block>
 * </lambda>
 * 
 */

namespace srcDispatch {

    struct BlockData;
    class BlockPolicy;

    struct DeclData;
    class DeclPolicy;

    struct LambdaData : public ElementData {

        DeltaElement<std::shared_ptr<CaptureData>> capture;
        DeltaElement<std::shared_ptr<TypeData>> returnType;
        std::vector<DeltaElement<std::shared_ptr<DeclData>>> parameters;
        DeltaElement<std::shared_ptr<BlockData>> block;
    };

    class LambdaPolicy : 
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {
        
    private:
        LambdaData data;

        std::unique_ptr<CapturePolicy>   capturePolicy;
        std::unique_ptr<TypePolicy>      typePolicy;
        std::unique_ptr<DeclPolicy>      declPolicy;
        std::unique_ptr<BlockPolicy>     blockPolicy;

    public:
        LambdaPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
        ~LambdaPolicy();

    protected:
        std::any DataInner() const override;
        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx);
        void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx);
    
    private:
        void InitializeLambdaPolicy();
        void CollectCaptureHandlers();
        void CollectTypeHandlers();
        void CollectParameterHandlers();
        void CollectBlockHandlers();

    };

};

#endif