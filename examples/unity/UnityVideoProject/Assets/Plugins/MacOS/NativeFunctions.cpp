#define EXPORT __attribute__((visibility("default")))

#include <memory>

class someBridge;
typedef std::shared_ptr<someBridge> someBridgePrt;


class someBridge {
public:
    static someBridgePrt instance_;
    static someBridgePrt getBridge(){
        return instance_;
    }
    someBridge(){
        
    }
    
    uint32_t* getInput(){
        if(inputArray_){
            return inputArray_.get();
        }
        return nullptr;
    }
    
    void initInputBuffer(uint32_t size){
        inputArraySize_ = size;
        inputArray_ = std::unique_ptr<uint32_t>(new uint32_t[size]);

        for(int i = 0 ; i < inputArraySize_ ; i++)
        {
            inputArray_.get()[i] = i * 2;
        }
    }

    void copyInputArray(uint32_t *outArray)
    {
        memcpy(outArray, someBridge::getBridge()->getInput(), inputArraySize_ * sizeof(uint32_t));
    }

    void setInputBufferData(uint32_t *bufferData)
    {
        memcpy(someBridge::getBridge()->getInput(), bufferData, inputArraySize_ * sizeof(uint32_t));
    }
    
private:
    std::unique_ptr<uint32_t> inputArray_;
    uint32_t inputArraySize_;
    
};

someBridgePrt someBridge::instance_ = std::make_shared<someBridge>();

extern "C"{

    void __stdcall initInputBufferCS(uint32_t size){
        someBridge::getBridge()->initInputBuffer(size);
    }

    void __stdcall getInputBufferCS(uint32_t* outBuffer){
        someBridge::getBridge()->copyInputArray(outBuffer);
    }

    void __stdcall setInputBufferDataCS(uint32_t* bufferData){
        someBridge::getBridge()->setInputBufferData(bufferData);
    }

    EXPORT uint32_t* getInputBufferCpp(){
        return someBridge::getBridge()->getInput();
    }
}