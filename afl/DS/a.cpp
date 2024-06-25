// #include <iostream>
// #include <signal.h>
// #include <sanitizer/asan_interface.h>

// // 사용자 정의 시그널 핸들러
// void signalHandler(int sig) {
//     std::cerr << "Caught signal: " << sig << std::endl;

//     // ASan의 내장 시그널 핸들러 호출
//     __sanitizer_print_stack_trace();

//     // 시그널 핸들러를 기본 핸들러로 재설정
//     signal(sig, SIG_DFL);

//     // 기본 핸들러 호출 (ASan 핸들러)
//     raise(sig);
// }

// int main() {
//     // SIGABRT 시그널에 대한 사용자 정의 핸들러 설정
//     signal(SIGABRT, signalHandler);

//     // SIGBUS 시그널에 대한 사용자 정의 핸들러 설정
//     signal(SIGBUS, signalHandler);

//     // SIGSEGV 시그널에 대한 사용자 정의 핸들러 설정
//     signal(SIGSEGV, signalHandler);

//     // ASan의 오류 처리 방식을 변경하여 오류 발생 시 종료하지 않고 핸들러를 호출하도록 설정
//     __asan_set_error_report_callback([](const char* report) {
//         std::cerr << report << std::endl;
//         signalHandler(SIGABRT); // SIGABRT 핸들러 호출
//     });

//     // 예제용으로 heap buffer overflow 발생
//     int *ptr = new int[10];
//     ptr[10] = 42; // ptr 배열의 인덱스를 벗어난 위치에 값 할당

//     // 메모리 해제
//     delete[] ptr;

//     return 0;
// }

#include <iostream>
#include <signal.h>
#include <sanitizer/asan_interface.h>

// 사용자 정의 시그널 핸들러
void customSigsegvHandler(int sig, siginfo_t *info, void *context) {
    std::cerr << "Caught signal: " << sig << std::endl;

    // ASan의 내장 시그널 핸들러 호출
    __sanitizer_print_stack_trace();

    // 시그널 핸들러를 기본 핸들러로 재설정
    signal(sig, SIG_DFL);

    // 기본 핸들러 호출 (ASan 핸들러)
    raise(sig);
}

// ASan 오류 콜백 함수
void asanErrorCallback(const char* report) {
    std::cerr << "ASan error report: " << report << std::endl;
}

// 시그널 핸들러 설정 함수
void setSignalHandlers() {
    struct sigaction sa;
    sa.sa_sigaction = customSigsegvHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_NODEFER;

    sigaction(SIGABRT, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGILL, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);
}

// ASan 설정 함수
extern "C" const char* __asan_default_options() {
    return "allow_user_segv_handler=1";
}

int main() {
    // 시그널 핸들러 설정
    setSignalHandlers();

    // ASan의 오류 처리 방식을 변경하여 오류 발생 시 종료하지 않고 콜백을 호출하도록 설정
    __asan_set_error_report_callback(asanErrorCallback);

    // 예제용으로 힙 버퍼 오버플로우 발생
    int *ptr = new int[10];
    ptr[10] = 42; // ptr 배열의 인덱스를 벗어난 위치에 값 할당

    // 메모리 해제
    delete[] ptr;

    return 0;
}

