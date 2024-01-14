//implement a central memory allocation system in order to manage memory alloc/dealloc in one place.

static unsigned char memory_buffer[1024 * 1024 * 5]; // 1024 bytes x 1024 bytes x 5 = 5MB
static uint32_t offset;

unsigned char* request_memory(uint32_t mem_size){
	unsigned char* ptr = &memory_buffer[offset];
	offset += mem_size;
	return ptr;
}
