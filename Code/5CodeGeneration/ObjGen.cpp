//目标代码生成
#include "ObjGen.h"

//void ObjCodeGen(CodeGenContext& context, string& filename, int outputType) {
void ObjCodeGen(CodeGenContext& context, string& filename) {
    cout<<"ObjCodeGen"<<endl;

    //初始化llvm，设置目标机
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    //获取目标三元组并设置
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    context.Module->setTargetTriple(targetTriple);

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
    context.Module->setDataLayout(theTargetMachine->createDataLayout());

    //int outputType = 0;
    llvm::CodeGenFileType type;
    if(filename[filename.size()-1] == 's'){    //生成汇编文件
        type = llvm::CGFT_AssemblyFile;
    }
    else if(filename[filename.size()-1] == 'o'){ //生成目标文件
        type = llvm::CGFT_ObjectFile;
    }
    else{
        llvm::errs() << "Unsupported target file format: " + filename;
        return;
    }

    //将目标代码输出到文件
    error_code ErrCode;
    llvm::raw_fd_ostream dest(filename.c_str(), ErrCode, llvm::sys::fs::OF_None);

    llvm::legacy::PassManager pass;
    if(theTargetMachine->addPassesToEmitFile(pass, dest, &dest, type)){
        llvm::errs() << "theTargetMachine can't emit a file of this type";
        return;
    }
    
    pass.run(*(context.Module));
    dest.flush();

    //打印LLVM IR
    //context.Module->print(llvm::outs(), nullptr);

    llvm::outs() << "Object code wrote to " << filename.c_str() << "\n";
}
