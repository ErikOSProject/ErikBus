/**
 * @file bus.c
 * @brief Bus communication functions.
 * 
 * This file contains functions for managing services, interfaces, and methods
 * on the bus. It includes functions for pushing and popping parameters to/from
 * the bus, as well as for invoking methods and services.
 */

#include <erikbus.h>

size_t strlen(const char *str);
void *malloc(size_t size);
void free(void *ptr);
int64_t _syscall(enum syscall_type type, void *data);

/**
 * @brief Finds a service by name.
 * 
 * This function searches for a service with the specified name on the bus.
 * It returns the service ID if found, or -1 if not found.
 * 
 * @param name The name of the service to find.
 * 
 * @return The service ID if found, or -1 if not found.
 */
int64_t _syscall_service(char *name)
{
	struct syscall_method_data m = { 0, 1, 0 };
	struct syscall_param p = { .type = SYSCALL_PARAM_ARRAY,
				   .size = strlen(name),
				   .array = (void *)name };
	_syscall(SYSCALL_PUSH, &p);
	return _syscall(SYSCALL_METHOD, &m);
}

/**
 * @brief Finds an interface by name.
 * 
 * This function searches for an interface with the specified name on the bus.
 * It returns the interface ID if found, or -1 if not found.
 * 
 * @param service The service ID to search within.
 * @param name The name of the interface to find.
 * 
 * @return The interface ID if found, or -1 if not found.
 */
int64_t _syscall_interface(int64_t service, char *name)
{
	struct syscall_method_data m = { service, 0, 0 };
	struct syscall_param p = { .type = SYSCALL_PARAM_ARRAY,
				   .size = strlen(name),
				   .array = (void *)name };
	_syscall(SYSCALL_PUSH, &p);
	return _syscall(SYSCALL_METHOD, &m);
}

/**
 * @brief Finds a method by name.
 * 
 * This function searches for a method with the specified name on the bus.
 * It returns the method ID if found, or -1 if not found.
 * 
 * @param service The service ID to search within.
 * @param interface The interface ID to search within.
 * @param name The name of the method to find.
 * 
 * @return The method ID if found, or -1 if not found.
 */
int64_t _syscall_method(int64_t service, int64_t interface, char *name)
{
	struct syscall_method_data m = { service, 0, 1 };
	struct syscall_param p1 = { .type = SYSCALL_PARAM_PRIMITIVE,
				    .value = interface };
	struct syscall_param p2 = { .type = SYSCALL_PARAM_ARRAY,
				    .size = strlen(name),
				    .array = (void *)name };
	_syscall(SYSCALL_PUSH, &p1);
	_syscall(SYSCALL_PUSH, &p2);
	return _syscall(SYSCALL_METHOD, &m);
}

/**
 * @brief Pushes a string onto the bus.
 * 
 * This function pushes a string onto the bus for communication with other
 * services. It returns the result of the syscall.
 * 
 * @param str The string to push onto the bus.
 * 
 * @return The result of the syscall.
 */
int64_t _syscall_push_string(char *str)
{
	struct syscall_param p = { .type = SYSCALL_PARAM_ARRAY,
				   .size = strlen(str),
				   .array = (void *)str };
	return _syscall(SYSCALL_PUSH, &p);
}

/**
 * @brief Pushes an integer onto the bus.
 * 
 * This function pushes an integer onto the bus for communication with other
 * services. It returns the result of the syscall.
 * 
 * @param value The integer to push onto the bus.
 * 
 * @return The result of the syscall.
 */
int64_t _syscall_push_int(int64_t value)
{
	struct syscall_param p = { .type = SYSCALL_PARAM_PRIMITIVE,
				   .value = value };
	return _syscall(SYSCALL_PUSH, &p);
}

/**
 * @brief Pops a string from the bus.
 * 
 * This function pops a string from the bus and allocates memory for it.
 * If the operation fails, it frees the allocated memory and sets the string
 * pointer to NULL.
 * 
 * @param str Pointer to the string to be popped.
 * 
 * @return The result of the syscall.
 */
int64_t _syscall_pop_string(char **str)
{
	struct syscall_param p = { .type = SYSCALL_PARAM_ARRAY,
				   .size = 0,
				   .array = NULL };
	int64_t ret = _syscall(SYSCALL_PEEK, &p);
	if (ret < 0)
		return ret;
	if (p.type != SYSCALL_PARAM_ARRAY)
		return -1;
	*str = malloc(p.size);
	if (!*str)
		return -1;
	p.array = *str;
	ret = _syscall(SYSCALL_POP, &p);
	if (ret < 0) {
		free(*str);
		*str = NULL;
	}
	return ret;
}

/**
 * @brief Pops an integer from the bus.
 * 
 * This function pops an integer from the bus and stores it in the provided
 * pointer. It returns the result of the syscall.
 * 
 * @param value Pointer to the integer to be popped.
 * 
 * @return The result of the syscall.
 */
int64_t _syscall_pop_int(int64_t *value)
{
	struct syscall_param p = { .type = SYSCALL_PARAM_PRIMITIVE,
				   .value = 0 };
	int64_t ret = _syscall(SYSCALL_PEEK, &p);
	if (ret < 0)
		return ret;
	if (p.type != SYSCALL_PARAM_PRIMITIVE)
		return -1;
	ret = _syscall(SYSCALL_POP, &p);
	*value = p.value;
	return ret;
}
