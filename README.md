# Algo_Project

이것은 알고리즘 수업에서 진행한 프로젝트에 관한 코드입니다.


## dynamic.c
문제 1 입니다. [최적 이진 탐색 트리 (OBST) 구현]

개발 언어는 c

개발 환경는 윈도우

컴파일러는 gcc

### 컴파일
#### Linux and Mac
```
gcc -o dynamic dynamic.c -lm
./dynamic
```

#### Windows
```
MSYS2 설치

이후...

gcc -o dynamic.exe dynamic.c -lm
./dynamic.exe
```


## bridge.c
문제 2 입니다. [브리지(Bridge) 테스트 및 찾기]

개발 언어는 c

개발 환경는 윈도우

컴파일러는 gcc

### 컴파일
#### Linux and Mac
```
gcc -o bridge bridge.c
./bridge
```

#### Windows
```
MSYS2 설치

이후...

gcc -o bridge.exe bridge.c
./bridge.exe
```


## flik_net.cpp
문제 4 입니다. [무선 네트워크]

개발 언어는 c++ (버전 11 이상)

개발 환경은 맥

컴파일러는 gcc

사용 라이브러리 : STL, FLIK

### FLIK 세팅 및 컴파일 / 실행 법
#### Linux
```
sudo apt update
sudo apt install libfltk1.3-dev
g++ -std=c++11 flik_net.cpp -o flik_net `fltk-config --cxxflags --ldflags`
./flik_net
```

#### Windows
```
MSYS2 설치

이후...

pacman -S mingw-w64-x86_64-fltk
g++ -std=c++11 flik_net.cpp -o flik_net.exe `fltk-config --cxxflags --ldflags`
./flik_net.exe
```

#### Mac
```
homebrew로 설치 진행

brew install fltk
g++ -std=c++11 flik_net.cpp -o flik_net `fltk-config --cxxflags --ldflags`
./flik_net
```


### 테스트 값 1

```
18 7.5

50 50 0
52 50 0
48 50 0
50 52 0
50 48 0
51 51 0
49 49 0
55 53 0
53 55 0
45 47 0
47 45 0
56 50 0
44 50 0
50 56 0
58 50 0
42 50 0
50 58 0
50 42 0
```

### 테스트 값 2

```
16 6.0

20 20 0
20 25 0
25 20 0
25 25 0
60 20 0
60 25 0
65 20 0
65 25 0
20 60 0
20 65 0
25 60 0
25 65 0
60 60 0
60 65 0
65 60 0
65 65 0
```
