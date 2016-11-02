# Menge


## Mac OSX

Steps to follow in order to get `Menge` compiled:

```bash
- cd Menge/projects/g++
- mkdir `build` # optional, but handy
- cmake -D CMAKE_MACOSX_RPATH=1 .. 
- make
```


@todo: with `g++` there are some linking errors. See also the discussion in #28. 
