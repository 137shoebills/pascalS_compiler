//目标代码生成
#include "ObjGen.h"

void ObjCodeGen(CodeGenContext& context, string& filename, int outputType) {
    //初始化llvm，设置目标机
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    //获取目标三元组并设置
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    context.module->setTargetTriple(targetTriple);

    //获取目标机数据类型并设置
    string TargetError;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, TargetError);
    if(!target){
        llvm::errs()<<TargetError;
        return;
    }
    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions options;
    auto relocationModel = llvm::Reloc::Model::PIC_;
    auto theTargetMachine = target->createTargetMachine(targetTriple, CPU, features, options, relocationModel);
    context.module->setDataLayout(theTargetMachine->createDataLayout());
    context.module->setTargetTriple(targetTriple);

    //将目标代码输出到文件
    error_code ErrCode;
    llvm::raw_fd_ostream dest(filename, ErrCode, llvm::sys::fs::OF_None);
    llvm::legacy::PassManager pass;

    llvm::CodeGenFileType type;
    if(outputType == ASSEMBLY_FILE){    //生成汇编文件
        type = llvm::CGFT_AssemblyFile;
        //llvm::LLVMTargetMachine::CodeGenFileType type = llvm::LLVMTargetMachine::CGFT_AssemblyFile;
    }
    else if(outputType == OBJECT_FILE){ //生成目标文件
        type = llvm::CGFT_ObjectFile;
        //llvm::LLVMTargetMachine::CodeGenFileType type = llvm::LLVMTargetMachine::CGFT_ObjectFile;
    }
    else{
        cout<<"UNSUPPORTED output file format"<<endl;
        return;
    }

    theTargetMachine->addPassesToEmitFile(pass, dest, nullptr, type);
    pass.run(*(context.module));
    dest.flush();

    //打印LLVM IR
    context.module->print(llvm::outs(), nullptr);

    llvm::outs() << "Object code wrote to " << filename.c_str() << "\n";
}