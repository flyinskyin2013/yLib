### User usage

1. Create yCompilerInstance
```cpp
compiler_instance((void *)new ycompiler::yCompilerInstance(), compiler_instance_deleter)
```


2. Set action type
```cpp
_invocation_helper->getFrontendOpts().act_type = ycompiler::ActionKind::PARSE_YCONFIG_AST;
```


3. Init filemgr
```cpp
if (!GetCompilerInstanceRawPtr(compiler_instance)->GetFileManger()->InitFileManager(file_path)){

    yLib::yLog::E("yConfig", "open cfg file(%s) failed\n", file_path.c_str());
    return -1;
}
```

4. Create action by action-type, run compiler instance
```cpp
// Create and execute the frontend action.
std::unique_ptr<FrontendAction> _act(CreateFrontendAction(*GetCompilerInstanceRawPtr(compiler_instance)));

if (!GetCompilerInstanceRawPtr(compiler_instance)->ExecuteAction(*_act.get())){

    LOGE("yConfig")<<"Run yconfig parse action failed.";
    return -1;
}
```




### yCompiler details
```txt
yCompilerInstance->ExecuteAction()
    -->SpecialAction->Execute()
        -->ParseAST()
```