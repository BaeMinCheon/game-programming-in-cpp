
---

# Materials of Chapter 02

---

### _INTEGRATING ACTOR AND COMPONENT_
- single-ton pattern
    - making single and global instance of the class
    - causing code change when you need multiple instances
    - taking note of constructor's access specifier, which is `private`

```c++
// definition
class SingleTon
{
public:
    static SingleTon* GetInstance()
    {
        if(!gInstance)
        {
            gInstance = new SingleTon();
        }

        return gInstance;
    }

    // code

private:
    SingleTon();
    ~SingleTon();

    static SingleTon* gInstance;
};
SingleTon* SingleTon::gInstance = nullptr;

// usage
void foo()
{
    auto st = SingleTon::GetInstance();
    auto data = st->GetData();
    // code
}
```

- dependency-injection pattern
    - taking a pointer to manager class at constructor parameter == `making dependency`
    - and you save the pointer as member variable
    - each instance can access to manager class easily

```c++
// definition
class Game
{
	// code
};
class Actor
{
public:
	Actor(class Game* _pg):
		mGame(_pg)
	{
		// code
	}

    void foo();

private:
	class Game* mGame;
};

// usage
void Actor::foo()
{
    auto data = mGame->GetData();
    // code
}
```

### _TEXTURE IN SDL_
- `IMG_Init()`
    - you can load 2D image files by initializing SDL image library with this function
    - parameter of `IMG_Init()` can be mixed like `IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)`
- `IMG_Load()`
    - taking filename as a parameter and returning `SDL_Surface*`
    - struct `SDL_Surface` contains information of the image
    - you can render image with `SDL_Surface` only in CPU
- `SDL_CreateTextureFromSurface()`
    - taking `SDL_Renderer*, SDL_Surface*` and returning `SDL_Texture*`
    - struct `SDL_Texture` is similar to `SDL_Surface` but optimized for your GPU
    - you can render image with `SDL_Texture` only in GPU
- more information for this topic, visit here https://stackoverflow.com/questions/21392755/difference-between-surface-and-texture-sdl-general

### _EXERCISE 2.2_
- making `AnimSpriteComponent` supporting multiple animation
- defining each animation according to `Character.txt`
- there are total 3 animations `Walking, Jumping and Punching`
- preventing animation repeated, this means animation stops when it reaches end frame

### _EXERCISE 2.3_
- making map by loading `MapLayer1.csv, MapLayer2.csv and MapLayer3.csv`
- reading `csv` file and saving its data
- defining `TileMapComponent` for handling tile maps
- draw order : `MapLayer3.csv` → `MapLayer2.csv` → `MapLayer1.csv`