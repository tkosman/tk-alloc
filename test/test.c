extern void testHeapAlloc();
extern void testHeapFree();

#include <stdio.h>

int main() {
    testHeapAlloc();
    testHeapFree();

    //TODO: Finish those tests.
    // testMemoryLeak();
    // testBlockIntegrity();

    printf("All tests succeded.\n");
    return 0;
}
