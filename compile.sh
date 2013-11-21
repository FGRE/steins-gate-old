g++ -I include -lsfml-graphics -lsfml-window -lnpa -lsfml-system \
-ggdb -Wl,-rpath ../libnpengine -Wl,-rpath ../libnpa -I ../libnpengine/include \
-lnpengine -L ../libnpa/ -L ../libnpengine/ -std=c++11 -o steins-gate \
-Wall -Werror src/* -lsfeMovie
