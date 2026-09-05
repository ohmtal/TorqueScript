//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// NeuralNetObject: Genann ElfScript Object
// TODO: add Constants for activation type
// NOTE: if you need better input/output handline add Array*
//-----------------------------------------------------------------------------
#include <SDL3/SDL.h>

#include "console/scriptPreprocessor.h"
#include "console/engineAPI.h"
#include "console/consoleExtras.h"
#include "math/mMathRand.h"

#include "ext/genann.h"
#include <math/mMathFn.h>

#define ANN_MAXINPUTS 100000
#define ANN_MAXOUTPUTS 100


#include "console/engineAPI.h"

class NeuralNetObject: public SimObject
{
    typedef SimObject Parent;
public:
    DECLARE_CONOBJECT(NeuralNetObject);
    Vector<F64> mInputs;
    Vector<F64> mOutputs;
    genann* mAnn = nullptr;

    ~NeuralNetObject() {
        if (mAnn) genann_free(mAnn);
    }
    // ------------------------------------------------------------------------
    inline bool resize(U32 inputs, U32 outputs) {
        if ( inputs > ANN_MAXINPUTS) return false;
        if ( outputs > ANN_MAXOUTPUTS) return false;
        mInputs.clear();
        mOutputs.clear();
        if (mInputs.setSize(inputs) != inputs) return false;
        if (mOutputs.setSize(outputs) != outputs) return false;
        return true;
    }
    // ------------------------------------------------------------------------
    inline genann_actfun getActivationFunctionByType(S32 type) {
        switch (type) {
            case 1:  return genann_act_tanh;
            case 2:  return genann_act_relu;
            case 3:  return genann_act_linear;
            case 0:
            default: return genann_act_sigmoid_cached;
        }
    }
    // ------------------------------------------------------------------------
    inline bool Init(S32 inputs, S32 hidden_layers, S32 hidden_neuros, S32 outputs) {

        if (inputs < 1 || inputs > ANN_MAXINPUTS || outputs < 1 || outputs > ANN_MAXOUTPUTS) {
            Con::errorf("NeuralNetObject - Sorry we support only max. %d inputs and max %d outputs.", ANN_MAXINPUTS, ANN_MAXOUTPUTS);
            return false;
        }

        if (hidden_layers < 1 || hidden_neuros < 1) {
            Con::errorf("NeuralNetObject - Hidden Layers and Hidden Neurons must be at least one.");
            return false;
        }
        if (!this->resize(inputs, outputs)) {
            Con::errorf("NeuralNetObject - Failed to set size of inputs and/or outputs.");
            return false;
        }
        mAnn = genann_init(inputs,  hidden_layers, hidden_neuros, outputs);
        if (!mAnn) return false;

        return true;
    }
    // ------------------------------------------------------------------------
    inline bool Save (const char*  filename) {
        if (!mAnn) {
            Con::errorf("NeuralNetObject save: not initialized");
            return false;
        }

        if (!filename || filename[0] == '\0') {
            Con::errorf("NeuralNetObject save: Invalid filename!");
            return false;
        }

        FILE *out = fopen(filename, "w");
        if (!out) {
            Con::errorf("NeuralNetObject save: failed to open file: %s", filename);
            return false;
        }

        genann_write(mAnn, out);

        if (ferror(out)) {
            Con::errorf("NeuralNetObject save failed to write to file: %s", filename);
            fclose(out);
            return false;
        }

        fclose(out);
        return true;
    }
    // ------------------------------------------------------------------------
    inline bool Load(const char* filename, S32 inputs = 0, S32 hidden_layers = 0, S32 hidden_neuros = 0, S32 outputs= 0) {
        if (!filename || filename[0] == '\0') {
            Con::errorf("NeuralNetObject load: invalid filename!");
            return false;
        }

        FILE *in = fopen(filename, "r");
        if (!in) {
            Con::errorf("NeuralNetObject load: failed to open file: %s for read.", filename);
            return false;
        }
        genann *ann = genann_read(in);
        fclose(in);
        if (!ann) {
            Con::errorf("NeuralNetObject load:  invalid file format detected: %s", filename);
            return false;
        }

        bool validateOK = true;
        String errors = "Load File validation failed:\n";
        if (inputs > 0 && ann->inputs != inputs) {
            errors = errors + "Inputs missmatch\n";
            validateOK = false;
        }
        if (hidden_layers > 0 && ann->hidden_layers != hidden_layers) {
            errors = errors + "Hidden Layers missmatch\n";
            validateOK = false;
        }
        if (hidden_neuros  > 0 &&  ann->hidden != hidden_neuros) {
            errors = errors + "Hidden Neurons missmatch\n";
            validateOK = false;
        }
        if (outputs  > 0 && ann->outputs != outputs) {
            errors = errors + "Outputs missmatch\n";
            validateOK = false;
        }

        if (!validateOK) {
            Con::errorf("NeuralNetObject: %s", errors.c_str());
            genann_free(ann);
            return false;
        }

        if (!this->resize(ann->inputs, ann->outputs)) {
            Con::errorf("NeuralNetObject load: failed to resize inputs or outputs");
            genann_free(ann);
            return false;
        }

        genann_actfun oldHidden = mAnn ? mAnn->activation_hidden : genann_act_sigmoid_cached;
        genann_actfun oldOutput = mAnn ? mAnn->activation_output : genann_act_sigmoid_cached;

        if (mAnn) genann_free(mAnn);
        mAnn = ann;

        mAnn->activation_hidden = oldHidden;
        mAnn->activation_output = oldOutput;

        return true;
    }
    // ------------------------------------------------------------------------
    // after load or load + mutate call a warmup
    inline bool WarmUp( ) {
        for (S32 i = 0; i < mInputs.size(); i++) mInputs[i] = 0.0;
        return Run();
    }
    // ------------------------------------------------------------------------
    inline bool Run() {
        if (!mAnn) {
            Con::errorf("NeuralNetObject run: not initialized!");
            return false;
        }
        if (mAnn->inputs != mInputs.size()) {
            Con::errorf("NeuralNetObject run: input size missmatch!!!!");
            return false;
        }
        if (mAnn->outputs != mOutputs.size()) {
            Con::errorf("NeuralNetObject run: output size missmatch!!!!");
            return false;
        }

        const F64* prediction = genann_run(mAnn, mInputs.address());
        for (S32 i = 0; i < mAnn->outputs; i++) {
            mOutputs[i] = prediction[i];
        }
        return true;
    }
    // ------------------------------------------------------------------------
    inline bool Train(F64 learningRate) {
        if (!mAnn) {
            Con::errorf("NeuralNetObject train: not initialized!");
            return false;
        }
        if (mAnn->inputs != mInputs.size()) {
            Con::errorf("NeuralNetObject train: input size missmatch!!!!");
            return false;
        }
        if (mAnn->outputs != mOutputs.size()) {
            Con::errorf("NeuralNetObject train: output size missmatch!!!!");
            return false;
        }
        genann_train(mAnn, mInputs.address(), mOutputs.address(), learningRate);
        return true;
    }
    // ------------------------------------------------------------------------
    inline bool Mutate(F64 mutationRate, F64 mutationAmount) {
        if (!mAnn) {
            Con::errorf("NeuralNetObject Mutate: not initialized!");
            return false;
        }
        for (S32 i = 0; i < mAnn->total_weights; i++) {
            if (ElfMath::mRandF64() <= mutationRate) {
                F64 noise = (ElfMath::mRandF64() * 2.0 - 1.0) * mutationAmount;
                mAnn->weight[i] += noise;
            }
        }
        return true;
    }
    // ------------------------------------------------------------------------
    // genann_copy
    inline bool Copy(const NeuralNetObject* parentA) {
        if (!mAnn || !parentA->mAnn ) {
            Con::errorf("NeuralNetObject Copy: Parent object is not initialized!");
            return false;;
        }
        genann *ann = genann_copy(parentA->mAnn);
        if (mAnn) genann_free(mAnn);
        mAnn = ann;

        return true;
    }
    // ------------------------------------------------------------------------
    inline bool Crossover(const NeuralNetObject* parentA, const NeuralNetObject* parentB) {
        if (!mAnn || !parentA->mAnn || !parentB->mAnn) {
            Con::errorf("NeuralNetObject Crossover: one or more objects not initialized!");
            return false;;
        }
        if (
            mAnn->total_weights != parentA->mAnn->total_weights ||
            mAnn->total_weights != parentB->mAnn->total_weights
        ) {
            Con::errorf("NeuralNetObject Crossover: weight size missmatch");
            return false;
        }
        for (S32 i = 0; i < mAnn->total_weights; i++) {
            if (ElfMath::mRandF64() > 0.5) {
                mAnn->weight[i] = parentA->mAnn->weight[i];
            } else {
                mAnn->weight[i] = parentB->mAnn->weight[i];
            }
        }
        return true;
    }

    // ------------------------------------------------------------------------
    inline F64 getInputValue(S32 index) {
        if (index < 0 || index >= mInputs.size()) return 0.0;
        return mInputs[index];
    }
    // ------------------------------------------------------------------------
    inline bool setInputValue(S32 index, F64 value) {
        if (index < 0 || index >= mInputs.size()) return false;
        mInputs[index] = value;
        return true;
    }
    // ------------------------------------------------------------------------
    inline F64 getOutputValue(S32 index) {
        if (index < 0 || index >= mOutputs.size()) return 0.0;
        return mOutputs[index];
    }
    // ------------------------------------------------------------------------
    inline bool setOutputValue(S32 index, F64 value) {
        if (index < 0 || index >= mOutputs.size()) return false;
        mOutputs[index] = value;
        return true;
    }
    // ------------------------------------------------------------------------


}; // class

IMPLEMENT_CONOBJECT(NeuralNetObject);

// -----------------------------------------------------------------------------
// Console methods:
// -----------------------------------------------------------------------------
DefineEngineMethod(NeuralNetObject, init, bool,
            (S32 inputs, S32 hidden_layers, S32 hidden_neuros, S32 outputs ),
            , "initialize network"){
    return object->Init(inputs, hidden_layers, hidden_neuros, outputs);
}

DefineEngineMethod(NeuralNetObject, Copy, bool,(U32 parentObjID),
            ,"deep copy the of a other network") {

    NeuralNetObject* parentA = dynamic_cast<NeuralNetObject*>(Sim::findObject(parentObjID));
    if (!parentA) return false;
    return object->Copy(parentA);
}

DefineEngineMethod(NeuralNetObject, Load, bool,
            (const char* filename, S32 inputs, S32 hidden_layers, S32 hidden_neuros, S32 outputs ),(0,0,0,0)
            , "load network and check parameter to match the loaded data."){
    return object->Load(filename, inputs, hidden_layers, hidden_neuros, outputs);
}
DefineEngineMethod(NeuralNetObject, Save, bool,(const char* filename),
                   ,"Save a network to file") {
    return object->Save(filename);
}
DefineEngineMethod(NeuralNetObject, WarmUP, bool,(), ,"Run with empty input to warmup the network") {
    return object->WarmUp();
}
DefineEngineMethod(NeuralNetObject, Run, bool,(), ,"Run with your input data and set output data") {
    return object->Run();
}
DefineEngineMethod(NeuralNetObject, Train, bool,(F64 learningRate),
            ,"Tain (Backpropagation) your network with your input data and output data") {
    return object->Train(learningRate);
}
DefineEngineMethod(NeuralNetObject, Mutate, bool,(F64 mutationRate, F64 mutationAmount),
            ,"Mutate (Evolutuion) modify the weight of your network") {
    return object->Mutate(mutationRate, mutationAmount);
}



DefineEngineMethod(NeuralNetObject, Crossover, bool,(U32 parentAObjID, U32 parentBObjID),
            ,"Crossover (Evolutuion) from 2 parents") {

    NeuralNetObject* parentA = dynamic_cast<NeuralNetObject*>(Sim::findObject(parentAObjID));
    if (!parentA) return false; //TODO error message ..
    NeuralNetObject* parentB = dynamic_cast<NeuralNetObject*>(Sim::findObject(parentBObjID));
    if (!parentB) return false; //TODO error message ..
    return object->Crossover(parentA, parentB);
}


DefineEngineMethod(NeuralNetObject, getIn, F64,(S32 index),,"Get the input value at index") {
    return object->getInputValue(index);
}
DefineEngineMethod(NeuralNetObject, setIn, bool,(S32 index, F64 value),,"Set the input value at index") {
    return object->setInputValue(index, value);
}

// ---  output
DefineEngineMethod(NeuralNetObject, getOut, F64,(S32 index),,"Get the output value at index") {
    return object->getOutputValue(index);
}
DefineEngineMethod(NeuralNetObject, setOut, bool,(S32 index, F64 value),,"Set the output value at index") {
    return object->setOutputValue(index, value);
}

// ------------------------------------------------------------------------
// activation types
// ------------------------------------------------------------------------

DefineEngineMethod(NeuralNetObject, setActivationHidden, void, (S32 type),, "Sets the hidden layer activation type.\n"
        " default/0: genann_act_sigmoid_cached, 1: genann_act_tanh, 2:genann_act_relu, 3: genann_act_linear"
) {
    if (object->mAnn) {
        object->mAnn->activation_hidden = object->getActivationFunctionByType(type);
    }
}

DefineEngineMethod(NeuralNetObject, setActivationOutput, void, (S32 type),, "Sets the output layer activation type.\n"
     " default/0: genann_act_sigmoid_cached, 1: genann_act_tanh, 2:genann_act_relu, 3: genann_act_linear"
) {
    if (object->mAnn) {
        object->mAnn->activation_output = object->getActivationFunctionByType(type);
    }
}
