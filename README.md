# pong

A remake of the Pong game. My first game in C++.

### Tools

For this project I'm using C++ and a graphics library called SDL. For running the game it's necessary to download the library following the instructions in its website, and compile the project with the following command:

```bash
<compiler> -o pong <all-cpp-files> `sdl2-config --cflags --libs`
```

This command will generate an executable `pong` to run the game.