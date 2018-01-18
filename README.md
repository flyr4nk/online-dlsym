# online-dlsym

This is a proof of concept of how to transfer code from one process to another, and then compile and link it to the receiving process.

For example, `Alice` sends `file.c` containing `int func(int)` to `Bob`. After receiving, `Bob` compiles `file.c`, dynamically links it and is able to to call `func` in the same execution.

```
┌───────────────────┐   ┌───────────────────┐
│ file.c            │   │ file.c            │
├───────────────────┤   ├───────────────────┤
│ int func(int x) { │   │ int func(int x) { │ 4. compile  ┌─────────┐
│   return x*x;     │   │   return x*x;     │────────────>│ file.so │
│ }                 │   │ }                 │             └─────────┘
└───────────────────┘   └───────────────────┘                  │
          │                       ^                            │
          │ 1. read               │ 3. write                   │
          v                       │                            │
      ┌───────┐  2. transfer  ┌───────┐    5. dynamically link │
      │ Alice │──────────────>│  Bob  │<───────────────────────┘
      └───────┘               └───────┘
                                  │
                                  │ 6. execute
                                  v
                         ┌──────────────────┐
                         │ int y = func(5); │
                         └──────────────────┘
```

You could for example use it to update your server without needing to restart it. Beware that the current implementation is probably not very safe since adversaries could put viruses in the transferred code.

NOTE: Only tested on macOS High Sierra.
