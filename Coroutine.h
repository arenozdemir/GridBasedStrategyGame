#pragma once
#include <functional>
#include <vector>

struct Coroutine {
    using StepFunc = std::function<bool(float)>;

    StepFunc func;
    bool finished;

    Coroutine(StepFunc f) : func(f), finished(false) {}

    bool Step(float deltaTime);
    bool IsFinished() const;
};

struct CoroutineManager {
    std::vector<Coroutine> coroutines;

    void Start(Coroutine::StepFunc func);
    void Update(float deltaTime);
};