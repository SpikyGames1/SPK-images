# SPK-images
![image](https://github.com/user-attachments/assets/267282a6-e6a6-46e5-a659-f091095178e9)

A new way to view images in your computer! featuring 2 MB of more storage taken by some random file!

in order to run the `spkimageviewer.c` file, you must have a C Compiler (GCC recommended), SDL2 (Simple DirectMedia Layer) installed on your computer through your package manager. Once done, compile the project through your C compiler, here are some examples:

# CLang:
```
clang -o spk_viewer spkimageviewer.c -lSDL2
```

# GCC:
```
gcc -o spk_viewer spkimageviewer.c -lSDL2
```

# TinyCC (TCC):
```
tcc -o spk_viewer spkimageviewer.c -lSDL2
```

Once that is done you should have a `spk_viewer` file in the same directory as the `spkimageviewer.c` file. You can convert PNG into SPK and view them instantly by using:
```
./spk_viewer <input png file> <output spk file>
```
Make sure to replace <input png file> and <out spk file> with the appropriate file extension.

(I know everyone think C is outdated but still is more cool than 50% of people out there.... JKJK =P)
(either way you are the best and have an amazing day!)
