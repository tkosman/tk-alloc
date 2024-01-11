extern void testHeapAlloc();
extern void testHeapFree();
extern void testPrintMemoryStats();

#include <stdio.h>

int main() {
    testHeapAlloc();
    testHeapFree();
    testPrintMemoryStats();

    printf("All tests succeded.\n");
    return 0;
}
