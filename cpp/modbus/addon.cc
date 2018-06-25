#include <node.h>
#include <v8.h>
#include <string>
#include <vector>
#include "mbrtu.h"

namespace mb {

  //Global vec for the result of send arr
  std::vector<std::string> vec;

  //Managed function to find the vector
  #pragma managed
  void callManaged(std::string arr[])
  {
    vec.clear();
    send_arr(arr, 12, vec);
  }


  //Arguments to use v9
  #pragma unmanaged
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using namespace v8;

  //Method sets returns the result to javascript
  void Method(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    //C++ vars
    int val [8];
    std::string arr[8];

    //Array that holds JS arguments
    val[0] = args[0]->NumberValue(); //ComPort
    val[1] = args[1]->NumberValue(); //Parity
    val[2] = args[2]->NumberValue(); //Speed
    val[3] = args[3]->NumberValue(); //Code
    val[4] = args[4]->NumberValue(); //Unit Address
    val[5] = args[5]->NumberValue(); // Start Register
    val[6] = args[6]->NumberValue(); // Number of Registers

    for(int i = 0; i < 7; i++){
      arr[i] = std::to_string(val[i]);
    }
    v8::String::Utf8Value param1(args[7]->ToString());
    arr[7] = std::string(*param1);

    //Call the managed function to receive result
    callManaged(arr);

    //Obtain a v8 array with vector as its argument
    v8::Handle<v8::Array> result = v8::Array::New(isolate, vec.size());
    for(int i = 0; i < vec.size(); i++){
      result->Set(i, v8::String::NewFromUtf8(isolate, vec[i].c_str()));
    }

    //Return result
    args.GetReturnValue().Set(result);
  }

  //initialize the function
  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "modbus", Method);
  }

  NODE_MODULE(addon, init)
}
