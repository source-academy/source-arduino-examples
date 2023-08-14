# SevSeg Library Port

The examples folder includes some example programs written in Source. These programs can be compiled using this [web demo](https://angelsl.github.io/sinter/).

Do a hexdump with the following command and place the resulting `program.h` at the root of this directory. Upload the sketch to run the program.

```bash
echo -n "const " > program.h && xxd -i program.svm >> program.h
```
