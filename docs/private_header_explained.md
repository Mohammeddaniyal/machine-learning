# Understanding Private Headers in C Libraries

## 1. Your Question
You asked:
- How does keeping a header like `___mlfw_error.h` in `private/include/` make it private?
- If internal code uses that header, why don’t we need to ship it to users?
- Why are private headers only needed at compile time, not at runtime?

---

## 2. Public vs Private Headers

- **Public headers (`include/mlfw_error.h`)**
  - Part of the official API of your library.
  - Distributed to users along with the compiled library (`.a` or `.so`).
  - Users include these in their code to use your library.

- **Private headers (`private/include/___mlfw_error.h`)**
  - Only used by the library’s internal `.c` files during compilation.
  - Not distributed to users — kept internal.
  - Users never need them, because the compiled library already contains the machine code.

---

## 3. Why Private Headers Are Not Installed

When you build a library:
1. **During compilation**
   - Your `.c` files include both public and private headers.
   - The compiler uses macros, constants, and prototypes from the private headers.
   - These get baked into the object files (`.o`) and then into your library (`.so`/`.a`).

2. **When distributing the library**
   - You install:
     - `libmlfw.so` or `libmlfw.a`
     - `include/mlfw_error.h`
   - You do **not** install:
     - `private/include/___mlfw_error.h`

3. **When users link against your library**
   - They only include `mlfw_error.h`.
   - They link against the precompiled binary (`libmlfw.so`).
   - They don’t need the private header, since it was only required during compilation.

---

## 4. Compile-Time vs Runtime

### a. Macros (`#define`)
Example:
```c
#define ERR_CODE 42
````

When used:

```c
return ERR_CODE;
```

The preprocessor replaces it with:

```c
return 42;
```

✅ At runtime, only the value `42` exists — no macro.

### b. Inline or Static Helpers

```c
static inline int helper(int x) { return x * 2; }
```

The compiler inserts this directly into `.c` code where it’s used.
✅ At runtime, just normal instructions exist.

### c. Function Prototypes

```c
// in ___mlfw_error.h
int __mlfw_internal_handler(int);
```

The function is actually implemented in a `.c` file.
✅ At runtime, the binary code of the function is in `libmlfw.so` — the header is not needed anymore.

---

## 5. Core Idea

* **Headers**: Instructions for the compiler (compile-time only).
* **Libraries**: Compiled machine code (runtime).
* **Private headers** are like blueprints: once the building (your library) is complete, you don’t give the blueprints to the user.

---

## 6. Visual Pipeline

```
Source Code (.c + .h)
       ↓  Preprocessor (#include, macros)
Expanded Source (macros replaced)
       ↓  Compiler
Object Code (.o)
       ↓  Linker
Library (.a / .so)
       ↓  Runtime
Executable uses only the compiled library (no headers needed)
```

---

## ✅ In Short

* Private headers are **needed to build the library**, but not **needed to use it**.
* At **compile-time**, macros and declarations from private headers guide compilation.
* At **runtime**, only the compiled machine code is used, so the private headers disappear.

