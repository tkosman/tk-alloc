buildLib:
	@mkdir -p ./bin
	@cd .. && gcc -Wall -Wextra -Wno-deprecated-declarations -shared -fPIC ./src/*.c -I./include -o e2eTests/bin/heapAllocator.so

compileTests:
	@mkdir -p ./bin
	@cd .. && gcc -Wall -Wextra -Wno-deprecated-declarations ./src/*.c ./e2eTests/testSegFault.c -I./include -o e2eTests/bin/seq_fault.out

runTests:
	@pytest

cleanTests:
	@rm -rf ./bin