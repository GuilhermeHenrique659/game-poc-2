#include "Timer.h"

/**
 * @param accumulator Ponteiro para uma variável float que guarda o tempo decorrido.
 * @param interval O intervalo em segundos (ex: 1.0f para 1 segundo).
 * @param action A função ou lambda a ser executada.
 */
void RunEvery(float *accumulator, float interval, std::function<void()> action) {
    // GetFrameTime() retorna o tempo desde o último frame (ex: 0.016s para 60fps)
    *accumulator += GetFrameTime();

    if (*accumulator >= interval) {
        action();          // Executa a estratégia
        *accumulator = 0;  // Reseta o timer
        // Use *accumulator -= interval; se quiser precisão absoluta em eventos rápidos
    }
}