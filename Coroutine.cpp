#include "pch.h"
#include "Coroutine.h"

bool Coroutine::Step(float deltaTime) {
    if (finished) return false;

    bool running = func(deltaTime);
    if (!running) finished = true;
    return running;
}

bool Coroutine::IsFinished() const {
    return finished;
}

void CoroutineManager::Start(Coroutine::StepFunc func) {
    coroutines.emplace_back(func);
}

void CoroutineManager::Update(float deltaTime) {
    for (int i = (int)coroutines.size() - 1; i >= 0; i--) {
        if (!coroutines[i].Step(deltaTime)) {
            coroutines.erase(coroutines.begin() + i);
        }
    }
}