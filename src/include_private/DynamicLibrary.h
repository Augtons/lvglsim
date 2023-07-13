#ifndef LVGLSIM_DYNAMICLIBRARY_H
#define LVGLSIM_DYNAMICLIBRARY_H

class DynamicLibrary {
public:
    explicit DynamicLibrary(char *name): libName(name) { }

    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary& operator=(const DynamicLibrary&) = delete;

    DynamicLibrary(DynamicLibrary&&) noexcept = default;
    DynamicLibrary& operator=(DynamicLibrary&&) noexcept = default;

    bool load();
    void free();

    template<typename Ret, typename... Args>
    Ret(*getFunction(const char *funcName))(Args...) {
        return (Ret(*)(Args...))getFunction(funcName);
    }

private:
    char *libName;

    void *libHandle = nullptr;
    void *getFunction(const char *funcName);

};

#endif //LVGLSIM_DYNAMICLIBRARY_H
