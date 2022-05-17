//目标代码生成
#ifndef __OBJGEN_HPP__
#define __OBJGEN_HPP__

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/ADT/Optional.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>

#include "CodeGen.h"    //LLVM IR生成

using namespace std;

void ObjCodeGen(CodeGenContext& context, string& filename);

/*
class CodeGenBlock{
public:
    llvm::BasicBlock* bb;
    llvm::Value* retValue;

    map<string, llvm::Value*> name_Value;
    map<string, shared_ptr<Identifier>> name_type;
}

class CodeGenContext{
private:
    vector<CodeGenBlock*> blockStack;

public:
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder;
    unique_ptr<llvm::Module> module;    //目标代码生成要用到
}

*/

void ObjCodeGen(CodeGenContext& context, string& filename) {
    //初始化llvm，设置目标机
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    //获取目标三元组并设置
    auto targetTriple = sys::getDefaultTargetTriple();
    context.module->setTargetTriple(targetTriple);

    //获取目标机数据类型并设置
    string TargetError;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, TargetError);
    if(!target){
        errs()<<TargetError;
        return;
    }
    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions options;
    auto relocationModel = llvm::Reloc::Model::PIC_;
    auto theTargetMachine = target->createTargetMachine(TargetTriple, CPU, features, options, relocationModel);
    context.module->setDataLayout(theTargetMachine->createDataLayout());
    
    //将目标代码输出到文件
    error_code ErrCode;
    llvm::raw_fd_ostream dest(filename, ErrCode, llvm::sys::fs::OF_None);
    llvm::legacy::PassManager pass;

    if(filename[filename.size()-1] == 's'){     //生成汇编文件
        llvm::CodeGenFileType type = llvm::CGFT_AssemblyFile;
    }
    else if(filename[filename.size()-1] == 'o'){    //生成可重定位目标文件
        llvm::CodeGenFileType type = llvm::CGFT_ObjectFile;
    }
    else{   //.s和.o以外的后缀
        cout<<"UNSUPPORTED output file format"<<endl;
        return;
    }

    theTargetMachine->addPassesToEmitFile(pass, dest, nullptr, type);
    pass.run(*context.module.get());
    dest.flush();

    //打印LLVM IR
    context.module->print(llvm::outs(), nullptr);
}

#endif