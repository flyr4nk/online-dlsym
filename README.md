# online-dlsym

This is a proof of concept of how to transfer code from one process to another, and then compile and link it to the receiving process.

As an example, `Alice` sends `myfile.c` containing `int my_function(int)` to `Bob`. After receiving, `Bob` compiles `myfile.c`, dynamically links it and is able to to call `my_function` in the same execution.

You could for example use it to update your server without needing to restart it. Beware that the current implementation is probably not very safe since adversaries could put viruses in the transferred code.

NOTE: Only tested on macOS High Sierra.
