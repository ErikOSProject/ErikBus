/**
 * @file erikbus.h
 * @brief Header file for ErikOS bus system calls.
 * 
 * This file contains declarations for system calls related to the ErikOS bus
 * system. It includes functions for managing services, interfaces, and methods
 * on the bus. The bus system allows for communication between different
 * components of the operating system.
 */

#ifndef _ERIKBUS_H
#define _ERIKBUS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum syscall_type {
	SYSCALL_EXIT,
	SYSCALL_METHOD,
	SYSCALL_SIGNAL,
	SYSCALL_TARGETED_SIGNAL,
	SYSCALL_PUSH,
	SYSCALL_PEEK,
	SYSCALL_POP,
};

enum syscall_param_type {
	SYSCALL_PARAM_ARRAY,
	SYSCALL_PARAM_PRIMITIVE,
};

struct syscall_method_data {
	uint64_t pid;
	uint64_t interface;
	uint64_t method;
};

struct syscall_param {
	enum syscall_param_type type;
	size_t size;
	union {
		uint64_t value;
		void *array;
	};
};

#define DECLARE_SERVICE(ident) int64_t bus_service_##ident(void);
#define DECLARE_INTERFACE(service, ident) \
	int64_t bus_interface_##service##_##ident(void);
#define DECLARE_METHOD(service, interface, ident, ...) \
	int64_t bus_method_##service##_##interface##_##ident(void);

#define DEFINE_SERVICE(ident, name)                       \
	int64_t bus_service_##ident(void)                 \
	{                                                 \
		static int64_t service = -1;              \
		if (service == -1)                        \
			service = _syscall_service(name); \
		return service;                           \
	}

#define DEFINE_INTERFACE(service, ident, name)                  \
	int64_t bus_interface_##service##_##ident(void)         \
	{                                                       \
		static int64_t interface = -1;                  \
		if (interface == -1)                            \
			interface = _syscall_interface(         \
				bus_service_##service(), name); \
		return interface;                               \
	}

#define DEFINE_METHOD(service, interface, ident, name, ...)              \
	int64_t bus_method_##service##_##interface##_##ident(void)       \
	{                                                                \
		static int64_t method = -1;                              \
		if (method == -1)                                        \
			method = _syscall_method(                        \
				bus_service_##service(),                 \
				bus_interface_##service##_##interface(), \
				name);                                   \
		return method;                                           \
	}

int64_t _syscall(enum syscall_type type, void *data);

int64_t _syscall_service(char *name);
int64_t _syscall_interface(int64_t service, char *name);
int64_t _syscall_method(int64_t service, int64_t interface, char *name);

int64_t _syscall_push_string(char *str);
int64_t _syscall_push_int(int64_t value);

int64_t _syscall_pop_string(char **str);
int64_t _syscall_pop_int(int64_t *value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _ERIKBUS_H
