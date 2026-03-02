#include "raylib.h"
#include <functional>

/**
 * @param accumulator Ponteiro para uma variável float que guarda o tempo decorrido.
 * @param interval O intervalo em segundos (ex: 1.0f para 1 segundo).
 * @param action A função ou lambda a ser executada.
 */
void RunEvery(float *accumulator, float interval, std::function<void()> action);