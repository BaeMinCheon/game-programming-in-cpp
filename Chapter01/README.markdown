
# How to run this project ? `on Windows 10`

---

### _A1. NOT HAVE VS2017_
1. download visual studio community 2017 installer at https://imagine.microsoft.com/ko-kr/Catalog/Product/530
2. execute the installer
3. select and install proper work-load `Game development with C++`

### _A2. ALREADY HAVE VS2017_
1. open visual studio 2017
2. `Tools` → `Get Tools and Features...`
3. check whether proper work-load installed `Game development with C++`

---

### _B. PREPARE CODE_
1. download code from https://github.com/gameprogcpp/code
2. open solution in chapter folder with visual studio 2017

---

### _C. RUN PROJECT_
1. build solution
2. if you encounter build failure for Windows SDK version issue, right-click solution and select `Retarget Solution`

---

# Materials of Chapter 01

---

### _INPUT PROCESSING_
- background
    - SDL maintains a queue saving messages from operation system
    - the messages in the queue may or may not be about input, so you should use `switch()` or else for filtering
- `SDL_PollEvent()`
    - if there is event(s) in the queue, returns `true` and assigns event value into parameter
    - otherwise, just returns `false`
    - as a queue, the event assigned is removed and next event will be located in head

### _DELTA TIME_
- means the interval between last frame and current frame
- in other words, the game has passed for delta time from last frame

### _EXERCISE 1.1_
- making the Pong for 2 players
- replacing right wall to second paddle
- separating input keys for each player
- checking collision with second paddle

### _EXERCISE 1.2_
- making the Pong for multiple balls
- maintaining balls in some container like `std::vector`
- updating balls in the container at the same time