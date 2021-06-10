#ifndef SCENE_H
#define SCENE_H

class Scene {
public:
    virtual bool Update(float deltaTime) = 0;
    virtual void Render(SDL_Renderer* aRenderer) = 0;
    bool isFinished() {return finished; };
private:
    bool finished;
};

#endif // SCENE_H