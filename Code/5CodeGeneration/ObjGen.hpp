//目标代码生成
#ifndef __OBJGEN_HPP__
#define __OBJGEN_HPP__

#define ASSEMBLY_FILE 1
#define OBJECT_FILE 2

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

void ObjCodeGen(CodeGenContext& context, string& filename, int outputType) {
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

    if(outputType == ASSEMBLY_FILE){    //生成汇编文件
        //llvm::CodeGenFileType type = llvm::CGFT_AssemblyFile;
        llvm::LLVMTargetMachine::CodeGenFileType type = llvm::LLVMTargetMachine::CGFT_AssemblyFile;
    }
    else if(outputType == OBJECT_FILE){ //生成目标文件
        //llvm::CodeGenFileType type = llvm::CGFT_ObjectFile;
        llvm::LLVMTargetMachine::CodeGenFileType type = llvm::LLVMTargetMachine::CGFT_ObjectFile;
    }
    else{
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
